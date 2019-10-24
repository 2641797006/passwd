#include <iostream>
#include <string>
#include <stdarg.h>
#include "base64.h"
using namespace std;
using namespace akm;

const string oe="-e", od="-d";
void kerr(const char*, ...);

int main(int argc, char **argv)
{
	if (argc < 2 || ((oe!=argv[1]) && (od!=argv[1]))) {
		cerr<<"Usage: "<<argv[0]<<" -e/-d"<<endl;
		return 0;
	}
	Base64 base64;
	string s;

	cout<< (argv[1] == oe ? "[Encrypt]" : "[Decrypt]") <<'\n';
	cout<< "please enter a string: \n";
	getline(cin, s);

	cout<< (argv[1] == oe ? base64(s) : base64[s]) <<endl;
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
