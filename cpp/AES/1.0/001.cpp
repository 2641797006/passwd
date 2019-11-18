#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include "aes.h"
using namespace std;
using namespace akm;

const string oe="-e", od="-d";
void kerr(const char*, ...);

int main(int argc, char **argv)
{
	Aes aes;
	string s;

if (argc == 4 && (argv[1]==oe || argv[1]==od)) {

	fstream in(argv[2], ios::binary | ios::in);
	if ( ! in.is_open() )
		kerr("open in_file error");

	fstream out(argv[3], ios::binary | ios::out | ios::trunc);
	if ( ! out.is_open() )
		kerr("open out_file error");

	cout<< "please enter the Password: ";
	getline(cin, s);
	aes.set_key(s);

	if (argv[1] == oe)
		aes.encrypt(in, out);
	else
		aes.decrypt(in, out);
	return 1;
}

	cerr<<"Usage: "<<argv[0]<<" -e/-d [in_file] [out_file]"<<endl;
	return 0;
}

void
kerr(const char *fmt, ...)
{
#ifndef KERR_NOT_PRINT
	va_list ap;
	va_start(ap, fmt);
	fprintf(stderr, "\nkerr> ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
#endif

#ifndef KERR_NOT_EXIT
	exit(24);
#endif
}
