#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>

using namespace std;

string trim(string str){
	 str.erase(remove(str.begin(), str.end(), ' '), str.end());
	 return str;
}
void parser(string filename){
	ifstream file(filename);
	string line;
	while(getline(file, line)){
		if(line.empty() || line[0] == '#')
			continue;
		cout << trim(line) <<endl;
	}
}
int main(int argc, char* argv[]){
	parser(argv[1]);
	return 0;
}
