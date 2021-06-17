#ifndef ALL
#define ALL
#include <bits/stdc++.h>
using namespace std;
//#include "BPlustree.hpp"
const int R = 1e2 + 5;
const int SF = 1e6 + 5;
const int F = 1e5 + 5;
const int N = 1e4 + 5;
const int MAXTrain = R;

typedef std::pair<int, int> pii;

int getnum(char *s) {
	int t = 0;
	int l = strlen(s);
	for(int i = 0; i < l; ++ i)
	t = t * 10 + s[i] - '0';
	return t;
}

#endif
