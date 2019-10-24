#ifndef _BASE64_H_
#define _BASE64_H_

#ifndef _GLIBCXX_STRING
#include <string>
#endif

namespace akm {
using namespace std;

class Base64{
  public:
	using DWORD = uint32_t;

  private:
	static DWORD b64[64];
	static char c256[256];

  public:
	Base64(){}
	~Base64(){}
	string operator() (string const&);
	string operator[] (string const&);
	string encode(string const& s) { return (*this)(s); }
};

string
Base64::operator() (string const& s)
{
	string es;
	if ( ! s.size() )
		return es;
	int n = s.size()/3;
	int m = s.size()%3;
	DWORD *data, a, b, c;
	es.resize( (n + m?1:0 )*4 );
	data = (DWORD*)&es[0];
	for (int i=0; i<n; ++i) {
		a = s[i], b = s[i+1], c = s[i+2];
		data[i] = b64[a>>2] | b64[(a&0x3)<<4 | b>>4] << 8 |
			 b64[(b&0xf)<<2 | c>>6] << 16 | b64[c&0x3f] << 24;
	}
	if ( m ) {
		a = s[n];
		b = m>1 ? s[n+1] : 0;
		c = 0;
		data[n] = b64[a>>2] | b64[(a&0x3)<<4 | b>>4] << 8 |
			 (m>1 ? b64[(b&0xf)<<2] : '=') << 16 | '=' << 24;
	}
	return es;
}

string
Base64::operator[] (string const& s)
{
	string ds;
	if ( ! s.size() || s.size()%4 )
		return ds;
	int n = s.size()/4;
	DWORD a, b, c;
	ds.resize( n*3 );
	for (int i=0; i<n; ++i) {
		a = s[i], b = s[i+1], c = s[i+2];
		data[i] = b64[a>>2] | b64[(a&0x3)<<4 | b>>4] << 8 |
			 b64[(b&0xf)<<2 | c>>6] << 16 | b64[c&0x3f] << 24;
	}
	if ( m ) {
		a = s[n];
		b = m>1 ? s[n+1] : 0;
		c = 0;
		data[n] = b64[a>>2] | b64[(a&0x3)<<4 | b>>4] << 8 |
			 (m>1 ? b64[(b&0xf)<<2] : '=') << 16 | '=' << 24;
	}
	return ds;
}

Base64::DWORD Base64::b64[64] = {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/'
};

char Base64::c256[256] =
{      -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   62,   -1,   -1,   -1,   63,   52,   53,   54,
       55,   56,   57,   58,   59,   60,   61,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,    0,    1,    2,    3,    4,    5,
        6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
       16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
       -1,   -1,   -1,   -1,   -1,   -1,   26,   27,   28,   29,

       30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
       40,   41,   42,   43,   44,   45,   46,   47,   48,   49,
       50,   51,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,

       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1
} ;

} // namespace akm

#endif // _BASE64_H_

