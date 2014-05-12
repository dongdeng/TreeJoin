#include "Gram.h"
#include <algorithm>
#include <iostream>

using namespace std;

bool pairCompare(const pair<unsigned, unsigned> &a, const pair<unsigned, unsigned> &b) {
	return a.first == b.first ? a.second < b.second : a.first < b.first;
}

Gram::Gram()
{
}

Gram::~Gram()
{
}

void Gram::insert(unsigned no, unsigned pos)
{
	m_list.push_back(make_pair(no, pos));
}

void Gram::sort()
{
	std::sort(m_list.begin(), m_list.end(), pairCompare);
}

vector<pair<unsigned, unsigned> > &Gram::getList()
{
	return m_list;
}

int Gram::size()
{
	return m_list.size();
}
