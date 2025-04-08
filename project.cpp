#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
using namespace std;

void trim(string& str){
	 str.erase(remove(str.begin(), str.end(), ' '), str.end());
}

void addNodesToAdjList(string input, string output, unordered_map<string, vector<pair<string, int>>>& adjList){
	stringstream ss(input);
	string token;
	while(getline(ss, token, ',')){
		for(pair<string, int>& pair : adjList[token])
			pair.second++;
		adjList[output].push_back({token, 0});
	}
}
void parser(string filename){
	vector<string> inputs;
	vector<string> outputs;
	unordered_map<string, vector<pair<string, int>>> adjList;
	ifstream file(filename);
	string line;
	while(getline(file, line)){
		trim(line);
		if(line.empty() || line[0] == '#')
			continue;
		else if(line.find("INPUT(") == 0){
			size_t start = line.find("(");
			size_t end = line.find(")");
			string sub_str = line.substr(start + 1, end - start - 1);
			inputs.push_back(sub_str);
			cout << "INPUT: " << sub_str << endl;			
		}
		else if(line.find("OUTPUT(") == 0){
			size_t start = line.find("(");
			size_t end = line.find(")");
			string sub_str = line.substr(start + 1, end - start - 1);
			outputs.push_back(sub_str);
			cout << "OUTPUT: " << sub_str << endl;
		}
		else if(line.find_first_of("G") == 0){
			string output = line.substr(0, line.find("="));
			size_t start = line.find("(");
			size_t end = line.find(")");
			string input_substr = line.substr(start + 1, end -start - 1);
			addNodesToAdjList(input_substr, output, adjList);
			cout << "GATE:: input: " << input_substr << " output: " << output << endl; 
		}
		else {
			cout << "SOMETHING SERIOUSLY WRONG WITH THE BENCHMARK FILE!!! PLEASE VALIDATE!!!" << endl;
			break;
		}
	}
	for(auto& [node, vector] : adjList){
		cout<< "Node: " << node << " Neighbours: ";
		for(auto& [neighbour, weight] : vector){
			cout << neighbour << " weight: " << weight << " ";
		}
		cout << "" << endl;
	}
}
int main(int argc, char* argv[]){
	parser(argv[1]);
	return 0;
}
