/*Copyright (c) 2014 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/


/*Copyright (c) 2010 Rolf Andreassen

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "Parser.h"
#include <fstream>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/qi.hpp>
#include "..\Log.h"

using namespace boost::spirit;

static void setLHS			(string key);
static void pushObj			();
static void setRHSleaf		(string val);
static void setRHSobject	();
static void setRHSobjlist	();
static void setRHStaglist	(vector<string> val);

static Object* topLevel = NULL;  
vector<Object*> stack; 
vector<Object*> objstack; 

template <typename Iterator>
struct SkipComment : qi::grammar<Iterator>
{
	qi::rule<Iterator> comment;

	SkipComment() : SkipComment::base_type(comment)
	{
		comment = qi::raw[qi::lexeme[lit("#") >> *(iso8859_1::char_ - qi::eol)] >> -qi::eol];
	}
};

static bool debugme = false;
template <typename Iterator>
struct Parser : public qi::grammar<Iterator, SkipComment<Iterator> > {
	static Object* topLevel; 

	// leaf: either left or right side of assignment.  unquoted keyword.
	// example: leaf
	qi::rule<Iterator, string(), SkipComment<Iterator> >	leaf;

	// taglist: a grouping of anonymous (rhs) leaves or strings
	// examples: { TAG TAG TAG } or { "string" "string" TAG }
	qi::rule<Iterator, vector<string>(), SkipComment<Iterator> >	taglist;

	// assign: assignment
	// examples: lhs = rhs or lhs = { lhs = rhs }
	qi::rule<Iterator, SkipComment<Iterator> >	assign;

	// object: a grouping of assignments
	// example: object = { leaf = "string" }
	qi::rule<Iterator, SkipComment<Iterator> >	object;

	// objlist: a grouping of anonymous (rhs) objects
	// example: objlist = { { leaf = "string } { leaf = leaf } }
	qi::rule<Iterator, SkipComment<Iterator> >	objlist;

	// str: a quoted literal string.  may include extended and/or reserved characters.
	// example: "I am a string."
	qi::rule<Iterator, string()>	str;

	// tolleaf: a tolerant leaf.  may include extended and other unreserved characters.  rhs only.
	// example: leaves with accents (names, for instance).
	qi::rule<Iterator, string(), SkipComment<Iterator> >	tolleaf;

	// braces: a stray set of empty rhs braces (without an lhs)
	// EU3 seems to do this for certain decision mods.
	qi::rule<Iterator, SkipComment<Iterator> >	braces;

	// root: evaluation starts here.  objects recurse to root.
	qi::rule<Iterator, SkipComment<Iterator> >	root;

	Parser() : Parser::base_type(root)
	{
		// { }
		braces = *(iso8859_1::space) >> lit('{') >> *(iso8859_1::space) >> lit('}') >> *(iso8859_1::space);

		// a string enclosed in quotes
		str = lexeme[lit('"') >> raw[*(~iso8859_1::char_('"') | lit(0x80) | lit(0x81) | lit(0x82) | lit(0x83) | lit(0x84) | lit(0x85) | lit(0x86) | lit(0x87) | lit(0x88) | lit(0x89) | lit(0x8A) | lit(0x8B) | lit(0x8C) | lit(0x8D) | lit(0x8E) | lit(0x8F) | lit(0x90) | lit(0x91) | lit(0x92) | lit(0x93) | lit(0x94) | lit(0x95) | lit(0x96) | lit(0x97) | lit(0x98) | lit(0x99) | lit(0x9A) | lit(0x9B) | lit(0x9C) | lit(0x9D) | lit(0x9E) | lit(0x9F))] >> lit('"')];

		// a 'forgiving' string without quotes
		tolleaf = raw[+(~iso8859_1::char_("\"{}= \t\r\n") | lit(0x80) | lit(0x81) | lit(0x82) | lit(0x83) | lit(0x84) | lit(0x85) | lit(0x86) | lit(0x87) | lit(0x88) | lit(0x89) | lit(0x8A) | lit(0x8B) | lit(0x8C) | lit(0x8D) | lit(0x8E) | lit(0x8F) | lit(0x90) | lit(0x91) | lit(0x92) | lit(0x93) | lit(0x94) | lit(0x95) | lit(0x96) | lit(0x97) | lit(0x98) | lit(0x99) | lit(0x9A) | lit(0x9B) | lit(0x9C) | lit(0x9D) | lit(0x9E) | lit(0x9F))];

		// a strict string without quotes
		leaf = raw[+(iso8859_1::alnum | iso8859_1::char_("-._:") | lit(0x80) | lit(0x81) | lit(0x82) | lit(0x83) | lit(0x84) | lit(0x85) | lit(0x86) | lit(0x87) | lit(0x88) | lit(0x89) | lit(0x8A) | lit(0x8B) | lit(0x8C) | lit(0x8D) | lit(0x8E) | lit(0x8F) | lit(0x90) | lit(0x91) | lit(0x92) | lit(0x93) | lit(0x94) | lit(0x95) | lit(0x96) | lit(0x97) | lit(0x98) | lit(0x99) | lit(0x9A) | lit(0x9B) | lit(0x9C) | lit(0x9D) | lit(0x9E) | lit(0x9F))];

		// a 
		taglist = lit('{') >> omit[*(iso8859_1::space)] >> lexeme[( ( str | skip[tolleaf] ) % *(iso8859_1::space) )] >> omit[*(iso8859_1::space)] >> lit('}');
		object  = raw[lit('{') >> *(root) >> *(iso8859_1::space) >> lit('}')];
		objlist = raw[lit('{') >> *( *(iso8859_1::space) >> object[&pushObj] ) >> *(iso8859_1::space) >> lit('}')];
		assign  = raw[(*(iso8859_1::space) >> ( leaf[&setLHS] | str[&setLHS]) >> *(iso8859_1::space) >> lit('=')
			>> *(iso8859_1::space) 
			>> ( leaf[&setRHSleaf] | str[&setRHSleaf] | taglist[&setRHStaglist] | objlist[&setRHSobjlist] | object[&setRHSobject] ) 
			>> *(iso8859_1::space))];
		root	= +(assign | braces);

		str.name("str");
		leaf.name("leaf");
		taglist.name("taglist");
		object.name("object");
		objlist.name("objlist");
		assign.name("assign");
		braces.name("braces");
		root.name("root");

		if (debugme)
		{
			debug(str);
			debug(leaf);
			debug(taglist);
			debug(object);
			debug(objlist);
			debug(assign);
			debug(braces);
			debug(root);
		}
	}
};

Object* getTopLevel()
{
	return topLevel;
}

void initParser()
{
	topLevel = new Object("topLevel");
}

string bufferOneObject(ifstream& read)
{
	int openBraces = 0;
	string currObject, buffer;
	bool topLevel = true;
	while (read.good())
	{
		getline(read, buffer);
		if (buffer == "CK2txt")
		{
			continue;
		}
		else if (buffer == "EU3txt")
		{
			continue;
		}
		currObject += "\n";

		bool opened = false;
		bool isInLiteral = false;
		const char* str = buffer.c_str();
		unsigned int strSize = buffer.size();
		for (unsigned int i = 0; i < strSize; ++i)
		{
			if ('"' == str[i])
			{
				isInLiteral = !isInLiteral;
			}
			if (isInLiteral)
			{
				currObject += str[i];
				continue;
			}
			if ('#' == str[i])
			{
				break;
			}
			if ('{' == str[i])
			{
				++openBraces;
				opened = true;
			}
			else if ('}' == str[i])
			{
				--openBraces;
			}
			currObject += str[i];
		}

		if (openBraces > 0)
		{
			topLevel = false;
			continue;
		}

		if (currObject == "")
		{
			continue;
		}

		// Don't try to end an object that hasn't started properly;
		// accounts for such constructions as
		// object = 
		// { 
		// where openBraces is zero after the first line of the object. 
		// Not a problem for non-top-level objects since these will have
		// nonzero openBraces anyway.
		// But don't continue if the object was all on one line.
		if (topLevel && !opened)
		{
			continue; 
		}

		break;
	}
	return currObject;
}


bool readFile(ifstream& read)
{
	clearStack();
	read.unsetf(std::ios::skipws);

	char firstChar = read.peek();
	if (firstChar == (char)0xEF)
	{
		char bitBucket[3];
		read.read(bitBucket, 3);
	}

	/* - it turns out that the current implementation of spirit::istream_iterator is ungodly slow...
	static Parser<boost::spirit::istream_iterator> p;
	static SkipComment<boost::spirit::istream_iterator> s;

	boost::spirit::istream_iterator begin(read);
	boost::spirit::istream_iterator end;

	return qi::phrase_parse(begin, end, p, s);
	*/

	static Parser<string::iterator> p;
	static SkipComment<string::iterator> s;

	/* buffer and parse one object at a time */
	while (read.good())
	{
		string currObject = bufferOneObject(read);
		if (!qi::phrase_parse(currObject.begin(), currObject.end(), p, s))
			return false;
	}
	return true;
}


void clearStack()
{
	if (!stack.empty())
	{
		Log logOutput(LogLevel::Warning);
		logOutput << "Clearing stack size " << stack.size() << " - this should not happen in normal operation\n";
		for (vector<Object*>::iterator i = stack.begin(); i != stack.end(); ++i)
		{
			logOutput << **i << '\n';
		}
	}
	stack.clear();
}


void setLHS(string key)
{
	//cout << "Setting lefthand side " << key << endl;
	Object* p = new Object(key);
	if (0 == stack.size())
	{
		topLevel->setValue(p);
	}
	stack.push_back(p);
}

void pushObj()
{
	string key("objlist");
	Object* p = new Object(key); 
	p->setObjList(); 
	objstack.push_back(p); 
}


void setRHSleaf(string val)
{
	Object* l = stack.back();
	stack.pop_back(); 
	l->setValue(val); 
	if (0 < stack.size())
	{
		Object* p = stack.back(); 
		p->setValue(l); 
	}
}

void setRHStaglist(vector<string> val)
{
	Object* l = stack.back();
	stack.pop_back(); 
	l->addToList(val.begin(), val.end());
	if (0 < stack.size())
	{
		Object* p = stack.back(); 
		p->setValue(l); 
	}
}

void setRHSobject()
{
	// No value is set, a bunch of leaves have been defined. 
	Object* l = stack.back();
	stack.pop_back(); 
	if (0 < stack.size())
	{
		Object* p = stack.back(); 
		p->setValue(l); 
	}
}

void setRHSobjlist()
{
	Object* l = stack.back();
	l->setValue(objstack);
	objstack.clear();
	stack.pop_back(); 
	if (0 < stack.size())
	{
		Object* p = stack.back(); 
		p->setValue(l); 
	}
}


Object* doParseFile(const char* filename)
{
	ifstream	read;				// ifstream for reading files

	/* - when using parser debugging, also ensure that the parser object is non-static!
	debugme = false;
	if (string(filename) == "D:\\Victoria 2\\technologies\\commerce_tech.txt")
		debugme = true;
	*/

	initParser();
	Object* obj = getTopLevel();
	read.open(filename); 
	if (!read.is_open())
	{
		return NULL;
	}
	readFile(read);  
	read.close();
	read.clear();

	return obj;
}
