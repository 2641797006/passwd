#include <iostream>
#include "des.h"
using namespace std;
using namespace akm;

int main()
{
	Des des;
	Des::QWORD q = 7661238, r;

	r = des.init_permutation(q);
	cout<< r <<endl;
	r = des.final_permutation(r);
	cout<< r <<endl;
}

