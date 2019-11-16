#ifndef _AES_H_
#define _AES_H_

#define _XTIME(x)	( (x<<1) ^ (((x>>7) & 1) * 0x1b) )
#define XTIME(x)	_XTIME((x))

#define _MUL(x, y) (((y & 1) * x) ^ ((y>>1 & 1) * XTIME(x)) ^ ((y>>2 & 1) * XTIME(XTIME(x))) ^ ((y>>3 & 1) * XTIME(XTIME(XTIME(x)))) ^ ((y>>4 & 1) * XTIME(XTIME(XTIME(XTIME(x))))))
#define MUL(x, y)	_MUL((x), (y))














#endif

