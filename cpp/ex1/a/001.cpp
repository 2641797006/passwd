#include <iostream>
#include <string>
#include "caesar.h"
using namespace std;
using namespace akm;

int main()
{
	string s;
	Caesar caesar;

	cout<<"Please enter a string[a-zA-Z]: ";
	getline(cin, s);
	s = caesar.encrypt(s);
	cout<<"Encrypt: "<< s <<endl;
	s = caesar.decrypt(s);
	cout<<"Decrypt: "<< s <<endl;
}

