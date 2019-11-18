#ifndef _AES_H_
#define _AES_H_

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

// openssl aes-128-ecb -md md5 -nosalt -k $KEY -in $IN -out $OUT -e/-d
// padding = PKCS7
class Aes {
  public:
	using BYTE = uint8_t;
	using DWORD = uint32_t;

  private:
	static DWORD S_Box[16][16], Inv_S_Box[16][16], Rcon[11];

	DWORD *ek; // key_expansion ek[60]
	DWORD nk=4, nr=10;

	md5sum md5;

	DWORD xtime(DWORD);
	DWORD mul(DWORD, DWORD);
	DWORD sub_dword(DWORD); // S-box
	void key_expansion(DWORD*);

	void add_round_key(DWORD*, DWORD*);

	void sub_bytes(DWORD*); // S-box
	void shift_rows(DWORD*);
	void mix_columns(DWORD*);

	void inv_sub_bytes(DWORD*);
	void inv_shift_rows(DWORD*);
	void inv_mix_columns(DWORD*);

	void fcrypt(fstream&, fstream&, int flag);

  public:
	Aes();
	~Aes();

	void set_key(DWORD*, DWORD n=4);
	void set_key(string const& s, DWORD n=128);

	void encrypt(DWORD*);
	void decrypt(DWORD*);
	string encrypt(string const&);
	string decrypt(string const&);
	void encrypt(fstream&, fstream&);
	void decrypt(fstream&, fstream&);
};

inline
Aes::DWORD
Aes::xtime(DWORD d)
{
	if (d & 0x80)
		return (d<<1) ^ 0x11b;
	return d<<1;
}

inline
void
Aes::set_key(DWORD *da, DWORD n)
{
	nk = n;
	nr = nk + 6;
	key_expansion(da);
}

} // namespace akm;

#endif // _AES_H_

