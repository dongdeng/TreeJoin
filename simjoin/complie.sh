g++ -O3 -c -DNDEBUG -o free.o free.cc
g++ -O3 -c -DNDEBUG -o io.o io.cc
g++ -O3 -c -DNDEBUG -o main.o main.cc
g++ -O3 -c -DNDEBUG -o pre.o pre.cc
g++ -O3 -c -DNDEBUG -o join.o join.cc
g++ -O3 -o PassJoin free.o io.o main.o pre.o join.o
