#include <iostream>
#include "tree.h"
#include <vector>
#include <cstring>
#include <fstream>
#include "SimJoiner.h"

using namespace std;

#define LIMIT 20

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

string getPostorderedString(TreeNode *root) {
	string ret = "";
	for (auto & i : root->child)
		ret.append(getPostorderedString(i) + "$");
	ret.append(root->label);
	return ret;
}

int dfs(TreeNode *f1, TreeNode *f2, int **ans, int sum1, int sum2, int threshold);

int treeED(TreeNode *f1, TreeNode *f2, int threshold) {
	int n = f1->sum, m = f2->sum;
	if (abs(n - m) > threshold)
		return LIMIT + 1;
	int **ans = new int*[n];
	for (int i = 0; i < n; ++i) {
		ans[i] = new int[m];
		memset(ans[i], -1, sizeof(int) * m);
	}
	dfs(f1, f2, ans, 0, 0, threshold);
	int ret = ans[0][0];
	for (int i = 0; i < n; ++i)
		delete ans[i];
	delete [] ans;
	return ret;
}

int dfs(TreeNode *f1, TreeNode *f2, int **ans, int sum1, int sum2, int threshold) {
	if (f1 == NULL && f2 == NULL)
		return 0;
	if (ans[sum1][sum2] != -1)
		return ans[sum1][sum2];
	if (threshold < 0)
		return LIMIT + 1;
	if (f1->getSize() == 0 || f2->getSize() == 0)
		return max(f1->getSize(), f2->getSize());
	if (abs(f1->sum - sum1 - f2->sum + sum2) > threshold)
		return LIMIT + 1;
	int ret = LIMIT + 1;
	//cout << "0" << endl;
	if (f1->getSize() > 0) {
		int temp = f1->getSize();
		TreeNode *v = f1->deleteRightmostChild();
		string s = "";
		ret = min(ret, dfs(f1, f2, ans, sum1 + 1, sum2, threshold - costFunc(v->label, s)) + costFunc(v->label, s));
		while (f1->getSize() > temp - 1)
			f1->deleteRightmostTree();
		f1->insertChild(v);
	}
	//cout << "1" << endl;
	if (f2->getSize() > 0) {
		int temp = f2->getSize();
		TreeNode *w = f2->deleteRightmostChild();
		string s = "";
		ret = min(ret, dfs(f1, f2, ans, sum1, sum2 + 1, threshold- costFunc(s, w->label)) + costFunc(s, w->label));
		while (f2->getSize() > temp - 1)
			f2->deleteRightmostTree();
		f2->insertChild(w);
	}
	//cout << "2" << endl;
	if (f1->getSize() > 0 && f2->getSize() > 0) {
		TreeNode *v = f1->deleteRightmostTree();
		TreeNode *w = f2->deleteRightmostTree();
		int distance = treeED(v, w, threshold);
		ret = min(ret, distance + dfs(f1, f2, ans, sum1 + v->sum, sum2 + w->sum, threshold - distance - costFunc(v->label, w->label)) + costFunc(v->label, w->label));
		f1->insertChild(v);
		f2->insertChild(w);
	}
	ans[sum1][sum2] = ret;
	return ret;
}

int generatePostorderedString(TreeNode *root, const char *filename) {
	cout << "start generating postordered string" << endl;
	ofstream fout(filename);
	int count = 0;
	for (auto & i : root->child) {
		string temp = getPostorderedString(i);
		if (temp != "" && temp[0] != '$') {
			fout << temp << endl;
			++count;
		}
		i->postString = temp;
	}
	fout.close();
	cout << "geneating finished" << endl;
	return count;
}

void findSimilarityJoin(int edThreshold, vector<EDJoinResult> &resultED) {
	SimJoiner joiner;
	unsigned q = 10;
	//double jaccardThreshold = 0.8;
	joiner.joinED("strings.txt", "strings.txt", q, edThreshold, resultED);
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
	f1->calc();
	f2->calc();

	int n = generatePostorderedString(f1, "strings.txt");
	cout << "totalNum = " << n << endl;

	for (int i = 1; i <= 20; ++i) {
		int edThreshold = i;
		cout << "the threshold = " << i << endl;
		vector<EDJoinResult> resultED;
		clock_t begin = clock()	;
		findSimilarityJoin(edThreshold, resultED);
		clock_t end = clock();
		cout << "the result of PassJoin = " << resultED.size() << endl;
		cout << "the time of PassJoin = " << (end - begin) / CLOCKS_PER_SEC << endl;
		vector<pair<int, int> > result;
		begin = clock();
		for (auto & i : resultED) {
			if (i.id1 == i.id2 || treeED(f1->child[i.id1], f2->child[i.id2], edThreshold) <= edThreshold) {
				result.push_back(make_pair(i.id1, i.id2));
				//cout << i.id1 << ' ' << i.id2 << endl;
			}
		}
		end = clock();
		cout << "the result of TreeED = " << result.size() << endl;
		cout << "the time of TreeED = " << (end - begin) / CLOCKS_PER_SEC << endl;
		cout << "-----------------------------------------------------" << endl;
	}
}
