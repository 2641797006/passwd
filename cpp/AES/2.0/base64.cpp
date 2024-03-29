#include "base64.h"

namespace akm {
using namespace std;

Base64::Base64(char c62, char c63, char f)
{

b64 = new DWORD[64] {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/'
};

c256 = new char[256]
{      -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
       -1,   -1,   -1,   -1,   -1,   -1,   -1,   52,   53,   54,
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

	b64[62] = (BYTE)c62;
	b64[63] = (BYTE)c63;
	c256[b64[62]] = 62;
	c256[b64[63]] = 63;
	fill = f;
}

string
Base64::operator() (string const& s)
{
	string es;
	if ( ! s.size() )
		return es;
	int n = s.size()/3;
	int m = s.size()%3;
	DWORD *data, a, b, c;
	es.resize( (n + (m?1:0) )*4 );
	data = (DWORD*)&es[0];
	for (int i=0,i3=0; i<n; ++i,i3+=3) {
		a = (BYTE)s[i3], b = (BYTE)s[i3+1], c = (BYTE)s[i3+2];
		data[i] = b64[a>>2] | b64[(a&0x3)<<4 | b>>4] << 8 |
			 b64[(b&0xf)<<2 | c>>6] << 16 | b64[c&0x3f] << 24;
	}
	if ( m ) {
		a = (BYTE)s[n*3];
		b = m>1 ? (BYTE)s[n*3+1] : 0;
		c = 0;
		data[n] = b64[a>>2] | b64[(a&0x3)<<4 | b>>4] << 8 |
			 (m>1 ? b64[(b&0xf)<<2] : fill) << 16 | fill << 24;
	}
	return es;
}

string
Base64::operator[] (string const& _s)
{
	static const string err = "base64: error decrypt";
	string s = _s;
	string ds;
	if ( ! s.size() || s.size()%4 )
		return err;
	int fill_num = 0;
	if ( (BYTE)s[s.size()-1] == fill ) {
		s[s.size()-1] = '0', ++fill_num;
		if ( (BYTE)s[s.size()-2] == fill )
			s[s.size()-2] = '0', ++fill_num;
	}
	int n = s.size()/4;
	DWORD a[4];
	ds.resize( n*3 );
	for (int i=0,i3=0,i4=0; i<n; ++i,i3+=3,i4+=4) {
		for (int j=0; j<4; ++j) {
			char c = c256[(BYTE)s[i4+j]];
			if ( c < 0 )
				return err;
			a[j] = c;
		}
		ds[i3] = a[0] << 2 | a[1] >> 4;
		ds[i3+1] = a[1] << 4 | a[2] >> 2;
		ds[i3+2] = a[2] << 6 | a[3];
	}
	ds.resize(ds.size() - fill_num);
	return ds;
}

} // namespace akm


