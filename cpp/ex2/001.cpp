#include <iostream>
#include <cstring>
#include "des.h"
using namespace std;
using namespace akm;

int main()
{
	Des des(0x133457799BBCDFF1);

	cout<<hex<<(des.encrypt(0x0123456789ABCDEF))<<endl;
}

