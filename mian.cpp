#include "stdafx.h"
#include"hyouka.h"
#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<fstream>
#include<string>
using namespace std;

int main()
{
	satoshi::Database da;
	while (1) {
		bool t=da.check_instructor();
		if (!t) {
			break;
		}
		da.operation();
	}
    return 0;
}
