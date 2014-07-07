#ifndef TSTRING_H
#define TSTRING_H

#include <vector>
#include <iostream>

using namespace std;


class TString {
private:
	vector<string> m_str;
public:
	TString();
	TString(string s);
	~TString();
	int length();
	string substr(int pos, int length);
	string &getElement(int index);
};

#endif