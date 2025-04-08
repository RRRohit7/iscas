all: iscas

iscas: project.cpp
	g++ -g -o iscas project.cpp

clean:
	rm -f iscas
