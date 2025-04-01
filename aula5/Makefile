CXX = g++
CXXFLAGS = -std=c++11 -Wall

target: etapa2

etapa2: lex.yy.o main.o symbols.o parser.tab.o
	$(CXX) $(CXXFLAGS) lex.yy.o main.o symbols.o parser.tab.o -o etapa2

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c

lex.yy.cpp: scanner.l
	flex -o lex.yy.cpp scanner.l

parser.tab.cpp: parser.ypp
	bison parser.ypp -d -Wcounterexamples

clean:
	rm -f etapa2 lex.yy.cpp parser.tab.cpp *.o 
