#ifndef _MD5SUM_H_
#define _MD5SUM_H_

#define akm_MD5_UPPER	0x0001

namespace akm{
using namespace std;

class md5sum
{
  public:
	void set_upper(){flag|=akm_MD5_UPPER;}
	void set_lower(){flag&=~akm_MD5_UPPER;}
	string const& operator()(string const&);
	string const& operator()(string const& str, string const& salt){return (*this)(str+salt);}

	md5sum():flag(0){}
	uint32_t getA() { return A; }
	uint32_t getB() { return B; }
	uint32_t getC() { return C; }
	uint32_t getD() { return D; }

  private:
	uint32_t flag;
	uint32_t A, B, C, D;
	uint32_t a, b, c, d;
	uint32_t x[16];
	string s_null;
	void init(){A=0x67452301, B=0xefcdab89, C=0x98badcfe, D=0x10325476;}
	void loop();
};

} //namespace akm

#endif
