#ifndef _CAESAR_H_
#define _CAESAR_H_

#ifndef _GLIBCXX_STRING
#include <string>
#endif

namespace akm{
using namespace std;

class Caesar{
	int K, N;
	int lower;
	int ctoi(int c);

  public:
	Caesar(int k=3) { K = k; N = 26; }

	char encrypt(int c);
	char decrypt(int c);
	string encrypt(string const& s);
	string decrypt(string const& s);

}; // class Caesar;

} // namespace akm;

#endif
