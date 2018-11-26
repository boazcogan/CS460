P2.out : Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o
	g++ -g -o P2.out Project2.o SetLimits.o LexicalAnalyzer.o SyntacticalAnalyzer.o

Project2.o : Project2.cpp SetLimits.h SyntacticalAnalyzer.h
	g++ -g -c Project2.cpp

SetLimits.o : SetLimits.cpp SetLimits.h
	g++ -g -c SetLimits.cpp

LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -g -c LexicalAnalyzer.cpp

SyntacticalAnalyzer.o : SyntacticalAnalyzer.cpp SyntacticalAnalyzer.h LexicalAnalyzer.h
	g++ -g -c SyntacticalAnalyzer.cpp

clean : 
	rm *.o P2.out *.gch

submit : Project2.cpp LexicalAnalyzer.h LexicalAnalyzer.cpp SyntacticalAnalyzer.h SyntacticalAnalyzer.cpp makefile README.txt
	rm -rf TeamSP2
	mkdir TeamSP2
	cp Project2.cpp TeamSP2
	cp LexicalAnalyzer.h TeamSP2
	cp LexicalAnalyzer.cpp TeamSP2
	cp SyntacticalAnalyzer.h TeamSP2
	cp SyntacticalAnalyzer.cpp TeamSP2
	cp makefile TeamSP2
	cp README.txt TeamSP2
	tar cfvz TeamSP2.tgz TeamSP2
	cp TeamSP2.tgz ~tiawatts/cs460drop
