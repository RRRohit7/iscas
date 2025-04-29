all: iscas

iscas: project.cpp
	g++ -std=c++17 -g -o iscas project.cpp

clean:
	rm -f iscas
