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
	vector<char> vc;

	getline(cin, s);
	vc.assign(s.begin(), s.end());
	cout<<"input: "<<vc.data()<<endl;
	vc = des.encrypt(vc);
	cout<<"encrypt: "<<vc.data()<<endl;
	vc = des.decrypt(vc);
	cout<<"decrypt: "<<vc.data()<<endl;
}

