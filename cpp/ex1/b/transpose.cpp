#include <map>
#include "transpose.h"

namespace akm{
using namespace std;

string
Transpose::decrypt(string const& s)
{
	string ds;
	if ( ! s.size() || (s.size()%key.size() != 0))
		return ds;
	int m = s.size() / key.size();
	ds.resize( s.size() );
	for (int i=0; i<m; ++i)
		for (uint32_t j=0; j<key.size(); ++j)
			ds[i*key.size()+kv[j]] = s[i*key.size()+j];
	return ds;
}

string
Transpose::encrypt(string const& _s)
{
	string es, s;
	for (auto c : _s)
		if ( isalpha(c) )
			s.push_back(c);
	if ( ! s.size() || ! key.size() )
		return es;
	int n = s.size() % key.size();
	int m = s.size() / key.size() + (n ? 1 : 0);
	if ( n )
		for (uint32_t i=n; i<key.size(); ++i)
			s.push_back('?');
	es.resize( s.size() );
	for (int i=0; i<m; ++i)
		for (uint32_t j=0; j<key.size(); ++j)
			es[i*key.size()+j] = s[i*key.size()+kv[j]];
	return es;
}

bool
Transpose::set_kv(string const& s)
{
	string s1;
	multimap<char, int> mm;

	for (auto c : s)
		if ( isalpha(c) )
			s1.push_back(c);

	if ( ! s1.size() )
		return false;
	key = s1;

	int i=0;
	for (auto c : s1)
		mm.insert({c, i++});

	vector<int> vi;
	vi.reserve( s1.size() );
	for (auto &x : mm)
		vi.push_back( x.second );

	kv.resize( s1.size() );
	i=0;
	for (auto x : vi) {
		kv[x] = i;
		++i;
	}
	return true;
}

} // namespace akm;

