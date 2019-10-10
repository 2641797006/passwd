#include <iostream>
#include <string>
#include "transpose.h"
using namespace std;
using namespace akm;

int main()
{
	string s;
	Transpose transpose("my key"); // the key

	cout<<"Please enter a string[a-zA-Z]: ";
	getline(cin, s);
	s = transpose.encrypt(s);
	cout<<"Encrypt: "<<s<<endl;
	s = transpose.decrypt(s);
	cout<<"Decrypt: "<<s<<endl;
}

