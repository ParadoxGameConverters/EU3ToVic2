#ifndef V2_TECH_SCHOOLS
#define V2_TECH_SCHOOLS



#include <string>
#include <vector>
using namespace std;

class Object;



typedef struct V2TechSchool
{
	string name;
	double armyInvestment;
	double commerceInvestment;
	double cultureInvestment;
	double industryInvestment;
	double navyInvestment;
} techSchool;


vector<V2TechSchool>	initTechSchools(Object* obj, vector<string> blockedTechSchools);
vector<string>			initBlockedTechSchools(Object*);



#endif // V2_TECH_SCHOOLS