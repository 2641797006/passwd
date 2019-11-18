#include <iostream>
#include "aes.h"
using namespace std;
using namespace akm;

int main()
{
	Aes aes;
	Aes::DWORD
	key[4]={0x16157e2b, 0xa6d2ae28, 0x8815f7ab, 0x3c4fcf09},
	da[4] ={0xe0318832, 0x37315a43, 0x079830f6, 0x34a28da8};
//	da[4] ={0xa8f64332, 0x8d305a88, 0xa2983131, 0x340737e0};

	aes.print_da(da, "s");

	aes.set_key(key);
	aes.encrypt(da);

	aes.print_da(key, "key");
	aes.print_da(da, "es");
}

