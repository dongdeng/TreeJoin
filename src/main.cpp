#include <iostream>
#include "tree.h"
#include <vector>
#include <cstring>

using namespace std;

int getMin(int a, int b) {
	if (a == -1)
		return b;
	if (b == -1)
		return a;
	return a > b ? b : a;
}

int costFunc(const string &a, const string &b) {
	if (a == b)
		return 0;
	return 1;	
}

int dfs(TreeNode *f1, TreeNode *f2, int **ans, int sum1, int sum2);

int treeED(TreeNode *f1, TreeNode *f2) {
	int n = f1->getSum(), m = f2->getSum();
	int **ans = new int*[n];
	for (int i = 0; i < n; ++i) {
		ans[i] = new int[m];
		memset(ans[i], -1, sizeof(int) * m);
	}
	dfs(f1, f2, ans, 0, 0);
	int ret = ans[0][0];
	delete ans;
	return ret;
}

int dfs(TreeNode *f1, TreeNode *f2, int **ans, int sum1, int sum2) {
	if (f1 == NULL && f2 == NULL)
		return 0;
	if (ans[sum1][sum2] != -1)
		return ans[sum1][sum2];
	int ret = -1;
	//cout << "0" << endl;
	if (f1->getSize() > 0) {
		int temp = f1->getSize();
		TreeNode *v = f1->deleteRightmostChild();
		string s = "";
		ret = getMin(ret, dfs(f1, f2, ans, sum1 + 1, sum2) + costFunc(v->getLabel(), s));
		while (f1->getSize() > temp - 1)
			f1->deleteRightmostTree();
		f1->insertChild(v);
	}
	//cout << "1" << endl;
	if (f2->getSize() > 0) {
		int temp = f2->getSize();
		TreeNode *w = f2->deleteRightmostChild();
		string s = "";
		ret = getMin(ret, dfs(f1, f2, ans, sum1, sum2 + 1) + costFunc(s, w->getLabel()));
		while (f2->getSize() > temp - 1)
			f2->deleteRightmostTree();
		f2->insertChild(w);
	}
	//cout << "2" << endl;
	if (f1->getSize() > 0 && f2->getSize() > 0) {
		TreeNode *v = f1->deleteRightmostTree();
		TreeNode *w = f2->deleteRightmostTree();
		ret = getMin(ret, treeED(v, w) + dfs(f1, f2, ans, sum1 + v->getSum(), sum2 + w->getSum()) + costFunc(v->getLabel(), w->getLabel()));
		f1->insertChild(v);
		f2->insertChild(w);
	}
	if (ret == -1)
		ret = 0;
	ans[sum1][sum2] = ret;
	return ret;
}

int main(int argc, char **argv) {
	if (argc < 3) {
		cout << "Usage : " << endl;
		cout << "main filename1 filename2" << endl;
		return 0;
	}
	cout << "reading..." << endl;
	TreeNode *f1 = new TreeNode();
	f1->readFile(argv[1]);
	TreeNode *f2 = new TreeNode();
	f2->readFile(argv[2]);
	cout << "reading finished" << endl;
	cout << treeED(f1, f2) << endl;
}