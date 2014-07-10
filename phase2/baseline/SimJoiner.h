	#pragma once
#include <vector>
#include <unordered_map>
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
	vector<pair<unsigned, unsigned> > m_rawResultED;
	int m_len1, m_len2;
public:
	SimJoiner();
	~SimJoiner();
	void readFiles(const char *filename1, const char *filename2);
	void createInvertedListED(unsigned q, unsigned threshold);
	void preprocessED(const char *filename1, const char *filename2, unsigned q, unsigned threshold);
	void getResultED(unsigned q, unsigned threshold,  vector<EDJoinResult> &resultED);
	int getED(TString &a, TString &b, int threshold);
	int joinED(const char *filename1, const char *filename2, unsigned q, unsigned threshold, std::vector<EDJoinResult> &result);
};

