#ifndef ILIST_H
#define ILIST_H

#include <vector>
#include <unordered_map>

using namespace std;

class IList {
private:
	vector<vector<unordered_map<string, vector<int> > > > m_map;
public:
	IList();
	~IList();
	void insert(unsigned no, unsigned length, unsigned pos, string &s);
	vector<vector<unordered_map<string, vector<int> > > > &getList();
};

#endif