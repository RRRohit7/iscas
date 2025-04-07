all: iscas

iscas: project.cpp
	g++ -o iscas project.cpp

clean:
	rm -f iscas
