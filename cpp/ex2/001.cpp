#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include "des.h"
#include "base64.h"
using namespace std;
using namespace akm;

const string oe="-e", od="-d", oeb="-eb", odb="-db";
void kerr(const char*, ...);

int main(int argc, char **argv)
{
	Des des;
	string s;

if (argc == 2 && (argv[1]==oeb || argv[1]==odb)) {

	string s1;
	Base64 base64;

	cout<< (argv[1] == oeb ? "[Encrypt]" : "[Decrypt]") <<'\n';
	cout<< "please enter a string: \n";
	getline(cin, s);

	cout<< "\nplease enter the Password: ";
	getline(cin, s1);
	des.set_key(s1);

	if (argv[1] == oeb)
		cout<< base64( des.encrypt(s) ) <<endl;
	else
		cout<< des.decrypt( base64[s] ) <<endl;
	return 1;
}

if (argc == 4 && (argv[1]==oe || argv[1]==od)) {

	fstream in(argv[2], ios::binary | ios::in);
	if ( ! in.is_open() )
		kerr("open in_file error");

	fstream out(argv[3], ios::binary | ios::out | ios::trunc);
	if ( ! out.is_open() )
		kerr("open out_file error");

	cout<< "please enter the Password: ";
	getline(cin, s);
	des.set_key(s);

	if (argv[1] == oe)
		des.encrypt(in, out);
	else
		des.decrypt(in, out);
	return 1;
}

	cerr<<"Usage: "<<argv[0]<<" -e/-d [in_file] [out_file]"<<endl;
	cerr<<"Usage: "<<argv[0]<<" -eb/-db"<<endl;
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
