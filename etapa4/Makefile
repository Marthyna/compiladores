# UFRGS - Marthyna Weber - 2025-1
#
# Makefile
# Código desenvolvido em sala de aula pelo professor Marcelo Johann.

CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -O0

target: etapa4

etapa4: lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o
	$(CXX) $(CXXFLAGS) lex.yy.o main.o symbols.o parser.tab.o ast.o semantic.o -o etapa4

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c

lex.yy.cpp: scanner.l parser.tab.hpp
	flex -o lex.yy.cpp scanner.l

parser.tab.cpp parser.tab.hpp: parser.ypp
	bison parser.ypp -d

semantic.o: semantic.cpp parser.tab.hpp
	$(CXX) $(CXXFLAGS) semantic.cpp -c

clean:
	rm -f etapa4 lex.yy.cpp parser.tab.cpp *.o 
