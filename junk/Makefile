CXX=clang++
STD=--std=c++11
EXECS=parser qtests
parser: parser.cpp
	$(CXX) $(STD) -o parser parser.cpp  -lQt5Core -fPIC -I/usr/include/qt5/QtCore -I/usr/include/qt5

qtests: qtests.cpp
	$(CXX) $(STD) -o qtests qtests.cpp  -lQt5Core -fPIC -I/usr/include/qt5/QtCore -I/usr/include/qt5

all: $(EXECS) 

clean:
	rm -fr $(EXECS) *.o *.a *~ *.*~
