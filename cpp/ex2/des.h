#ifndef _DES_H_
#define _DES_H_

#define QWORD_BIT(q, n)	( (q >> (n-1)) & 1 )

namespace akm {

class Des {
  public:
	using BYTE = uint8_t;
	using DWORD = uint32_t;
	using QWORD = uint64_t;

  private:
	static DWORD ip64[64], fp64[64];
	DWORD *d64;
	QWORD permutation(QWORD, int);

  public:
	Des(){ d64 = new DWORD[64]; }
	~Des(){ delete[] d64; }
	QWORD init_permutation(QWORD); // 初始化排列 ip[64]
	QWORD final_permutation(QWORD); // 最终排列 fp[64]
};

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

Des::QWORD
Des::permutation(QWORD q, int flag)
{
	QWORD r = 0;
	DWORD *p64 = flag ? ip64 : fp64;
	for (int i=0; i<64; ++i)
		r |= QWORD_BIT(q, p64[i]) << i;
	return r;
}

Des::QWORD
Des::init_permutation(QWORD q)
{
	return permutation(q, 1);
}

Des::QWORD
Des::final_permutation(QWORD q)
{
	return permutation(q, 0);
}


} // namespace akm

#endif // _DES_H_

