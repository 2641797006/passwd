#include <iostream>
#include <cstring>
#include "des.h"
using namespace std;
using namespace akm;

int main()
{
	Des::QWORD q=0;
	Des des(0x133457799BBCDFF1);

	q = des.encrypt(0x1234567890ABCDEF);
	cout<<hex<< q <<endl;
	q = des.decrypt(q);
	cout<<hex<< q <<endl;
}

