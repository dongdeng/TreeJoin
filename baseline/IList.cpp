#include "IList.h"

IList::IList() {
}

IList::~IList() {
}

void IList::insert(unsigned no, unsigned length, unsigned pos, string &s) {
	if(m_map.size() < length + 1)
		m_map.resize(length + 1);
	if(m_map[length].size() < pos + 1)
		m_map[length].resize(pos + 1);
	if(m_map[length][pos].find(s) == m_map[length][pos].end()) {
		vector<int> v;
		v.push_back(no);
		m_map[length][pos][s] = v;
	} 
	else
		m_map[length][pos][s].push_back(no);
}

vector<vector<unordered_map<string, vector<int> > > > &IList::getList() {
	return m_map;
}