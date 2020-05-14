#pragma once
#include<string>
using namespace std;

template <class K> class KeyComp
{

public:
	int operator()(const K& k1, const K& k2) {
		if (k1 < k2)
			return -1;
		else if (k1 == k2)
			return 0;
		return 1;
	}
};

template<> class KeyComp<string>
{
public:
	int operator()(const string& k1, const string& k2)
	{
		int n;
		if (k1.length() > k2.length())
			n = k2.length() / 2;
		else n = k1.length() / 2;

		if ((k1 == k2) || k1.substr(0, n) == k2.substr(0, n))
			return 0;
		else
		{
			for (int i = 0; i <= n; i++)
				if (k1[i] < k2[i])
					return -1;
				else return 1;
		}
		
	}
};