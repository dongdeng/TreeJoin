#include "PartJoin.h"

char D; // edit distance threshold
char PN; // part number

vector<string> dict; // original string
int MaxDictLen = 0;
int MinDictLen = 0x7FFFFFFF;
int N; // data number

void readDistance(char *dis) {
	D = atoi(dis); 
  PN = D + 1;
	fprintf(stderr, "#     Edit Distance : %d\n", D);
	fprintf(stderr, "#   Number of Parts : %d\n", PN);
}

void readData(char *filename) {
  string entity;
  std::ifstream dataFile(filename, std::ios::in);
  while (getline(dataFile, entity)) {
    if (MaxDictLen < static_cast<int>(entity.length()))
      MaxDictLen = entity.length();
    if (MinDictLen > static_cast<int>(entity.length()))
      MinDictLen = entity.length();
    dict.push_back(entity);
  }
  N = dict.size();
  dataFile.close();
	fprintf(stderr, "#       Data Number : %d\n", N);
}
