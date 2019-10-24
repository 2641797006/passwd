#include <iostream>
#include <string>
using namespace std;

int main()
{
	string s("string");

	char *p;
	p = &s[0];
	*p = 'a';

	cout<<s<<endl;
}

