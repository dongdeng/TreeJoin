#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class TreeNode {
private:	
	string m_label, m_eulerString;
	TreeNode *m_father;
	vector<TreeNode*> m_child;
	TreeNode *read(ifstream &fin);
public:
	TreeNode();
	TreeNode(string label);
	~TreeNode();
	void readFile(char *filename);
	void setLabel(string label);
	string getLabel();
	bool insertChild(TreeNode *child);
	TreeNode *deleteRightmostTree();
	TreeNode *deleteRightmostChild();
	int getSize();
	int getSum();
	vector<TreeNode*> &getChild();
	void calcEulerString();
	string &getEulerString();
	void setFather(TreeNode *father);
	TreeNode *getFather();
};

#endif