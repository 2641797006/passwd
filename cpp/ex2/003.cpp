#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "des.h"
using namespace std;
using namespace akm;

int main()
{
	Des des;
	string s;

	cout<<"请输入待加密文件名: ";
	getline(cin, s);
	fstream in(s, ios::binary | ios::in);
	if ( ! in.is_open() )
		return 0;

	cout<<"请输入密钥: ";
	getline(cin, s);
	des.set_key(s);

	cout<<"请输入加密后文件名: ";
	getline(cin, s);
	fstream out(s, ios::binary |ios::in|ios::out| ios::trunc);
	if ( ! out.is_open() )
		return 0;

	fstream df("0", ios::binary | ios::out|ios::trunc);

	des.encrypt(in, out);
	des.decrypt(out, df);
}

