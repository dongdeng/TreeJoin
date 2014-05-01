g++ -g -c -o free.o free.cc
g++ -g -c -o io.o io.cc
g++ -g -c -o main.o main.cc
g++ -g -c -o pre.o pre.cc
g++ -g -c -o join.o join.cc
g++ -g -o simJoin free.o io.o main.o pre.o join.o
