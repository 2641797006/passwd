#ifndef _TRANSPOSE_H_
#define _TRANSPOSE_H_

#ifndef _GLIBCXX_STRING
#include <string>
#endif

#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif

namespace akm{
using namespace std;

class Transpose{
	string key;
	vector<int> kv;
	bool set_kv(string const&);

  public:
	Transpose(string const& key) { set_kv(key); }

	string encrypt(string const& s);
	string decrypt(string const& s);

}; // class Transpose;

} // namespace akm;

#endif
