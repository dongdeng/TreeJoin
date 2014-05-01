#ifndef TREE_H
#define TREE_H

#include <list>
#include <iostream>

using namespace std;

class TreeNode {
private:	
	string m_label;
	list<TreeNode*> m_child;
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
	list<TreeNode*> &getChild();
};

#endif