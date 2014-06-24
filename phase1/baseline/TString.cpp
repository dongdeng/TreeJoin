#include "TString.h"

using namespace std;

TString::TString() {
}

TString::TString(string s) {
	int i = 0;
	int length = s.length();
	while (i < length) {
		int j = i;
		for (; j < length; ++j)
			if (s[j] == '$')
				break;
		m_str.push_back(s.substr(i, j - i));
		i = j + 1;
	}
}

TString::~TString() {
}

int TString::length() {
	return m_str.size();
}

string TString::substr(int pos, int length) {
	string ret = "";
	for (int i = pos; i < length; ++i)
		ret += m_str[i];
	return ret;
}

string &TString::getElement(int index) {
	return m_str[index];
}