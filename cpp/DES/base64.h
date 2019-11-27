#ifndef _BASE64_H_
#define _BASE64_H_

#ifndef _GLIBCXX_STRING
#include <string>
#endif

namespace akm {
using namespace std;

class Base64{
  public:
	using BYTE = uint8_t;
	using DWORD = uint32_t;

  private:
	DWORD fill;
	DWORD *b64;
	char *c256;

  public:
	Base64(char c62='+', char c63='/', char f='=');
	~Base64(){ delete[] b64; delete[] c256; }
	string operator() (string const&);
	string operator[] (string const&);
	string encode(string const& s) { return (*this)(s); }
	string decode(string const& s) { return (*this)[s]; }
};

} // namespace akm

#endif // _BASE64_H_
