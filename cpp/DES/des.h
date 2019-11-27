#ifndef _DES_H_
#define _DES_H_

#ifndef _GLIBCXX_STRING
#include <string>
#endif

#ifndef _GLIBCXX_FSTREAM
#include <fstream>
#endif

#ifndef _MD5SUM_H_
#include "md5sum.h"
#endif

namespace akm {
using namespace std;

class Des {
  public:
	using BYTE = uint8_t;
	using DWORD = uint32_t;
	using QWORD = uint64_t;

  private:
	static DWORD ip64[64], fp64[64], e48[48],
		s641[4][16], s642[4][16], s643[4][16], s644[4][16],
		s645[4][16], s646[4][16], s647[4][16], s648[4][16],
		(*s64[8])[16],
		p32[32], pc56[56], pc48[48], ls16[16];

	md5sum md5;
	QWORD key;
	QWORD *ckey;
	QWORD swap_bit(QWORD, DWORD*, int, int);
	QWORD loop_moveL28(QWORD, int);
	QWORD loop_moveR28(QWORD, int);
	void new_ckey() { ckey = new QWORD[16]; }
	void init_ckey();

  public:
	Des(QWORD q=0) { new_ckey(); set_key(q); }
	Des(string const& s) { new_ckey(); set_key(s); }
	~Des() { delete[] ckey; }

	void set_key(QWORD q) { key = q; init_ckey(); }
	void set_key(string const&);
	QWORD get_key() { return key; }

	QWORD init_permutation(QWORD); // 初始化排列 ip64
	QWORD final_permutation(QWORD); // 最终排列 fp64
	QWORD expand(QWORD); // 扩展 32bit -> 48bit
	QWORD s_box(QWORD); // S-box 48bit -> 32bit
	QWORD pp(QWORD); // s_box:32bit -> 32bit
	QWORD pc1(QWORD); // key:64bit -> 56bit
	QWORD pc2(QWORD); // pc1:56bit -> 48bit

	QWORD feistel(QWORD, int);
	QWORD encrypt(QWORD);
	QWORD decrypt(QWORD);
	string encrypt(string const&);
	string decrypt(string const&);
	void crypt(fstream&, fstream&, int flag);
	void encrypt(fstream&, fstream&);
	void decrypt(fstream&, fstream&);
};

} // namespace akm

#endif // _DES_H_
