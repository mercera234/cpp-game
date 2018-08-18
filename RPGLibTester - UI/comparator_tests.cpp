#include "comparator_tests.h"
#include "KeyComparators.h"
#include <iostream>
using namespace std;

void stringCompareTest()
{
	std::string str1 = "Apple";

	cout << str1.compare("Apple") << endl; //0
	cout << str1.compare("apple") << endl; //-1 caps comes before lower case
	cout << str1.compare("Apple ") << endl; //-1 Apple is shorter
	cout << str1.compare("Apple}") << endl; //-1

	cout << str1.compare("Bapple") << endl; //-1
	cout << str1.compare("Aapple") << endl; //1

	std::string str2 = "Aapple";
	cout << stringCompare(str1, str2) << endl;
}
