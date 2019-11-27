#include <iostream>
#include <fstream>
#include <string>
#include <stdarg.h>
#include <map>
#include "des.h"
#include "base64.h"
using namespace std;
using namespace akm;

#define _e	1
#define _d	2
#define _in	3
#define _out	4
#define _stdio	5
#define _help	6

typedef struct {
	string arg;
	int val;
}Arg;

Arg arg_list[] = {
{"-e", _e},
{"-d", _d},
{"-in",	_in},
{"-out", _out},
{"-stdio", _stdio},
{"-h", _help},
{"-help", _help},
{"", -1}
};

string help(string name);
void kerr(const char*, ...);

int main(int argc, char **argv)
{
	map<string, int> arg_map;
	for (int i=0; ; ++i) {
		if ( ! arg_list[i].arg.size() )
			break;
		arg_map.insert({arg_list[i].arg, arg_list[i].val});
	}

	int mode=_e;
	bool is_stdio = false;
	string in, out;

	int optind=0, val;
	while (++optind < argc) {
		auto it = arg_map.find(argv[optind]);
		if (it == arg_map.end())
			val = -1;
		else
			val = it->second;
		switch (val) {
		case _e: case _d:
			mode = val;
			break;
		case _in:
			if (++optind < argc)
				in = argv[optind];
			break;
		case _out:
			if (++optind < argc)
				out = argv[optind];
			break;
		case _stdio:
			is_stdio = true;
			break;
		case _help:
			cout<<help(argv[0]);
			return 0;
		default:
			cerr<<"unrecognized command line option "<<argv[optind]<<endl;
			cerr<<"use -h/-help for a list of possible options"<<endl;
			return 1;
		}
	}

	if ( ! in.size() && ! out.size() )
		is_stdio = true;

	Des des;
	string s;

if (is_stdio) {

	string s1;
	Base64 base64;

	cout<< (mode == _e ? "[Encrypt]" : "[Decrypt]") <<'\n';
	cout<< "please enter a string: " <<endl;
	getline(cin, s);

	cout<< "\nplease enter the Password: ";
	getline(cin, s1);
	des.set_key(s1);

	if (mode == _e)
		cout<< base64( des.encrypt(s) ) <<endl;
	else
		cout<< des.decrypt( base64[s] ) <<endl;

} else {

	if ( ! in.size() ) {
		cerr<< "Missing input file" <<endl;
		return 1;
	}
	if ( ! out.size() ) {
		cerr<< "Missing Output file" <<endl;
		return 1;
	}

	fstream in_file(in, ios::binary | ios::in);
	if ( ! in_file.is_open() )
		kerr("open in_file error");

	fstream out_file(out, ios::binary | ios::out | ios::trunc);
	if ( ! out_file.is_open() )
		kerr("open out_file error");

	cout<< "please enter the Password: ";
	getline(cin, s);
	des.set_key(s);

	if (mode == _e) {
		des.encrypt(in_file, out_file);
		cout<<"Encrypt OK"<<endl;
	} else {
		des.decrypt(in_file, out_file);
		cout<<"Decrypt OK"<<endl;
	}
}

	return 0;
}

string
help(string name)
{
	string s = "Usage:    ";
		s += name;
		s += " [options] [-in IN_FILE -out OUT_FILE]\n"
		"Valid options are:\n"
		" -e                Encrypt\n"
		" -d                Decrypt\n"
		" -in in_file       Input file\n"
		" -out out_file     Output file\n"
		" -stdio            Standard input and output\n"
		" -h/-help          Display this message\n";
	return s;
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
