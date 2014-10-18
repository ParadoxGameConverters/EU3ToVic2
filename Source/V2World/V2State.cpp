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


#include "V2State.h"
#include "V2Pop.h"
#include "V2Province.h"
#include "V2Factory.h"
#include "../Log.h"


V2State::V2State(int newId, V2Province* firstProvince)
{
	id					= newId;
	colonial			= false;
	provinces.clear();
	provinces.push_back(firstProvince);
	factories.clear();
}


void V2State::addRailroads()
{
	for (vector<V2Province*>::iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		(*itr)->setRailLevel(1);
	}
}


void V2State::setupPops(WorldType game, string primaryCulture, set<string> acceptedCultures, string religion)
{
	int	statePopulation = getStatePopulation();
	bool	cot = hasCOT();
	for (vector<V2Province*>::iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		(*itr)->doCreatePops(game, (itr == provinces.begin()), statePopulation, cot);
	}
}


bool V2State::isCoastal() const
{
	for (vector<V2Province*>::const_iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		if ((*itr)->isCoastal())
		{
			return true;
		}
	}
	return false;
}


bool V2State::hasLocalSupply(string product) const
{
	for (vector<V2Province*>::const_iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		if ((*itr)->getRgoType() == product)
		{
			return true;
		}
	}
	return false;
}


double V2State::getSuppliedInputs(const V2Factory* factory) const
{
	// find out the total needs
	map<string, float> inputs = factory->getInputs();
	for (vector<const V2Factory*>::const_iterator itr = factories.begin(); itr != factories.end(); itr++)
	{
		map<string, float> newInputs = (*itr)->getInputs();
		for (auto jtr = newInputs.begin(); jtr != newInputs.end(); jtr++)
		{
			inputs[jtr->first] += jtr->second;
		}
	}

	// find out what we have from both RGOs and existing factories
	map<string, float> supplies;
	for (vector<V2Province*>::const_iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		string rgo = (*itr)->getRgoType();
		supplies[rgo] += 1.0;
	}
	for (vector<const V2Factory*>::const_iterator itr = factories.begin(); itr != factories.end(); itr++)
	{
		supplies[(*itr)->getOutputGoods()] += 1.0;
	}

	// determine how much of the inputs are supplied
	double totalSupplied = 0.0;
	for (map<string, float>::const_iterator inputItr = inputs.begin(); inputItr != inputs.end(); inputItr++)
	{
		map<string, float>::const_iterator supplyItr = supplies.find(inputItr->first);
		if (supplyItr != supplies.end())
 		{
			totalSupplied += supplyItr->second / inputItr->second;
 		}
 	}
	return totalSupplied;
}


bool V2State::provInState(int id) const
{
	for (vector<V2Province*>::const_iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		if ( (*itr)->getNum() == id )
		{
			return true;
		}
	}

	return false;
}


int V2State::getStatePopulation() const
{
	int population = 0;
	for (vector<V2Province*>::const_iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		population += (*itr)->getOldPopulation();
	}
	return population;
}


void V2State::addFactory(const V2Factory* factory)
{
	provinces[0]->addFactory(factory);
	factories.push_back(factory);
}


bool V2State::hasCOT()
{
	for (vector<V2Province*>::iterator itr = provinces.begin(); itr != provinces.end(); ++itr)
	{
		if ( (*itr)->getCOT() )
		{
			return true;
		}
	}

	return false;
}


bool V2State::hasLandConnection() const
{
	if (provinces.size() > 0)
	{
		return provinces[0]->hasLandConnection();
	}
	else
	{
		return false;
	}
}
