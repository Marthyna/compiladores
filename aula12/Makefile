# UFRGS - Marthyna Weber - 2025-1
#
# Makefile
# Código desenvolvido em sala de aula pelo professor Marcelo Johann.

CXX = g++
CXXFLAGS = -std=c++11 -Wall

target: etapa2

etapa2: lex.yy.o main.o symbols.o parser.tab.o ast.o
	$(CXX) $(CXXFLAGS) lex.yy.o main.o symbols.o parser.tab.o ast.o -o etapa2

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c

lex.yy.cpp: scanner.l parser.tab.hpp
	flex -o lex.yy.cpp scanner.l

parser.tab.cpp parser.tab.hpp: parser.ypp
	bison parser.ypp -d

clean:
	rm -f etapa2 lex.yy.cpp parser.tab.cpp *.o 
