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


#include "V2World.h"
#include <Windows.h>
#include <fstream>
#include <algorithm>
#include <io.h>
#include <list>
#include <queue>
#include <cmath>
#include <cfloat>
#include <sys/stat.h>
#include "../Parsers/Parser.h"
#include "../Log.h"
#include "../Configuration.h"
#include "../WinUtils.h"
#include "../EU3World/EU3World.h"
#include "../EU3World/EU3Relations.h"
#include "../EU3World/EU3Loan.h"
#include "../EU3World/EU3Leader.h"
#include "../EU3World/EU3Province.h"
#include "../EU3World/EU3Diplomacy.h"
#include "V2Province.h"
#include "V2State.h"
#include "V2Relations.h"
#include "V2Army.h"
#include "V2Leader.h"
#include "V2Pop.h"
#include "V2Country.h"
#include "V2Reforms.h"
#include "V2Flags.h"
#include "V2LeaderTraits.h"


typedef struct fileWithCreateTime
{
	string	filename;
	time_t	createTime;
	bool operator < (const fileWithCreateTime &rhs) const
	{
		return createTime < rhs.createTime;
	};
} fileWithCreateTime;


V2World::V2World()
{
	LOG(LogLevel::Info) << "Importing provinces";

	struct _finddata_t	provinceFileData;
	intptr_t					fileListing = NULL;
	list<string>			directories;
	directories.push_back("");
	struct _stat st;
	if (_stat(".\\blankMod\\output\\history\\provinces\\", &st) == 0)
	{
		while (directories.size() > 0)
		{
			if ((fileListing = _findfirst((string(".\\blankMod\\output\\history\\provinces") + directories.front() + "\\*.*").c_str(), &provinceFileData)) == -1L)
			{
				LOG(LogLevel::Error) << "Could not open directory .\\blankMod\\output\\history\\provinces" << directories.front() << "\\*.*";
				exit(-1);
			}

			do
			{
				if (strcmp(provinceFileData.name, ".") == 0 || strcmp(provinceFileData.name, "..") == 0)
				{
					continue;
				}
				if (provinceFileData.attrib & _A_SUBDIR)
				{
					string newDirectory = directories.front() + "\\" + provinceFileData.name;
					directories.push_back(newDirectory);
				}
				else
				{
					V2Province* newProvince = new V2Province(directories.front() + "\\" + provinceFileData.name);
					provinces.insert(make_pair(newProvince->getNum(), newProvince));
				}
			} while (_findnext(fileListing, &provinceFileData) == 0);
			_findclose(fileListing);
			directories.pop_front();
		}
	}
	else
	{
		while (directories.size() > 0)
		{
			if ((fileListing = _findfirst((Configuration::getV2Path() + "\\history\\provinces" + directories.front() + "\\*.*").c_str(), &provinceFileData)) == -1L)
			{
				LOG(LogLevel::Error) << "Could not open directory " << Configuration::getV2Path() << "\\history\\provinces" << directories.front() << "\\*.*";
				exit(-1);
			}

			do
			{
				if (strcmp(provinceFileData.name, ".") == 0 || strcmp(provinceFileData.name, "..") == 0)
				{
					continue;
				}
				if (provinceFileData.attrib & _A_SUBDIR)
				{
					string newDirectory = directories.front() + "\\" + provinceFileData.name;
					directories.push_back(newDirectory);
				}
				else
				{
					V2Province* newProvince = new V2Province(directories.front() + "\\" + provinceFileData.name);
					provinces.insert(make_pair(newProvince->getNum(), newProvince));
				}
			} while (_findnext(fileListing, &provinceFileData) == 0);
			_findclose(fileListing);
			directories.pop_front();
		}
	}

	// set V2 basic population levels
	//log("\tImporting historical pops.\n");
	//printf("\tImporting historical pops.\n");
	//struct _finddata_t	popsFileData;
	//if ( (fileListing = _findfirst( (V2Loc + "\\history\\pops\\1836.1.1\\*").c_str(), &popsFileData)) == -1L)
	//{
	//	log("Could not open pops files.\n");
	//	return;
	//}
	//do
	//{
	//	if (strcmp(popsFileData.name, ".") == 0 || strcmp(popsFileData.name, "..") == 0 )
	//	{
	//		continue;
	//	}

	//	Object*	obj2;				// generic object
	//	ifstream	read;				// ifstream for reading files
	//	initParser();
	//	obj2 = getTopLevel();
	//	read.open( (V2Loc + "\\history\\pops\\1836.1.1\\" + popsFileData.name).c_str() );
	//	if (!read.is_open())
	//	{
	//		log("Error: Could not open %s\n", popsFileData.name);
	//		printf("Error: Could not open %s\n", popsFileData.name);
	//		read.close();
	//		read.clear();
	//		continue;
	//	}
	//	readFile(read);
	//	read.close();
	//	read.clear();
	//
	//	vector<Object*> leaves = obj2->getLeaves();
	//	for (unsigned int j = 0; j < leaves.size(); j++)
	//	{
	//		int provNum = atoi(leaves[j]->getKey().c_str());
	//		unsigned int k = 0;
	//		while (k < provinces.size() && provNum != provinces[k]->getNum())
	//		{
	//			k++;
	//		}
	//		if (k == provinces.size())
	//		{
	//			log("Could not find province %d for original pops.\n", provNum);
	//			continue;
	//		}
	//		else
	//		{
	//			vector<Object*> pops = leaves[j]->getLeaves();
	//			for(unsigned int l = 0; l < pops.size(); l++)
	//			{
	//				vector< pair<int, double> > issues;
	//				issues.clear();
	//				V2Pop* newPop = new V2Pop(pops[l]->getKey(), atoi(pops[l]->getLeaf("size").c_str()), pops[l]->getLeaf("culture"), pops[l]->getLeaf("religion"), 10.0, 0.0, 0.0, 0.0, issues);
	//				provinces[k]->addOldPop(newPop);
	//			}
	//		}
	//	}
	//} while(_findnext(fileListing, &popsFileData) == 0);
	//_findclose(fileListing);

	// determine whether a province is coastal or not by checking if it has a naval base
	// if it's not coastal, we won't try to put any navies in it (otherwise Vicky crashes)
	//log("\tFinding coastal provinces.\n");
	//printf("\tFinding coastal provinces.\n");
	//Object*	obj2 = doParseFile((V2Loc + "\\map\\positions.txt").c_str());
	//if (obj2 == NULL)
	//{
	//	log("Could not parse file %s\n", (V2Loc + "\\map\\positions.txt").c_str());
	//	exit(-1);
	//}
	//vector<Object*> objProv = obj2->getLeaves();
	//if (objProv.size() == 0)
	//{
	//	log("Error: map\\positions.txt failed to parse.");
	//	printf("Error: map\\positions.txt failed to parse.");
	//	exit(1);
	//}
	//for (vector<Object*>::iterator itr = objProv.begin(); itr != objProv.end(); ++itr)
	//{
	//	int provinceNum = atoi((*itr)->getKey().c_str());
	//	vector<Object*> objPos = (*itr)->getValue("building_position");
	//	if (objPos.size() == 0)
	//		continue;
	//	vector<Object*> objNavalBase = objPos[0]->getValue("naval_base");
	//	if (objNavalBase.size() != 0)
	//	{
	//		// this province is coastal
	//		for (vector<V2Province*>::iterator pitr = provinces.begin(); pitr != provinces.end(); ++pitr)
	//		{
	//			if ( (*pitr)->getNum() == provinceNum)
	//			{
	//				(*pitr)->setCoastal(true);
	//				break;
	//			}
	//		}
	//	}
	//}

	countries.clear();

	LOG(LogLevel::Info) << "Getting potential countries";
	potentialCountries.clear();
	dynamicCountries.clear();
	const date FirstStartDate = date("1836.1.1");
	ifstream V2CountriesInput;
	if (_stat(".\\blankMod\\output\\common\\countries.txt", &st) == 0)
	{
		V2CountriesInput.open(".\\blankMod\\output\\common\\countries.txt");
	}
	else
	{
		V2CountriesInput.open((Configuration::getV2Path() + "\\common\\countries.txt").c_str());
	}
	if (!V2CountriesInput.is_open())
	{
		LOG(LogLevel::Error) << "Could not open countries.txt";
		exit(1);
	}

	bool	staticSection = true;
	while (!V2CountriesInput.eof())
	{
		string line;
		getline(V2CountriesInput, line);

		if ( (line[0] == '#') || (line.size() < 3) )
		{
			continue;
		}
		else if (line.substr(0,12) == "dynamic_tags")
		{
			staticSection = false;
			continue;
		}
		
		string tag;
		tag = line.substr(0, 3);

		string countryFileName;
		int start			= line.find_first_of('/');
		int size				= line.find_last_of('\"') - start;
		countryFileName	= line.substr(start, size);

		Object* countryData;
		if (_stat((string(".\\blankMod\\output\\common\\countries\\") + countryFileName).c_str(), &st) == 0)
		{
			countryData = doParseFile((string(".\\blankMod\\output\\common\\countries\\") + countryFileName).c_str());
			if (countryData == NULL)
			{
				LOG(LogLevel::Warning) << "Could not parse file .\\blankMod\\output\\common\\countries\\" << countryFileName;
			}
		}
		else if (_stat((Configuration::getV2Path() + "\\common\\countries\\" + countryFileName).c_str(), &st) == 0)
		{
			countryData = doParseFile((Configuration::getV2Path() + "\\common\\countries\\" + countryFileName).c_str());
			if (countryData == NULL)
			{
				LOG(LogLevel::Warning) << "Could not parse file " << Configuration::getV2Path() << "\\common\\countries\\" << countryFileName;
			}
		}
		else
		{
			LOG(LogLevel::Debug) << "Could not find file common\\countries\\" << countryFileName << " - skipping";
			continue;
		}

		vector<Object*> partyData = countryData->getValue("party");
		vector<V2Party*> localParties;
		for (vector<Object*>::iterator itr = partyData.begin(); itr != partyData.end(); ++itr)
		{
			V2Party* newParty = new V2Party(*itr);
			localParties.push_back(newParty);
		}

		V2Country* newCountry = new V2Country(tag, countryFileName, localParties, this);
		if (staticSection)
		{
			potentialCountries.push_back(newCountry);
		}
		else
		{
			potentialCountries.push_back(newCountry);
			dynamicCountries.insert( make_pair(tag, newCountry) );
		}
	}
	V2CountriesInput.close();

	colonies.clear();
}


void V2World::output() const
{
	// Create common\countries path.
	string countriesPath = "Output\\" + Configuration::getOutputName() + "\\common\\countries";
	if (!WinUtils::TryCreateFolder(countriesPath))
	{
		return;
	}

	// Output common\countries.txt
	LOG(LogLevel::Debug) << "Writing countries file";
	FILE* allCountriesFile;
	if (fopen_s(&allCountriesFile, ("Output\\" + Configuration::getOutputName() + "\\common\\countries.txt").c_str(), "w") != 0)
	{
		LOG(LogLevel::Error) << "Could not create countries file";
		exit(-1);
	}
	for (map<string, V2Country*>::const_iterator i = countries.begin(); i != countries.end(); i++)
	{
		const V2Country& country = *i->second;
		map<string, V2Country*>::const_iterator j = dynamicCountries.find(country.getTag());
		if (j == dynamicCountries.end())
		{
			country.outputToCommonCountriesFile(allCountriesFile);
		}
	}
	fprintf(allCountriesFile, "\n");
	if ((Configuration::getV2Gametype() == "HOD") || (Configuration::getV2Gametype() == "HoD_NNM"))
	{
		fprintf(allCountriesFile, "##HoD Dominions\n");
		fprintf(allCountriesFile, "dynamic_tags = yes # any tags after this is considered dynamic dominions\n");
		for (map<string, V2Country*>::const_iterator i = dynamicCountries.begin(); i != dynamicCountries.end(); i++)
		{
			i->second->outputToCommonCountriesFile(allCountriesFile);
		}
	}
	fclose(allCountriesFile);

	// Create flags for all new countries.
	V2Flags flags;
	flags.SetV2Tags(countries);
	flags.Output();

	// Create localisations for all new countries. We don't actually know the names yet so we just use the tags as the names.
	LOG(LogLevel::Debug) << "Writing localisation text";
	string localisationPath = "Output\\" + Configuration::getOutputName() + "\\localisation";
	if (!WinUtils::TryCreateFolder(localisationPath))
	{
		return;
	}
	string source = Configuration::getV2Path() + "\\localisation\\text.csv";
	string dest = localisationPath + "\\text.csv";
	WinUtils::TryCopyFile(source, dest);
	FILE* localisationFile;
	if (fopen_s(&localisationFile, dest.c_str(), "a") != 0)
	{
		LOG(LogLevel::Error) << "Could not update localisation text file";
		exit(-1);
	}
	for (map<string, V2Country*>::const_iterator i = countries.begin(); i != countries.end(); i++)
	{
		const V2Country& country = *i->second;
		if (country.isNewCountry())
		{
			country.outputLocalisation(localisationFile);
		}
	}
	fclose(localisationFile);

	LOG(LogLevel::Debug) << "Writing provinces";
	for (map<int, V2Province*>::const_iterator i = provinces.begin(); i != provinces.end(); i++)
	{
		//i->second->sortPops();
		i->second->output();
	}

	LOG(LogLevel::Debug) << "Writing countries";
	for (auto itr = countries.begin(); itr != countries.end(); itr++)
	{
		itr->second->output();
	}
	for (map<string, V2Country*>::const_iterator itr = dynamicCountries.begin(); itr != dynamicCountries.end(); itr++)
	{
		itr->second->isANewCountry();
		itr->second->output();
	}
	diplomacy.output();
	/*if ((Configuration::getV2Gametype() == "HOD") || (Configuration::getV2Gametype() == "HoD-NNM"))
	{
		for (map< int, set<string> >::const_iterator colonyIter = colonies.begin(); colonyIter != colonies.end(); colonyIter++)
		{
			fprintf(output, "region=\n");
			fprintf(output, "{\n");
			fprintf(output, "	index=%d\n", colonyIter->first);
			fprintf(output, "	phase=0\n");
			fprintf(output, "	temperature=0.000\n");
			for (set<string>::iterator countriesIter = colonyIter->second.begin(); countriesIter != colonyIter->second.end(); countriesIter++)
			{
				fprintf(output, "	colony=\n");
				fprintf(output, "	{\n");
				fprintf(output, "		tag=\"%s\"\n", countriesIter->c_str());
				fprintf(output, "		points=1\n");
				fprintf(output, "		invest=80\n");
				fprintf(output, "		date=\"1836.1.1\"\n");
				fprintf(output, "	}\n");
			}
			fprintf(output, "}\n");
		}
	}*/
}


bool scoresSorter(pair<V2Country*, int> first, pair<V2Country*, int> second)
{
	return (first.second > second.second);
}


void V2World::convertCountries(const EU3World& sourceWorld, const CountryMapping& countryMap, const cultureMapping& cultureMap, const unionCulturesMap& unionCultures, const religionMapping& religionMap, const governmentMapping& governmentMap, const inverseProvinceMapping& inverseProvinceMap, const vector<techSchool>& techSchools, map<int,int>& leaderMap, const V2LeaderTraits& lt)
{
	vector<string> outputOrder;
	outputOrder.clear();
	for (unsigned int i = 0; i < potentialCountries.size(); i++)
	{
		outputOrder.push_back(potentialCountries[i]->getTag());
	}

	map<string, EU3Country*> sourceCountries = sourceWorld.getCountries();
	for (map<string, EU3Country*>::iterator i = sourceCountries.begin(); i != sourceCountries.end(); i++)
	{
		EU3Country* sourceCountry = i->second;
		std::string EU3Tag = sourceCountry->getTag();
		V2Country* destCountry = NULL;
		const std::string& V2Tag = countryMap[EU3Tag];
		if (!V2Tag.empty())
		{
			for (vector<V2Country*>::iterator j = potentialCountries.begin(); j != potentialCountries.end() && !destCountry; j++)
			{
				V2Country* candidateDestCountry = *j;
				if (candidateDestCountry->getTag() == V2Tag)
				{
					destCountry = candidateDestCountry;
				}
			}
			if (!destCountry)
			{ // No such V2 country exists yet for this tag so we make a new one.
				std::string countryFileName = '/' + sourceCountry->getName() + ".txt";
				destCountry = new V2Country(V2Tag, countryFileName, std::vector<V2Party*>(), this, true);
			}
			destCountry->initFromEU3Country(sourceCountry, outputOrder, countryMap, cultureMap, religionMap, unionCultures, governmentMap, inverseProvinceMap, techSchools, leaderMap, lt);
			countries.insert(make_pair(V2Tag, destCountry));
		}
		else
		{
			LOG(LogLevel::Warning) << "Could not convert EU3 tag " << i->second->getTag() << " to V2";
		}
	}

	// set national values
	list< pair<V2Country*, int> > libertyScores;
	list< pair<V2Country*, int> > equalityScores;
	set<V2Country*>					valuesUnset;
	for (map<string, V2Country*>::iterator countryItr = countries.begin(); countryItr != countries.end(); countryItr++)
	{
		int libertyScore = 1;
		int equalityScore = 1;
		int orderScore = 1;
		countryItr->second->getNationalValueScores(libertyScore, equalityScore, orderScore);
		if (libertyScore > orderScore)
		{
			libertyScores.push_back(make_pair(countryItr->second, libertyScore));
		}
		if ((equalityScore > orderScore) && (equalityScore > libertyScore))
		{
			equalityScores.push_back(make_pair(countryItr->second, equalityScore));
		}
		valuesUnset.insert(countryItr->second);
		LOG(LogLevel::Debug) << "Value scores for " << countryItr->first << ": order = " << orderScore << ", liberty = " << libertyScore << ", equality = " << equalityScore;
	}
	equalityScores.sort(scoresSorter);
	int equalityLeft = 5;
	for (list< pair<V2Country*, int> >::iterator equalItr = equalityScores.begin(); equalItr != equalityScores.end(); equalItr++)
	{
		if (equalityLeft < 1)
		{
			break;
		}
		set<V2Country*>::iterator unsetItr = valuesUnset.find(equalItr->first);
		if (unsetItr != valuesUnset.end())
		{
			valuesUnset.erase(unsetItr);
			equalItr->first->setNationalValue("nv_equality");
			equalityLeft--;
			LOG(LogLevel::Debug) << equalItr->first->getTag() << " got national value equality";
		}
	}
	libertyScores.sort(scoresSorter);
	int libertyLeft = 20;
	for (list< pair<V2Country*, int> >::iterator libItr = libertyScores.begin(); libItr != libertyScores.end(); libItr++)
	{
		if (libertyLeft < 1)
		{
			break;
		}
		set<V2Country*>::iterator unsetItr = valuesUnset.find(libItr->first);
		if (unsetItr != valuesUnset.end())
		{
			valuesUnset.erase(unsetItr);
			libItr->first->setNationalValue("nv_liberty");
			libertyLeft--;
			LOG(LogLevel::Debug) << libItr->first->getTag() << " got national value liberty";
		}
	}
	for (set<V2Country*>::iterator unsetItr = valuesUnset.begin(); unsetItr != valuesUnset.end(); unsetItr++)
	{
		(*unsetItr)->setNationalValue("nv_order");
		LOG(LogLevel::Debug) << (*unsetItr)->getTag() << " got national value order";
	}

	// ALL potential countries should be output to the file, otherwise some things don't get initialized right
	for (vector<V2Country*>::iterator itr = potentialCountries.begin(); itr != potentialCountries.end(); ++itr)
	{
		map<string, V2Country*>::iterator citr = countries.find((*itr)->getTag());
		if (citr == countries.end())
		{
			(*itr)->initFromHistory();
			countries.insert(make_pair((*itr)->getTag(), *itr));
		}
	}

	// put countries in the same order as potentialCountries was (this is the same order V2 will save them in)
	/*vector<V2Country*> sortedCountries;
	for (vector<string>::const_iterator oitr = outputOrder.begin(); oitr != outputOrder.end(); ++oitr)
	{
		map<string, V2Country*>::iterator itr = countries.find((*itr)->getTag());
		{
			if ( (*itr)->getTag() == (*oitr) )
			{
				sortedCountries.push_back(*itr);
				break;
			}
		}
	}
	countries.swap(sortedCountries);*/
}


void V2World::convertDiplomacy(const EU3World& sourceWorld, const CountryMapping& countryMap)
{
	vector<EU3Agreement> agreements = sourceWorld.getDiplomacy()->getAgreements();
	for (vector<EU3Agreement>::iterator itr = agreements.begin(); itr != agreements.end(); ++itr)
	{
		const std::string& EU3Tag1 = itr->country1;
		const std::string& V2Tag1 = countryMap[EU3Tag1];
		if (V2Tag1.empty())
		{
			LOG(LogLevel::Warning) << "EU3 Country " << EU3Tag1 << " used in diplomatic agreement doesn't exist";
			continue;
		}
		const std::string& EU3Tag2 = itr->country2;
		const std::string& V2Tag2 = countryMap[EU3Tag2];
		if (V2Tag2.empty())
		{
			LOG(LogLevel::Warning) << "EU3 Country " << EU3Tag2 << " used in diplomatic agreement doesn't exist";
			continue;
		}

		map<string, V2Country*>::iterator country1 = countries.find(V2Tag1);
		map<string, V2Country*>::iterator country2 = countries.find(V2Tag2);
		if (country1 == countries.end())
		{
			LOG(LogLevel::Warning) << "Vic2 country " << V2Tag1 << " used in diplomatic agreement doesn't exist";
			continue;
		}
		if (country2 == countries.end())
		{
			LOG(LogLevel::Warning) << "Vic2 country " << V2Tag2 << " used in diplomatic agreement doesn't exist";
			continue;
		}
		V2Relations* r1 = country1->second->getRelations(V2Tag2);
		if (!r1)
		{
			r1 = new V2Relations(V2Tag2);
			country1->second->addRelation(r1);
		}
		V2Relations* r2 = country2->second->getRelations(V2Tag1);
		if (!r2)
		{
			r2 = new V2Relations(V2Tag1);
			country2->second->addRelation(r2);
		}

		if ((itr->type == "royal_marriage") || (itr->type == "guarantee"))
		{
			// influence level +1, but never exceed 4
			if (r1->getLevel() < 4)
				r1->setLevel(r1->getLevel() + 1);
		}
		if (itr->type == "royal_marriage")
		{
			// royal marriage is bidirectional; influence level +1, but never exceed 4
			if (r2->getLevel() < 4)
				r2->setLevel(r2->getLevel() + 1);
		}
		if ((itr->type == "sphere") || (itr->type == "vassal") || (itr->type == "union"))
		{
			// influence level = 5
			r1->setLevel(5);
			/* FIXME: is this desirable?
			// if relations are too poor, country2 will immediately eject country1's ambassadors at the start of the game
			// so, for stability's sake, give their relations a boost
			if (r1->getRelations() < 1)
				r1->setRelations(1);
			if (r2->getRelations() < 1)
				r2->setRelations(1);
			*/
		}
		if ((itr->type == "alliance") || (itr->type == "vassal") || (itr->type == "union") || (itr->type == "guarantee"))
		{
			// copy agreement
			V2Agreement v2a;
			v2a.country1 = V2Tag1;
			v2a.country2 = V2Tag2;
			v2a.start_date = itr->startDate;
			v2a.type = itr->type;
			diplomacy.addAgreement(v2a);
		}
	}
}


struct MTo1ProvinceComp
{
	MTo1ProvinceComp() : totalPopulation(0) {};

	vector<EU3Province*> provinces;
	int totalPopulation;
};


void V2World::convertProvinces(const EU3World& sourceWorld, const provinceMapping& provinceMap, const resettableMap& resettableProvinces, const CountryMapping& countryMap, const cultureMapping& cultureMap, const religionMapping& religionMap, const stateIndexMapping& stateIndexMap)
{
	for (map<int, V2Province*>::iterator i = provinces.begin(); i != provinces.end(); i++)
	{
		int destNum												= i->first;
		provinceMapping::const_iterator provinceLink	= provinceMap.find(destNum);
		if ( (provinceLink == provinceMap.end()) || (provinceLink->second.size() == 0) )
		{
			LOG(LogLevel::Warning) << "No source for " << i->second->getName() << " (province " << destNum << ')';
			continue;
		}
		else if (provinceLink->second[0] == 0)
		{
			continue;
		}
		else if ((Configuration::getResetProvinces() == "yes") && (resettableProvinces.count(destNum) > 0))
		{
			continue;
		}

		i->second->clearCores();

		EU3Province*	oldProvince	= NULL;
		EU3Country*		oldOwner		= NULL;
		// determine ownership by province count, or total population (if province count is tied)
		map<string, MTo1ProvinceComp> provinceBins;
		double newProvinceTotalPop = 0;
		for (vector<int>::const_iterator itr = provinceLink->second.begin(); itr != provinceLink->second.end(); ++itr)
		{
			EU3Province* province = sourceWorld.getProvince(*itr);
			if (!province)
			{
				LOG(LogLevel::Warning) << "Old province " << provinceLink->second[0] << " does not exist (bad mapping?)";
				continue;
			}
			EU3Country* owner = province->getOwner();
			string tag;
			if (owner != NULL)
			{
				tag = owner->getTag();
			}
			else
			{
				tag = "";
			}
			if (provinceBins.find(tag) == provinceBins.end())
			{
				provinceBins[tag] = MTo1ProvinceComp();
			}

			if (((Configuration::getV2Gametype() == "HOD") || (Configuration::getV2Gametype() == "HoD-NNM")) && (province->getPopulation() < 1000) && (owner != NULL))
			{
				stateIndexMapping::const_iterator stateIndexMapping = stateIndexMap.find(i->first);
				if (stateIndexMapping == stateIndexMap.end())
				{
					LOG(LogLevel::Warning) << "Could not find state index for province " << i->first;
					continue;
				}
				else
				{
					map< int, set<string> >::iterator colony = colonies.find(stateIndexMapping->second);
					if (colony == colonies.end())
					{
						set<string> countries;
						countries.insert(owner->getTag());
						colonies.insert( make_pair(stateIndexMapping->second, countries) );
					}
					else
					{
						colony->second.insert(owner->getTag());
					}
				}
			}
			else
			{
				provinceBins[tag].provinces.push_back(province);
				provinceBins[tag].totalPopulation += province->getPopulation();
				newProvinceTotalPop += province->getPopulation();
				// I am the new owner if there is no current owner, or I have more provinces than the current owner,
				// or I have the same number of provinces, but more population, than the current owner
				if (	(oldOwner == NULL) ||
						(provinceBins[tag].provinces.size() > provinceBins[oldOwner->getTag()].provinces.size()) ||
						(	(provinceBins[tag].provinces.size() == provinceBins[oldOwner->getTag()].provinces.size()) &&
							(provinceBins[tag].totalPopulation > provinceBins[oldOwner->getTag()].totalPopulation)
						)
					)
				{
					oldOwner = owner;
					oldProvince = province;
				}
			}
		}
		if (oldOwner == NULL)
		{
			i->second->setOwner("");
			continue;
		}

		const std::string& V2Tag = countryMap[oldOwner->getTag()];
		if (V2Tag.empty())
		{
			LOG(LogLevel::Warning) << "Could not map provinces owned by " << oldOwner->getTag();
		}
		else
		{
			i->second->setOwner(V2Tag);
			map<string, V2Country*>::iterator ownerItr = countries.find(V2Tag);
			if (ownerItr != countries.end())
			{
				ownerItr->second->addProvince(i->second);
			}
			i->second->convertFromOldProvince(oldProvince);

			for (map<string, MTo1ProvinceComp>::iterator mitr = provinceBins.begin(); mitr != provinceBins.end(); ++mitr)
			{
				for (vector<EU3Province*>::iterator vitr = mitr->second.provinces.begin(); vitr != mitr->second.provinces.end(); ++vitr)
				{
					// assign cores
					vector<EU3Country*> oldCores = (*vitr)->getCores(sourceWorld.getCountries());
					for(vector<EU3Country*>::iterator j = oldCores.begin(); j != oldCores.end(); j++)
					{
						std::string coreEU3Tag = (*j)->getTag();
						// skip this core if the country is the owner of the EU3 province but not the V2 province
						// (i.e. "avoid boundary conflicts that didn't exist in EU3").
						// this country may still get core via a province that DID belong to the current V2 owner
						if (( coreEU3Tag == mitr->first) && ( coreEU3Tag != oldOwner->getTag()))
						{
							continue;
						}

						const std::string& coreV2Tag = countryMap[coreEU3Tag];
						if (!coreV2Tag.empty())
						{
							i->second->addCore(coreV2Tag);
						}
					}

					// determine demographics
					double provPopRatio = (*vitr)->getPopulation() / newProvinceTotalPop;
					vector<EU3PopRatio> popRatios = (*vitr)->getPopRatios();
					for (vector<EU3PopRatio>::iterator prItr = popRatios.begin(); prItr != popRatios.end(); ++prItr)
					{
						bool matched = false;
						string culture = "";
						for (cultureMapping::const_iterator cultureItr = cultureMap.begin(); (cultureItr != cultureMap.end()) && (!matched); cultureItr++)
						{
							if (cultureItr->srcCulture == prItr->culture)
							{
								bool match = true;
								for (vector<distinguisher>::const_iterator distiguisherItr = cultureItr->distinguishers.begin(); distiguisherItr != cultureItr->distinguishers.end(); distiguisherItr++)
								{
									if (distiguisherItr->first == DTOwner)
									{
										if ((*vitr)->getOwner()->getTag() != distiguisherItr->second)
											match = false;
									}
									else if (distiguisherItr->first == DTReligion)
									{
										if (prItr->religion != distiguisherItr->second)
											match = false;
									}
									else
									{
										LOG(LogLevel::Warning) << "Unhandled distinguisher type in culture rules";
									}

								}
								if (match)
								{
									culture = cultureItr->dstCulture;
									matched = true;
								}
							}
						}
						if (!matched)
						{
							LOG(LogLevel::Warning) << "Could not set culture for pops in province " << destNum;
						}

						string religion = "";
						religionMapping::const_iterator religionItr = religionMap.find(prItr->religion);
						if (religionItr != religionMap.end())
						{
							religion = religionItr->second;
						}
						else
						{
							LOG(LogLevel::Warning) << "Could not set religion for pops in province " << destNum;
						}

						V2Demographic demographic;
						demographic.culture		= culture;
						demographic.religion		= religion;
						demographic.ratio			= prItr->popRatio * provPopRatio;
						demographic.oldCountry	= oldOwner;
						demographic.oldProvince	= *vitr;
								
						i->second->addPopDemographic(demographic);
					}

					// set forts and naval bases
					if ((*vitr)->hasBuilding("fort4") || (*vitr)->hasBuilding("fort5") || (*vitr)->hasBuilding("fort6"))
					{
						i->second->setFortLevel(1);
					}
					// note: HTTT has only shipyard
					if (   (*vitr)->hasBuilding("shipyard") || (*vitr)->hasBuilding("grand_shipyard")
						|| (*vitr)->hasBuilding("naval_arsenal") || (*vitr)->hasBuilding("naval_base"))
					{
						// place naval bases only in port provinces
						vector<int> candidates;
						candidates.push_back(i->second->getNum());
						candidates = getPortProvinces(candidates);
						if (candidates.size() > 0)
						{
							i->second->setNavalBaseLevel(1);
						}
					}
				}
			}
		}
	}
}


void V2World::setupColonies(const adjacencyMapping& adjacencyMap, const continentMapping& continentMap)
{
	for (map<string, V2Country*>::iterator countryItr = countries.begin(); countryItr != countries.end(); countryItr++)
	{
		// find all land connections to capitals
		map<int, V2Province*>	openProvinces = provinces;
		queue<int>					goodProvinces;

		map<int, V2Province*>::iterator openItr = openProvinces.find(countryItr->second->getCapital());
		if (openItr == openProvinces.end())
		{
			continue;
		}
		openItr->second->setLandConnection(true);
		goodProvinces.push(openItr->first);
		openProvinces.erase(openItr);
		
		do
		{
			int currentProvince = goodProvinces.front();
			goodProvinces.pop();
			if (currentProvince > static_cast<int>(adjacencyMap.size()))
			{
				LOG(LogLevel::Warning) << "No adjacency mapping for province " << currentProvince;
				continue;
			}
			vector<int> adjacencies = adjacencyMap[currentProvince];
			for (unsigned int i = 0; i < adjacencies.size(); i++)
			{
				map<int, V2Province*>::iterator openItr = openProvinces.find(adjacencies[i]);
				if (openItr == openProvinces.end())
				{
					continue;
				}
				if (openItr->second->getOwner() != countryItr->first)
				{
					continue;
				}
				openItr->second->setLandConnection(true);
				goodProvinces.push(openItr->first);
				openProvinces.erase(openItr);
			}
		} while (goodProvinces.size() > 0);

		// find all provinces on the same continent as the owner's capital
		string capitalContinent = "";
		map<int, V2Province*>::iterator capital = provinces.find(countryItr->second->getCapital());
		if (capital != provinces.end())
		{
			continentMapping::const_iterator itr = continentMap.find(capital->first);
			if (itr != continentMap.end())
			{
				capitalContinent = itr->second;
			}
			else
			{
				continue;
			}
		}
		else
		{
			continue;
		}
		vector<V2Province*> ownedProvinces = countryItr->second->getProvinces();
		for (vector<V2Province*>::iterator provItr = ownedProvinces.begin(); provItr != ownedProvinces.end(); provItr++)
		{
			continentMapping::const_iterator itr = continentMap.find((*provItr)->getNum());
			if ((itr != continentMap.end()) && (itr->second == capitalContinent))
			{
				(*provItr)->setSameContinent(true);
			}
		}
	}

	for (map<int, V2Province*>::iterator provItr = provinces.begin(); provItr != provinces.end(); provItr++)
	{
		provItr->second->determineColonial();
	}
}


static int stateId = 0;
void V2World::setupStates(const stateMapping& stateMap)
{
	list<V2Province*> unassignedProvs;
	for (map<int, V2Province*>::iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		unassignedProvs.push_back(itr->second);
	}

	list<V2Province*>::iterator iter;
	while(unassignedProvs.size() > 0)
	{
		iter = unassignedProvs.begin();
		int		provId	= (*iter)->getNum();
		string	owner		= (*iter)->getOwner();

		if (owner == "")
		{
			unassignedProvs.erase(iter);
			continue;
		}

		V2State* newState = new V2State(stateId, *iter);
		stateId++;
		stateMapping::const_iterator stateItr = stateMap.find(provId);
		vector<int> neighbors;
		if (stateItr != stateMap.end())
		{
			neighbors = stateItr->second;
		}
		bool colonised = (*iter)->wasColonised();
		newState->setColonised(colonised);
		iter = unassignedProvs.erase(iter);

		for (vector<int>::iterator i = neighbors.begin(); i != neighbors.end(); i++)
		{
			for(iter = unassignedProvs.begin(); iter != unassignedProvs.end(); iter++)
			{
				if ((*iter)->getNum() == *i)
				{
					if ((*iter)->getOwner() == owner)
					{
						if ((*iter)->wasColonised() == colonised)
						{
							newState->addProvince(*iter);
							iter = unassignedProvs.erase(iter);
						}
					}
				}
			}
		}

		map<string, V2Country*>::iterator iter2 = countries.find(owner);
		if (iter2 != countries.end())
		{
			iter2->second->addState(newState);
		}
	}
}


void V2World::convertUncivReforms()
{
	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->convertUncivReforms();
	}
}



void V2World::setupPops(EU3World& sourceWorld)
{
	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->setupPops(sourceWorld);
	}
}


void V2World::addUnions(const unionMapping& unionMap)
{
	for (map<int, V2Province*>::iterator provItr = provinces.begin(); provItr != provinces.end(); provItr++)
	{
		for (unionMapping::const_iterator unionItr = unionMap.begin(); unionItr != unionMap.end(); unionItr++)
		{
			if (provItr->second->hasCulture(unionItr->first, 0.5) && !provItr->second->wasInfidelConquest() && !provItr->second->wasColonised())
			{
				provItr->second->addCore(unionItr->second);
			}
		}
	}
}


//#define TEST_V2_PROVINCES
void V2World::convertArmies(const EU3World& sourceWorld, const inverseProvinceMapping& inverseProvinceMap, const map<int,int>& leaderIDMap)
{
	// hack for naval bases.  not ALL naval bases are in port provinces, and if you spawn a navy at a naval base in
	// a non-port province, Vicky crashes....
	vector<int> port_whitelist;
	{
		int temp = 0;
		ifstream s("port_whitelist.txt");
		while (s.good() && !s.eof())
		{
			s >> temp;
			port_whitelist.push_back(temp);
		}
		s.close();
	}

	// get cost per regiment values
	double cost_per_regiment[num_reg_categories] = { 0.0 };
	Object*	obj2 = doParseFile("regiment_costs.txt");
	if (obj2 == NULL)
	{
		LOG(LogLevel::Error) << "Could not parse file regiment_costs.txt";
		exit(-1);
	}
	vector<Object*> objTop = obj2->getLeaves();
	if (objTop.size() == 0 || objTop[0]->getLeaves().size() == 0)
	{
		LOG(LogLevel::Error) << "regment_costs.txt failed to parse";
		exit(1);
	}
	for (int i = 0; i < num_reg_categories; ++i)
	{
		string regiment_cost = objTop[0]->getLeaf(RegimentCategoryNames[i]);
		cost_per_regiment[i] = atoi(regiment_cost.c_str());
	}

	// convert armies
	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		itr->second->convertArmies(leaderIDMap, cost_per_regiment, inverseProvinceMap, provinces, port_whitelist);
	}
}


void V2World::convertTechs(const EU3World& sourceWorld)
{
	map<string, EU3Country*> sourceCountries = sourceWorld.getCountries();
	
	double oldLandMean;
	double landMean;
	double highestLand;

	double oldNavalMean;
	double navalMean;
	double highestNaval;

	double oldTradeMean;
	double tradeMean;
	double highestTrade;

	double oldProductionMean;
	double productionMean;
	double highestProduction;

	double oldGovernmentMean;
	double governmentMean;
	double highestGovernment;

	int num = 2;
	map<string, EU3Country*>::iterator i = sourceCountries.begin();
	if (sourceCountries.size() == 0)
	{
		return;
	}
	while (i->second->getProvinces().size() == 0)
	{
		i++;
	}
	highestLand			= oldLandMean			= landMean			= i->second->getLandTech();
	highestNaval		= oldNavalMean			= navalMean			= i->second->getNavalTech();
	highestTrade		= oldTradeMean			= tradeMean			= i->second->getTradeTech();
	highestProduction	= oldProductionMean	= productionMean	= i->second->getProductionTech();
	highestGovernment	= oldGovernmentMean	= governmentMean	= i->second->getGovernmentTech();

	for (i++; i != sourceCountries.end(); i++)
	{
		if (i->second->getProvinces().size() == 0)
		{
			continue;
		}
		double newTech	= i->second->getLandTech();
		landMean			= oldLandMean + ((newTech - oldLandMean) / num);
		oldLandMean		= landMean; 
		if (newTech > highestLand)
		{
			highestLand = newTech;
		}

		newTech			= i->second->getNavalTech();
		navalMean		= oldNavalMean + ((newTech - oldNavalMean) / num);
		oldNavalMean	= navalMean; 
		if (newTech > highestNaval)
		{
			highestNaval = newTech;
		}

		newTech			= i->second->getTradeTech();
		tradeMean		= oldTradeMean + ((newTech - oldTradeMean) / num);
		oldTradeMean	= tradeMean; 
		if (newTech > highestTrade)
		{
			highestTrade = newTech;
		}

		newTech				= i->second->getProductionTech();
		productionMean		= oldProductionMean + ((newTech - oldProductionMean) / num);
		oldProductionMean	= productionMean; 
		if (newTech > highestProduction)
		{
			highestProduction = newTech;
		}

		newTech				= i->second->getGovernmentTech();
		governmentMean		= oldGovernmentMean + ((newTech - oldGovernmentMean) / num);
		oldGovernmentMean	= governmentMean; 
		if (newTech > highestGovernment)
		{
			highestGovernment = newTech;
		}

		num++;
	}

	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); itr++)
	{
		if ((Configuration::getV2Gametype() == "vanilla") || itr->second->isCivilized())
		{
			itr->second->setArmyTech(landMean, highestLand);
			itr->second->setNavyTech(navalMean, highestNaval);
			itr->second->setCommerceTech(tradeMean, highestTrade);
			itr->second->setIndustryTech(productionMean, highestProduction);
			itr->second->setCultureTech(governmentMean, highestGovernment);
		}
	}

	int numRomanticLit = 0;
	int numRomanticArt = 0;
	int numRomanticMusic = 0;
	if ((Configuration::getV2Gametype() == "vanilla") || (Configuration::getV2Gametype() == "AHD"))
	{
		for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); itr++)
		{
			if (itr->second->getInventionState(VANILLA_romanticist_literature) == active)
			{
				numRomanticLit++;
			}
			if (itr->second->getInventionState(VANILLA_romanticist_literature) == active)
			{
				numRomanticArt++;
			}
			if (itr->second->getInventionState(VANILLA_romanticist_literature) == active)
			{
				numRomanticMusic++;
			}
		}
	}
	else if (Configuration::getV2Gametype() == "HOD")
	{
		for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); itr++)
		{
			if (itr->second->getInventionState(HOD_romanticist_literature) == active)
			{
				numRomanticLit++;
			}
			if (itr->second->getInventionState(HOD_romanticist_literature) == active)
			{
				numRomanticArt++;
			}
			if (itr->second->getInventionState(HOD_romanticist_literature) == active)
			{
				numRomanticMusic++;
			}
		}
	}
	else if (Configuration::getV2Gametype() == "HoD-NNM")
	{
		for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); itr++)
		{
			if (itr->second->getInventionState(HOD_NNM_romanticist_literature) == active)
			{
				numRomanticLit++;
			}
			if (itr->second->getInventionState(HOD_NNM_romanticist_literature) == active)
			{
				numRomanticArt++;
			}
			if (itr->second->getInventionState(HOD_NNM_romanticist_literature) == active)
			{
				numRomanticMusic++;
			}
		}
	}

	double romanticLitPrestige = 0;
	for (int i = 1; i <= numRomanticLit; i++)
	{
		romanticLitPrestige += 1.0/i;
	}
	romanticLitPrestige *= 20;
	romanticLitPrestige /= numRomanticLit;

	double romanticArtPrestige = 0;
	for (int i = 1; i <= numRomanticArt; i++)
	{
		romanticArtPrestige += 1.0/i;
	}
	romanticArtPrestige *= 20;
	romanticArtPrestige /= numRomanticArt;

	double romanticMusicPrestige = 0;
	for (int i = 1; i <= numRomanticMusic; i++)
	{
		romanticMusicPrestige += 1.0/i;
	}
	romanticMusicPrestige *= 20;
	romanticMusicPrestige /= numRomanticMusic;

	if ((Configuration::getV2Gametype() == "vanilla") || (Configuration::getV2Gametype() == "AHD"))
	{
		for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
		{
			if (itr->second->getInventionState(VANILLA_romanticist_literature) == active)
			{
				itr->second->addPrestige(romanticLitPrestige);
			}
			if (itr->second->getInventionState(VANILLA_romanticist_art) == active)
			{
				itr->second->addPrestige(romanticArtPrestige);
			}
			if (itr->second->getInventionState(VANILLA_romanticist_music) == active)
			{
				itr->second->addPrestige(romanticMusicPrestige);
			}
		}
	}
	else if (Configuration::getV2Gametype() == "HOD")
	{
		for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
		{
			if (itr->second->getInventionState(HOD_romanticist_literature) == active)
			{
				itr->second->addPrestige(romanticLitPrestige);
			}
			if (itr->second->getInventionState(HOD_romanticist_art) == active)
			{
				itr->second->addPrestige(romanticArtPrestige);
			}
			if (itr->second->getInventionState(HOD_romanticist_music) == active)
			{
				itr->second->addPrestige(romanticMusicPrestige);
			}
		}
	}
	else if (Configuration::getV2Gametype() == "HoD-NNM")
	{
		for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
		{
			if (itr->second->getInventionState(HOD_NNM_romanticist_literature) == active)
			{
				itr->second->addPrestige(romanticLitPrestige);
			}
			if (itr->second->getInventionState(HOD_NNM_romanticist_art) == active)
			{
				itr->second->addPrestige(romanticArtPrestige);
			}
			if (itr->second->getInventionState(HOD_NNM_romanticist_music) == active)
			{
				itr->second->addPrestige(romanticMusicPrestige);
			}
		}
	}
}


void V2World::allocateFactories(const EU3World& sourceWorld, const V2FactoryFactory& factoryBuilder)
{
	// determine average production tech
	map<string, EU3Country*> sourceCountries = sourceWorld.getCountries();
	double productionMean = 0.0f;
	int num = 1;
	for (map<string, EU3Country*>::iterator itr = sourceCountries.begin(); itr != sourceCountries.end(); ++itr)
	{
		if ( (itr)->second->getProvinces().size() == 0)
		{
			continue;
		}
		if (( (itr)->second->getTechGroup() != "western" ) && ( (itr)->second->getTechGroup() != "latin" ))
		{
			continue;
		}

		double prodTech = (itr)->second->getProductionTech();
		productionMean += ((prodTech - productionMean) / num);
		++num;
	}

	// give all extant civilized nations an industrial score
	deque<pair<double, V2Country*>> weightedCountries;
	for (map<string, V2Country*>::iterator itr = countries.begin(); itr != countries.end(); ++itr)
	{
		if ( !itr->second->isCivilized() )
		{
			continue;
		}

		const EU3Country* sourceCountry = itr->second->getSourceCountry();
		if (sourceCountry == NULL)
		{
			continue;
		}

		if (sourceCountry->getProvinces().size() == 0)
		{
			continue;
		}

		// modified manufactory weight follows diminishing returns curve y = x^(3/4)+log((x^2)/5+1)
		int manuCount = sourceCountry->getManufactoryCount();
		double manuWeight = pow(manuCount, 0.75) + log((manuCount * manuCount) / 5.0 + 1.0);
		double industryWeight = (sourceCountry->getProductionTech() - productionMean) + manuWeight;
		// having one manufactory and average tech is not enough; you must have more than one, or above-average tech
		if (industryWeight > 1.0)
		{
			weightedCountries.push_back(pair<double, V2Country*>(industryWeight, itr->second));
		}
	}
	if (weightedCountries.size() < 1)
	{
		LOG(LogLevel::Warning) << "No countries are able to accept factories";
		return;
	}
	sort(weightedCountries.begin(), weightedCountries.end());

	// allow a maximum of 10 (plus any tied at tenth place) countries to recieve factories
	deque<pair<double, V2Country*>> restrictCountries;
	double threshold = 1.0;
	double totalIndWeight = 0.0;
	for (deque<pair<double, V2Country*>>::reverse_iterator itr = weightedCountries.rbegin();
		itr != weightedCountries.rend(); ++itr)
	{
		if ((restrictCountries.size() > 10) && (itr->first < (threshold - FLT_EPSILON)))
		{
			break;
		}
		restrictCountries.push_front(*itr); // preserve sort
		totalIndWeight += itr->first;
		threshold = itr->first;
	}
	weightedCountries.swap(restrictCountries);

	// determine how many factories each eligible nation gets
	deque<V2Factory*> factoryList = factoryBuilder.buildFactories();
	vector<pair<int, V2Country*>> factoryCounts;
	for (deque<pair<double, V2Country*>>::iterator itr = weightedCountries.begin(); itr != weightedCountries.end(); ++itr)
	{
		int factories = int(((itr->first / totalIndWeight) * factoryList.size()) + 0.5 /*round*/);
		LOG(LogLevel::Debug) << itr->second->getTag() << " has industrial weight " << itr->first << " granting max " << factories << " factories";
		factoryCounts.push_back(pair<int, V2Country*>(factories, itr->second));
	}

	// allocate the factories
	vector<pair<int, V2Country*>>::iterator lastReceptiveCountry = factoryCounts.begin();
	vector<pair<int, V2Country*>>::iterator citr = factoryCounts.begin();
	while (factoryList.size() > 0)
	{
		bool accepted = false;
		if (citr->first > 0) // can take more factories
		{
			for (deque<V2Factory*>::iterator qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
			{
				if ( citr->second->addFactory(*qitr) )
				{
					--(citr->first);
					lastReceptiveCountry = citr;
					accepted = true;
					factoryList.erase(qitr);
					break;
				}
			}
		}
		if (!accepted && citr == lastReceptiveCountry)
		{
			Log logOutput(LogLevel::Debug);
			logOutput << "No countries will accept any of the remaining factories:\n";
			for (deque<V2Factory*>::iterator qitr = factoryList.begin(); qitr != factoryList.end(); ++qitr)
			{
				logOutput << "\t  " << (*qitr)->getTypeName() << '\n';
			}
			break;
		}
		if (++citr == factoryCounts.end())
		{
			citr = factoryCounts.begin(); // loop around to beginning
		}
	}
}


map<string, V2Country*> V2World::getPotentialCountries() const
{
	map<string, V2Country*> retVal;
	for (vector<V2Country*>::const_iterator i = potentialCountries.begin(); i != potentialCountries.end(); i++)
	{
		retVal[ (*i)->getTag() ] = *i;
	}

	return retVal;
}


map<string, V2Country*> V2World::getDynamicCountries() const
{
	map<string, V2Country*> retVal = dynamicCountries;
	return retVal;
}


void V2World::getProvinceLocalizations(string file)
{
	ifstream read;
	string line;
	read.open( file.c_str() );
	while (read.good() && !read.eof())
	{
		getline(read, line);
		if (line.substr(0,4) == "PROV" && isdigit(line.c_str()[4]))
		{
			int position = line.find_first_of(';');
			int num = atoi( line.substr(4, position - 4).c_str() );
			string name = line.substr(position + 1, line.find_first_of(';', position + 1) - position - 1);
			for (unsigned int i = 0; i < provinces.size(); i++)
			{
				if (provinces[i]->getNum() == num)
				{
					provinces[i]->setName(name);
					break;
				}
			}
		}
	}
	read.close();
}


vector<int> V2World::getPortProvinces(vector<int> locationCandidates)
{
	// hack for naval bases.  not ALL naval bases are in port provinces, and if you spawn a navy at a naval base in
	// a non-port province, Vicky crashes....
	static vector<int> port_blacklist;
	if (port_blacklist.size() == 0)
	{
		int temp = 0;
		ifstream s("port_blacklist.txt");
		while (s.good() && !s.eof())
		{
			s >> temp;
			port_blacklist.push_back(temp);
		}
		s.close();
	}

	for (vector<int>::iterator litr = locationCandidates.begin(); litr != locationCandidates.end(); ++litr)
	{
		vector<int>::iterator black = std::find(port_blacklist.begin(), port_blacklist.end(), *litr);
		if (black != port_blacklist.end())
		{
			locationCandidates.erase(litr);
			break;
		}
	}
	for (vector<int>::iterator litr = locationCandidates.begin(); litr != locationCandidates.end(); ++litr)
	{
		map<int, V2Province*>::iterator pitr = provinces.find(*litr);
		if (pitr != provinces.end())
		{
			if (!pitr->second->isCoastal())
			{
				locationCandidates.erase(litr);
				--pitr;
				break;
			}
		}
	}
	return locationCandidates;
}


V2Country* V2World::getCountry(string tag)
{
	map<string, V2Country*>::iterator itr = countries.find(tag);
	if (itr != countries.end())
	{
		return itr->second;
	}
	else
	{
		return NULL;
	}
}
