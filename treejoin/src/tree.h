#ifndef TREE_H
#define TREE_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class TreeNode {
public:	
	string label, eulerString, postString;
	TreeNode *father;
	vector<TreeNode*> child;
	int anc, left, right, sum;

	TreeNode();
	TreeNode(string l);
	~TreeNode();
	TreeNode *read(ifstream &fin);
	void readFile(char *filename);
	bool insertChild(TreeNode *c);
	TreeNode *deleteRightmostTree();
	TreeNode *deleteRightmostChild();
	int getSize();
	void calcSum();
	void calcEulerString();
	void calcALR();
	void calc();
};

#endif