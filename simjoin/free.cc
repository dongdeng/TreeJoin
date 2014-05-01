#include "PartJoin.h"

extern char PN;
extern int MaxDictLen;
extern char **matrix;
extern char **_matrix;
extern bool *quickRef;
extern int **partLen;
extern int *dist;
extern int *comLen;
extern uint64_t *power;
extern int **partPos;
extern InvLists **invLists;
extern vector<PIndex> **partIndex;


void freeAll() {
  for (int lp = 0; lp < PN; lp++) {
    delete[] partPos[lp];
    delete[] partLen[lp];
    delete[] invLists[lp];
    delete[] partIndex[lp];
  }
  delete[] partPos[PN];
  
  for (int lp = 0; lp <= MaxDictLen; lp++) {
    delete[] matrix[lp];
    delete[] _matrix[lp];
  }

  delete[] matrix;
  delete[] _matrix;
  delete[] dist;
  delete[] comLen;
  delete[] power;
  delete[] partPos;
  delete[] partLen;
  delete[] invLists;
  delete[] partIndex;
  delete[] quickRef;
}
