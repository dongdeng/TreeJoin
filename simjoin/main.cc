#include "PartJoin.h"
#include <sys/time.h>

extern uint64_t realNum;
extern uint64_t candNum;

int main(int argc, char **argv) {
  readDistance(argv[1]);  // read in D and PartNum;
  timeval begin, end;

  // dump dictionary
  gettimeofday(&begin, NULL);
  readData(argv[2]);
  init();
  gettimeofday(&end, NULL);
  fprintf(stderr, "#     Allocate Time : %.3fs\n", end.tv_sec - begin.tv_sec +
      (end.tv_usec - begin.tv_usec) * 1.0 / CLOCKS_PER_SEC);

  gettimeofday(&begin, NULL);
  sortData();
  gettimeofday(&end, NULL);
  fprintf(stderr, "#    Sort Data Time : %.3fs\n", end.tv_sec - begin.tv_sec +
      (end.tv_usec - begin.tv_usec) * 1.0 / CLOCKS_PER_SEC);
  
  gettimeofday(&begin, NULL);
  prepare();
  gettimeofday(&end, NULL);
  fprintf(stderr, "#  Pre-Process Time : %.3fs\n", end.tv_sec - begin.tv_sec +
      (end.tv_usec - begin.tv_usec) * 1.0 / CLOCKS_PER_SEC);

  gettimeofday(&begin, NULL);
  join();
  gettimeofday(&end, NULL);

  fprintf(stderr, "#      Process Time : %.3fs\n", end.tv_sec - begin.tv_sec +
      (end.tv_usec - begin.tv_usec) * 1.0 / CLOCKS_PER_SEC);
  // fprintf(stderr, "#    Candidates Num : %lld\n", candNum);
  fprintf(stderr, "#  Real Results Num : %lld\n", realNum);

  freeAll();
  return 0;
}
