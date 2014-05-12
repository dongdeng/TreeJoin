#include "SimJoiner.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <algorithm>

bool EDJoinResultCompare(const EDJoinResult &a, const EDJoinResult &b) {
	return a.id1 == b.id1 ? a.id2 < b.id2 : a.id1 < b.id1;
}

bool JaccardJoinResultCompare(const JaccardJoinResult &a, const JaccardJoinResult &b) {
	return a.id1 == b.id1 ? a.id2 < b.id2 : a.id1 < b.id1;
}

bool stringCompare(const string &a, const string &b) {
	return a.length() == b.length() ? a < b : a.length() < b.length();
}

using namespace std;

SimJoiner::SimJoiner()
{
}

SimJoiner::~SimJoiner()
{
}

//read the strings
void SimJoiner::readFiles(const char *filename1, const char *filename2) {
	ifstream fin1(filename1);
	string s;
	m_len1 = 0;
	while(getline(fin1, s)) {
		TString temp(s);
		m_str1.push_back(temp);
		if(temp.length() > m_len1)
			m_len1 = temp.length();
	}
	fin1.close();

	ifstream fin2(filename2);
	m_len2 = 0;
	while(getline(fin2, s)) {
		TString temp(s);
		m_str2.push_back(temp);
		if(temp.length() > m_len2)
			m_len2 = temp.length();
	}
	fin2.close();
}

void SimJoiner::createInvertedListED(unsigned q, unsigned threshold)
{
	m_sstr.clear();
	int i = 0;
	for(auto & s : m_str1) {
	//for(int i = 0; i < m_str1.size(); ++ i) {
		unsigned length = s.length();
		if(length < threshold + 1) {
			m_sstr.push_back(i);
			++ i;
			continue;
		}
		int len1 = length / (threshold + 1), len2 = len1 + 1, k = length % (threshold + 1); 
		for(int j = 0; j < threshold + 1 - k; ++ j) {
			string sub = s.substr(j * len1, len1);
			m_list.insert(i, length, j, sub);
		}
		for(int j = 0; j < k; ++ j) {
			string sub = s.substr((threshold + 1 - k) * len1 + j * len2, len2);
			m_list.insert(i, length, threshold + 1 - k + j, sub);
		}
		++ i;
	}
}

void SimJoiner::preprocessED(const char *filename1, const char *filename2, unsigned q, unsigned threshold)
{
	//read the strings
	readFiles(filename2, filename1);

	//create the inverted list of m_str1
 	createInvertedListED(q, threshold);
}

void SimJoiner::getResultED(unsigned q, unsigned threshold, vector<int> &d0, vector<int> &d1, vector<EDJoinResult> &resultED)
{
	vector<vector<unordered_map<string, vector<int> > > > &list = m_list.getList();
	int i = 0;
	EDJoinResult rr;
	for(auto & s : m_str2) {
	//for(int i = 0; i < m_str2.size(); ++ i) {
		unsigned length = s.length();
		unordered_map<int, bool> map;
		int pstart = max(1, int(length) - int(threshold)), pmax = min(int(length + threshold), int(list.size()) - 1);
		for(int p = pstart; p <= pmax; ++ p) {
			int q = 0;
			for(auto & m : list[p]) {
				if(m.size() == 0) {
					++ q;
					continue;
				}
				int len = 0, pos = -1, k = p % (threshold + 1);
				if(q >= threshold + 1 - k) {
					len = p / (threshold + 1) + 1;
					pos = (len - 1) * (threshold + 1 - k) + (q - threshold - 1 + k) * len;
				}
				else {
					len = p / (threshold + 1);
					pos = len * q;
				}
				int jstart = max(0, pos - q);
				jstart = max(jstart, pos + int(length) - p - int(threshold) + q - 2);
				int jmax = min(int(length) - len, pos + q);
				jmax = min(jmax, pos + int(length) - p + int(threshold) - q);
				for(int j = jstart; j <= jmax; ++ j) {
					string sub = s.substr(j, len);
					if(m.find(sub) != m.end())
						for(auto & r : m[sub])
							if(map.find(r) == map.end()) {
								int ed = getED(m_str1[r], m_str2[i], threshold, d0, d1);
								if(ed <= threshold) {
									rr.id1 = i;
									rr.id2 = r;
									rr.s = ed;
									resultED.push_back(rr);
								}
								map[r] = true;
							}
				}
				++ q;
			}
		}
		int k = 0;
		while(k < m_sstr.size()) {
			int ed = getED(m_str1[m_sstr[k]], m_str2[i], threshold, d0, d1);
			if(ed <= threshold) {
				rr.id1 = i;
				rr.id2 = m_sstr[k];
				rr.s = ed;
				resultED.push_back(rr);
			}
			++ k;
		}
		++ i;
	}
}

unsigned SimJoiner::getED(TString &a, TString &b, unsigned threshold, vector<int> &d0, vector<int> &d1)
{
	unsigned dis = threshold + 1;
	int len_a = a.length(), len_b = b.length();
	if(abs(len_a - len_b) > threshold)
		return dis;
    for(int i = 0; i <= len_a; ++ i)
    {
       	int l = max(0, i - (int)threshold), r = min(len_b, i + (int)threshold);
        int minDis = threshold + 1;
        for(int j = l; j <= r; ++ j)
        {
        	if(i == 0)
                d1[j] = j;
            else if(j == 0)
               	d1[j] = i;
        	else 
        	{
        		if(a.getElement(i - 1) == b.getElement(j - 1))
                    d1[j] = d0[j - 1];
                else
                   	d1[j] = d0[j - 1] + 1;
        		if(j > l) d1[j] = min(d1[j], d1[j - 1] + 1);
            	if(j < i + threshold) d1[j] = min(d1[j], d0[j] + 1);
            }
            minDis = min(minDis, d1[j]);	
        }
        if(minDis > threshold)
        	return dis;
        swap(d0, d1);
   	}
    dis = d0[len_b];
    return dis;
}

int SimJoiner::joinED(const char *filename1, const char *filename2, unsigned q, unsigned threshold, vector<EDJoinResult> &result)
{
	//preprocess the strings
	result.clear();
	preprocessED(filename1, filename2, q, threshold);

	//get the result(for each one in m_str2)
	vector<int> d0(m_len1), d1(m_len2);
	getResultED(q, threshold, d0, d1, result);

	//sort(result.begin(), result.end(), EDJoinResultCompare);

	return SUCCESS;
}

