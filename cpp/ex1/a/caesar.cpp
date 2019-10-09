#include "caesar.h"

namespace akm{
using namespace std;

string
Caesar::encrypt(string const& s)
{
	string es;
	for ( auto c : s )
		es.push_back( encrypt(c) );
	return es;
}

string
Caesar::decrypt(string const& s)
{
	string ds;
	for ( auto c : s )
		ds.push_back( decrypt(c) );
	return ds;
}

char
Caesar::encrypt(int c)
{
	int pos, pos2;
	pos = ctoi(c);
	if ( pos < 0 )
		return '?';
	pos2 = (pos + K) % N;
	return lower ? ('a' + pos2) : ('A' + pos2);
}

char
Caesar::decrypt(int c)
{
	int pos, pos2;
	pos = ctoi(c);
	if ( pos < 0 )
		return '?';
	pos2 = (pos - K + N) % N;
	return lower ? ('a' + pos2) : ('A' + pos2);
}

int
Caesar::ctoi(int c)
{
	if ( islower(c) ) {
		lower = 1;
		return c - 'a';
	} else if ( isupper(c) ) {
		lower = 0;
		return c - 'A';
	} else
		return -1;
}

} // namespace akm;

