	#pragma once
#include <vector>
#include <unordered_map>
#include "IList.h"
#include "Gram.h"
#include "TString.h"

template <typename _IDType, typename _SimType>
struct JoinResult
{
	_IDType id1;
	_IDType id2;
	_SimType s;
};

typedef JoinResult<unsigned, double> JaccardJoinResult;
typedef JoinResult<unsigned, unsigned> EDJoinResult;

const int SUCCESS = 0;
const int FAILURE = 1;

class SimJoiner
{
private:
	vector<TString> m_str1, m_str2;
	IList m_list;
	vector<unsigned> m_sstr;
	unordered_map<string, Gram> m_map;
	vector<pair<unsigned, unsigned> > m_rawResultED;
	int m_len1, m_len2;
public:
	SimJoiner();
	~SimJoiner();
	void readFiles(const char *filename1, const char *filename2);
	void createInvertedListED(unsigned q, unsigned threshold);
	void preprocessED(const char *filename1, const char *filename2, unsigned q, unsigned threshold);
	void getResultED(unsigned q, unsigned threshold,  vector<int> &d0, vector<int> &d1, vector<EDJoinResult> &resultED);
	unsigned getED(TString &a, TString &b, unsigned threshold, vector<int> &d0, vector<int> &d1);
	int joinED(const char *filename1, const char *filename2, unsigned q, unsigned threshold, std::vector<EDJoinResult> &result);
};

