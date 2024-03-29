#ifndef _AES_H_
#define _AES_H_

#define _RotDword(d)	( d<<8 | d>>24 )
#define RotDword(d)	_RotDword( (d) )
#define _SwapDword(d)	( d>>24 | ((d>>8) & 0xff00) | ((d<<8) & 0xff0000 ) | d<<24 )
#define SwapDword(d)	_SwapDword( (d) )

namespace akm {
using namespace std;

class Aes {
  public:
	using BYTE = uint8_t;
	using DWORD = uint32_t;
	using QWORD = uint64_t;

  private:
	DWORD *ek; // key_expansion ek[60]
	DWORD nk=4, nr=10;
	static DWORD S_Box[16][16], Rcon[11];

  public:
	Aes() { ek = new DWORD[60]; }
	~Aes() { delete[] ek; }

	DWORD xtime(DWORD);
	DWORD mul(DWORD, DWORD);
	DWORD sub_dword(DWORD);
	void sub_bytes(DWORD*);
	void key_expansion(DWORD*); // 128bit = DWORD*4
	void add_round_key(DWORD*, DWORD*);
	void shift_rows(DWORD*);
	void mix_columns(DWORD*);

	void set_key(DWORD*, DWORD n=4);
	void encrypt(DWORD*);
	void decrypt(DWORD*);

	void print_da(DWORD*, const char *s="");
	void print_ek();
};

void
Aes::encrypt(DWORD *da)
{
#if 1
	BYTE *const ba = (BYTE*)da;
	for (DWORD r=1; r<4; ++r)
		for (DWORD c=0; c<r; ++c)
			swap( ba[(r<<2)+c], ba[r+(c<<2)] );
#endif
	print_da(da, "明文");//
	add_round_key(da, ek);
	print_da(da, "r1");//

	for (DWORD round=1; round<nr; ++round) {
		sub_bytes(da);
	print_da(da, "sub");//
		shift_rows(da);
	print_da(da, "row");//
		mix_columns(da);
	print_da(da, "col");//
		add_round_key(da, ek+(round<<2));
	print_da(da, "round");//
	}

	sub_bytes(da);
	shift_rows(da);
	add_round_key(da, ek+(nr<<2));
}

inline
void
Aes::set_key(DWORD *da, DWORD n)
{
	nk = n;
	nr = nk + 6;
	key_expansion(da);
}

void
Aes::mix_columns(DWORD *da)
{
	BYTE *const ba = (BYTE*)da;
	BYTE ca[4]; // col
	for (DWORD i=0; i<4; ++i) {
		for (DWORD j=0; j<4; ++j)
			ca[j] = ba[i+(j<<2)];
		ba[i] = mul(0x2, ca[0]) ^ mul(0x3, ca[1]) ^ ca[2] ^ ca[3];
		ba[i+4] = ca[0] ^ mul(0x2, ca[1]) ^ mul(0x3, ca[2]) ^ ca[3];
		ba[i+8] = ca[0] ^ ca[1] ^ mul(0x2, ca[2]) ^ mul(0x3, ca[3]);
		ba[i+12] = mul(0x3, ca[0]) ^ ca[1] ^ ca[2] ^ mul(0x2, ca[3]);
	}
}

void
Aes::shift_rows(DWORD *da)
{
	da[1] = da[1]>>8 | da[1]<<24;
	da[2] = da[2]>>16 | da[2]<<16;
	da[3] = da[3]>>24 | da[3]<<8;
}

void
Aes::sub_bytes(DWORD *da)
{
	BYTE *const ba = (BYTE*)da;
	for (DWORD i=0; i<16; ++i)
		ba[i] = S_Box[ (ba[i]>>4)&0xf ][ ba[i]&0xf ];
}

void
Aes::add_round_key(DWORD *da, DWORD *k)
{
	BYTE *const ba = (BYTE*)da;
	for (DWORD i=0; i<4; ++i) {
#if 1
		ba[i] ^= k[i] >> 24;
		ba[i+4] ^= (k[i] >> 16) & 0xff;
		ba[i+8] ^= (k[i] >> 8) & 0xff;
		ba[i+12] ^= k[i] & 0xff;
#else
		da[i] ^= k[i];
#endif
	}
}

void // key nk;  e nk+6+1
Aes::key_expansion(DWORD *key)
{
	DWORD i, t;
	for (i=0; i<nk; ++i)
#if 1
		ek[i] = SwapDword(key[i]);
#else
		ek[i] = key[i];
#endif
	DWORD n = (nr + 1) << 2;
	while (i <= n) {
		t = ek[i-1];
		if (i%nk == 0)
			t = sub_dword(RotDword(t)) ^ Rcon[i/nk-1];	//TODO;
		else if (nk == 8 && (i%nk == 4))
			t = sub_dword(t);
		ek[i] = ek[i-nk] ^ t;
		++i;
	}
}

Aes::DWORD
Aes::sub_dword(DWORD d)
{
	DWORD r=0, t;
	for (DWORD i=0; i<32; i+=8) {
		t = d>>i;
		t = S_Box[ (t>>4)&0xf ][ t&0xf ];
		r |= t<<i;
	}
	return r;
}

Aes::DWORD
Aes::mul(DWORD a, DWORD b)
{
	DWORD r=0;
	for (DWORD i=1; i&~0x100; i<<=1) {
		if (b & i)
			r ^= a;
		a = xtime(a);
	}
	return r;
}

inline
Aes::DWORD
Aes::xtime(DWORD d)
{
	if (d & 0x80)
		return (d<<1) ^ 0x11b;
	return d<<1;
}


Aes::DWORD Aes::S_Box[16][16]={
	{ 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76 },
	{ 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0 },
	{ 0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15 },
	{ 0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75 },
	{ 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84 },
	{ 0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF },
	{ 0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8 },
	{ 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2 },
	{ 0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73 },
	{ 0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB },
	{ 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79 },
	{ 0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08 },
	{ 0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A },
	{ 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E },
	{ 0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF },
	{ 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 }
};

Aes::DWORD Aes::Rcon[11] = {
0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000,
0x6c000000
};

void
Aes::print_da(DWORD *da, const char *s)
{
	puts(s);
	BYTE *const ba = (BYTE*)da;
#if 1
	for (int i=0; i<16; ++i) {
		printf("%02x ", ba[i]);
		if (i==7)
			puts("");
	}
	printf("\n");
//#else
	for (int r=0; r<4; ++r) {
		for (int c=0; c<4; ++c)
			printf("%02x ", ba[r+(c<<2)]);
		puts("");
	}
#endif
	printf("\n");
}

void
Aes::print_ek()
{
	DWORD n = (nr + 1)<<2;
	for (DWORD i=0; i<n; ++i) {
		printf("%08x  ", ek[i]);
		if (i%4 == 3)
			printf("\n");
	}
	printf("\n");
}

} // namespace akm;

#undef RotDword
#undef _RotDword

#endif // _AES_H_

