#include <iostream>
#include <string>
#include "base64.h"
using namespace std;
using namespace akm;

int main()
{
	Base64 base64;
	string s;

	getline(cin, s);

	cout<< base64(s) <<endl;

}

