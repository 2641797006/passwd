#include "des.h"

#define _QB(q, n)	( (q >> n) & 1 )
#define QB(q, n)	_QB((q), (n))

#define SX(q)	( (QB(q,5) << 1) | QB(q,0) )
#define SY(q)	( QB(q,1) | (QB(q,2) << 1) | (QB(q,3) << 2) | QB(q,4) << 3 )
#define _S(q, i)	( s64[i][SX(q)][SY(q)] )
#define S(q, i)	_S((q), (i))

#define _L28(q)	( (q & 0xfffffff0000000) >> 28 )
#define _R28(q)	( q & 0xfffffff )
#define L28(q)	_L28((q))
#define R28(q)	_R28((q))

#define _L32(q)	( (q & 0xffffffff00000000) >> 32 )
#define _R32(q)	( q & 0xffffffff )
#define L32(q)	_L32((q))
#define R32(q)	_R32((q))

namespace akm {
using namespace std;

void
Des::init_ckey()
{
	QWORD q, c, d;

	q = pc1(key);
	c = L28(q);
	d = R28(q);

	for (int i=0; i<16; ++i) {
		c = loop_moveL28(c, ls16[i]);
		d = loop_moveL28(d, ls16[i]);
		ckey[i] = pc2( c << 28 | d );
	}
}

void
Des::set_key(string const& s)
{
	md5(s);
	QWORD a = md5.getA();
	QWORD c = md5.getC();
	key = a << 32 | c;
	init_ckey();
}

Des::QWORD
Des::swap_bit(QWORD q, DWORD *table, int n, int m)
{
	QWORD r = 0;
	for (int i=0; i<n; ++i) {
		r <<= 1;
                r |= QB(q, m - table[i]);
	}
	return r;
}

Des::QWORD
Des::init_permutation(QWORD q)
{
	return swap_bit(q, ip64, 64, 64);
}

Des::QWORD
Des::final_permutation(QWORD q)
{
	return swap_bit(q, fp64, 64, 64);
}

Des::QWORD
Des::expand(QWORD q)
{
	return swap_bit(q, e48, 48, 32);
}

Des::QWORD
Des::s_box(QWORD q)
{
	QWORD r = 0;
	for (int i=0; i<8; ++i) {
		r <<= 4;
                r |= S( (q >> (7-i)*6) & 0x3f,  i);
	}
	return r;
}

Des::QWORD
Des::pp(QWORD q)
{
	return swap_bit(q, p32, 32, 32);
}

Des::QWORD
Des::pc1(QWORD q)
{
	return swap_bit(q, pc56, 56, 64);
}

Des::QWORD
Des::pc2(QWORD q)
{
	return swap_bit(q, pc48, 48, 56);
}

Des::QWORD
Des::loop_moveL28(QWORD q, int n)
{
	QWORD tmp;
	tmp = q << n;
	q = (tmp & 0xfffffff) | ((tmp & 0xfffffff0000000) >> 28);
	return q;
}

Des::QWORD
Des::loop_moveR28(QWORD q, int n)
{
	return (q >> n) | ((q << (28-n)) & 0xfffffff );
}

Des::QWORD
Des::feistel(QWORD r, int i)
{
	return pp(s_box( ckey[i] ^ expand(r) ));
}

Des::QWORD
Des::encrypt(QWORD q)
{
	QWORD ql, qr, tmp;
	q = init_permutation(q);
	ql = L32(q);
	qr = R32(q);

	for (int i=0; i<16; ++i) {
		tmp = qr;
		qr = feistel(qr, i) ^ ql;
		ql = tmp;
	}

	q = qr << 32 | ql;

	return final_permutation(q);
}

Des::QWORD
Des::decrypt(QWORD q)
{
	QWORD ql, qr, tmp;
	q = init_permutation(q);
	ql = L32(q);
	qr = R32(q);

	for (int i=0; i<16; ++i) {
		tmp = qr;
		qr = feistel(qr, 15-i) ^ ql;
		ql = tmp;
	}

	q = qr << 32 | ql;

	return final_permutation(q);
}

string
Des::encrypt(string const& s)
{
	static const string err = "error encrypt";
	string es = s;
	QWORD size = es.size();
	if ( ! size )
		return err;
	size_t n = size % 8;
	if ( n )
		es.resize( size-n+8, 0 );
	n = es.size() / 8;
	QWORD *data = (QWORD*)&es[0];
	for (size_t i=0; i<n; ++i)
		data[i] = encrypt(data[i]);
	es.resize( es.size() + 8 );
	data = (QWORD*)&es[0];
	data[n] = size;
	return es;
}

string
Des::decrypt(string const& s)
{
	static const string err = "error decrypt";
	string ds = s;
	if ( ds.size()<=8 || ds.size()%8 )
		return err;
	QWORD *data = (QWORD*)&ds[0];
	size_t n = ds.size()/8 - 1;
	QWORD size = data[n];
	if ( ds.size() - size >= 16 || ds.size() - size < 8 )
		return err;
	ds.resize( ds.size() - 8 );
	data = (QWORD*)&ds[0];
	for (size_t i=0; i<n; ++i)
		data[i] = decrypt(data[i]);
	ds.resize( size );
	return ds;
}

void
Des::crypt(fstream& in, fstream& out, int flag)
{
	in.seekg(0, ios_base::end);
	size_t size = in.tellg();
	if ( ! size )
		return;
	string s;
	s.resize( size );
	in.seekg(0);
	in.read( &s[0], size );
	s = flag ? encrypt(s) : decrypt(s);
	out.write( &s[0], s.size() );
}

void
Des::encrypt(fstream& in, fstream& out)
{
	crypt(in, out, 1);
}

void
Des::decrypt(fstream& in, fstream& out)
{
	crypt(in, out, 0);
}

Des::DWORD Des::ip64[64] = {
58,50,42,34,26,18,10,2,
60,52,44,36,28,20,12,4,
62,54,46,38,30,22,14,6,
64,56,48,40,32,24,16,8,
57,49,41,33,25,17, 9,1,
59,51,43,35,27,19,11,3,
61,53,45,37,29,21,13,5,
63,55,47,39,31,23,15,7,
};

Des::DWORD Des::fp64[64] = {
40,8,48,16,56,24,64,32,
39,7,47,15,55,23,63,31,
38,6,46,14,54,22,62,30,
37,5,45,13,53,21,61,29,
36,4,44,12,52,20,60,28,
35,3,43,11,51,19,59,27,
34,2,42,10,50,18,58,26,
33,1,41, 9,49,17,57,25,
};

Des::DWORD Des::e48[48] = {
32, 1, 2, 3, 4, 5,
 4, 5, 6, 7, 8, 9,
 8, 9,10,11,12,13,
12,13,14,15,16,17,
16,17,18,19,20,21,
20,21,22,23,24,25,
24,25,26,27,28,29,
28,29,30,31,32, 1
};

Des::DWORD Des::s641[4][16] = {
{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
};

Des::DWORD Des::s642[4][16] = {
{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
};

Des::DWORD Des::s643[4][16] = {
{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
};

Des::DWORD Des::s644[4][16] = {
{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
};

Des::DWORD Des::s645[4][16] = {
{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
};

Des::DWORD Des::s646[4][16] = {
{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
};

Des::DWORD Des::s647[4][16] = {
{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
};

Des::DWORD Des::s648[4][16] = {
{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
};

Des::DWORD (*Des::s64[8])[16] = {
s641, s642, s643, s644,
s645, s646, s647, s648
};

Des::DWORD Des::p32[32] = {
16, 7,20,21,29,12,28,17,
 1,15,23,26, 5,18,31,10,
 2, 8,24,14,32,27, 3, 9,
19,13,30, 6,22,11, 4,25
};

Des::DWORD Des::pc56[56] = {
57,49,41,33,25,17, 9, 1,
58,50,42,34,26,18,10, 2,
59,51,43,35,27,19,11, 3,
60,52,44,36,63,55,47,39,
31,23,15, 7,62,54,46,38,
30,22,14, 6,61,53,45,37,
29,21,13, 5,28,20,12, 4
};

Des::DWORD Des::pc48[48] = {
14,17,11,24, 1, 5, 3,28,
15, 6,21,10,23,19,12, 4,
26, 8,16, 7,27,20,13, 2,
41,52,31,37,47,55,30,40,
51,45,33,48,44,49,39,56,
34,53,46,42,50,36,29,32
};

Des::DWORD Des::ls16[16] = {
1, 1, 2, 2, 2, 2, 2, 2,
1, 2, 2, 2, 2, 2, 2, 1
};

} // namespace akm

#undef QB


