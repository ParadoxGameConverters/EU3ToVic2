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


#include "Configuration.h"
#include "Parsers/Parser.h"
#include "Log.h"

Configuration* Configuration::instance = NULL;

Configuration::Configuration()
{
	LOG(LogLevel::Info) << "Reading configuration file.";

	Object* tempObj = doParseFile("configuration.txt");
	if (tempObj == NULL)
	{
		LOG(LogLevel::Error) << "Could not parse file configuration.txt";
		exit(-1);
	}
	vector<Object*> obj = tempObj->getValue("configuration");
	if (obj.size() != 1)
	{
		LOG(LogLevel::Error) << "Configuration file must contain exactly one configuration section.";
		exit (-2);
	}

	maxLiteracy			= atof(obj[0]->getLeaf("max_literacy").c_str());
	resetProvinces		= obj[0]->getLeaf("resetProvinces");
	V2Path				= obj[0]->getLeaf("v2directory");
	V2DocumentsPath	= obj[0]->getLeaf("V2Documentsdirectory");
	EU3Path				= obj[0]->getLeaf("EU3directory");
	EU3gametype			= obj[0]->getLeaf("EU3gametype");
	V2gametype			= obj[0]->getLeaf("V2gametype");
	EU3Mod				= obj[0]->getLeaf("EU3Mod");
	removetype			= obj[0]->getLeaf("removetype");
	outputName			= "";
}