#ifndef GRAM_H
#define GRAM_H

#include <vector>

using namespace std;

class Gram
{
private:
	vector<pair<unsigned, unsigned> > m_list;
public:
	Gram();
	~Gram();
	void insert(unsigned no, unsigned pos);
	void sort();
	vector<pair<unsigned, unsigned> > &getList();
	int size();
};


#endif
