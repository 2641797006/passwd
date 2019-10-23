#include <iostream>
#include <string>
#include <vector>
#include "des.h"
using namespace std;
using namespace akm;

int main()
{
	Des des(0x12345ff68236);
	string s;

	getline(cin, s);
	cout<<"input s: "<<s<<endl;
	s = des.encrypt(s);
	cout<<"encrypt: "<<s<<endl;
	s = des.decrypt(s);
	cout<<"decrypt: "<<s<<endl;
}

