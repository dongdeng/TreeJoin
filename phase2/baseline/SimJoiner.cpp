#include "SimJoiner.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <unordered_set>

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

unordered_map<string, int> m_map;

void SimJoiner::createInvertedListED(unsigned q, unsigned threshold)
{
	int i = 0;
	m_map.clear();
	for(auto & s : m_str1) {
	//for(int i = 0; i < m_str1.size(); ++ i) {
		int length = s.length();
		for (int j = 0; j <= length - q; ++j) {
			string sub = s.substr(j, q);
			m_map[sub] += 1;
		}
	}
}

void SimJoiner::preprocessED(const char *filename1, const char *filename2, unsigned q, unsigned threshold)
{
	//read the strings
	readFiles(filename2, filename1);

	//create the inverted list of m_str1
 	createInvertedListED(q, threshold);
}

bool PairCompare(const string &a, const string &b) {
	return m_map[a] < m_map[b];
}

void SimJoiner::getResultED(unsigned q, unsigned threshold, vector<int> &d0, vector<int> &d1, vector<EDJoinResult> &resultED)
{
	EDJoinResult rr;
	int candidate = 0;
	unordered_map<string, vector<int> > L;
	for(int i = 0; i < m_str2.size(); ++i) {
		//get the list
		vector<string> list;
		int length = m_str2[i].length();
		for (int j = 0; j <= length - q; ++j) {
			string sub = m_str2[i].substr(j, q);
			list.push_back(sub);
		}
		sort(list.begin(), list.end(), PairCompare);

		//get the prefix
		unordered_set<string> exist;
		int m = list.size(), num = 0;
		vector<int> flag(m, 0);
		flag[0] = 1;
		int k;
		for (k = 0; k < m; ++k) {
			if (num == q * threshold + 1)
				break;
			if (exist.find(list[k]) == exist.end()) {
				++num;
				flag[k] = 1;
				exist.insert(list[k]);
			}
		}

		//get the candidates
		vector<int> candidates;
		unordered_map<int, bool> isDup;
		if (num < q * threshold + 1) {
			//cout << "wo cao" << endl;
			for (int j = 0; j <= i; ++j)
				candidates.push_back(j);
		}
		else {
			for (int j = 0; j < k; ++j)
			//if (flag[j] == 1) {
				if (L.find(list[j]) != L.end()) {
					for (auto & l : L[list[j]]) {
						//some pruning techniques
						//PRUNING 1
 						if (isDup.find(l) == isDup.end()) {
							candidates.push_back(l);
							isDup[l] = true;
						}
						//PRUNING 2
					}
				}
			//}
			candidates.push_back(i);
		}
		candidate += candidates.size();
		for (auto & j : candidates) {
			int distance = getED(m_str1[j], m_str1[i], threshold);
			if (distance <= threshold) {
				rr.id1 = i;
				rr.id2 = j;
				rr.s = distance;
				resultED.push_back(rr);
				if (i != j) {
					rr.id1 = j;
					rr.id2 = i;
					resultED.push_back(rr);
				}
			}
		}

		//verification

		//indexing all the prefix
		for (int j = 0; j < k; ++j) {
			if (L.find(list[j]) == L.end()) {
				vector<int> temp;
				L[list[j]] = temp;
			}
			L[list[j]].push_back(i);
		}
	}
	cout << "the number of candidates = " << candidate << endl;
}

int SimJoiner::getED(TString &a, TString &b, int threshold)
{
	double dis = 0;
	int len_a = a.length(), len_b = b.length();
	vector<int> d0(len_b + 1, 0);
	vector<int> d1(len_b + 1, 0);
	dis = threshold + 1;
	if (abs(len_a - len_b) > threshold)
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

