#include "tree.h"
#include <fstream>
#include <vector>

using namespace std;

TreeNode::TreeNode() {
}

TreeNode::TreeNode(string label) {
	m_label = label;
}

TreeNode::~TreeNode() {
}

void TreeNode::readFile(char *filename) {
	ifstream fin(filename);
	int n; //the number of roots
	fin >> n;
	for (int i = 0; i < n; ++i) {
		int m; // the number of nodes of each tree
		fin >> m;
		vector<TreeNode*> temp;
		for (int j = 0; j < m; ++j) {
			string s;
			fin >> s;
			temp.push_back(new TreeNode(s));
		}
		for (int j = 0; j < m - 1; ++j) {
			int x, y;
			fin >> x >> y;
			temp[x]->insertChild(temp[y]);
		}
		insertChild(temp[0]);
	}
}

void TreeNode::setLabel(string label) {
	m_label = label;
}

string TreeNode::getLabel() {
	return m_label;
}

bool TreeNode::insertChild(TreeNode *child) {
	m_child.push_back(child);
	return true;
}

TreeNode *TreeNode::deleteRightmostTree() {
	if (m_child.empty())
		return NULL;
	TreeNode *ret = m_child.back();
	m_child.pop_back();
	return ret;
}

TreeNode *TreeNode::deleteRightmostChild() {
	TreeNode *rightmostChild = deleteRightmostTree();
	if (rightmostChild == NULL)
		return NULL;
	list<TreeNode*> &childList = rightmostChild->getChild();
	for (auto & i : childList)
		m_child.push_back(i);
	return rightmostChild;
}

int TreeNode::getSize() {
	return m_child.size();
}

list<TreeNode*> &TreeNode::getChild() {
	return m_child;
}