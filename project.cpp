#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
#include<queue>
#include<climits>
#include<cstdlib>

using namespace std;

vector<string> inputs;
vector<string> outputs;

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
unordered_map<string, vector<pair<string, int>>> parser(string filename){
	unordered_map<string, vector<pair<string, int>>> adjList;
	ifstream file(filename);
	string line;
	if(!file){
		cout << "Wrong file name" << endl;
		exit(0);
	}
	else {
		while(getline(file, line)){
			trim(line);
			if(line.empty() || line[0] == '#')
				continue;
			else if(line.find("INPUT(") == 0){
				size_t start = line.find("(");
				size_t end = line.find(")");
				string sub_str = line.substr(start + 1, end - start - 1);
				inputs.push_back(sub_str);
				//cout << "INPUT: " << sub_str << endl;			
			}
			else if(line.find("OUTPUT(") == 0){
				size_t start = line.find("(");
				size_t end = line.find(")");
				string sub_str = line.substr(start + 1, end - start - 1);
				outputs.push_back(sub_str);
				//cout << "OUTPUT: " << sub_str << endl;
			}
			else if(line.find_first_of("G") == 0){
				string output = line.substr(0, line.find("="));
				size_t start = line.find("(");
				size_t end = line.find(")");
				string input_substr = line.substr(start + 1, end -start - 1);
				addNodesToAdjList(input_substr, output, adjList);
				// cout << "GATE:: input: " << input_substr << " output: " << output << endl; 
			}	
			else {
				cout << "SOMETHING SERIOUSLY WRONG WITH THE BENCHMARK FILE!!! PLEASE VALIDATE!!!" << endl;
				exit(0);
			}

		}
	}
	/*for(auto& [node, vector] : adjList){
		cout<< "Node: " << node << " Neighbours: ";
		for(auto& [neighbour, weight] : vector){
			cout << neighbour << " weight: " << weight << " ";
		}
		cout << "" << endl;
	} */
	return adjList;
}

int dijkstra_shortest_path(string input, string output, unordered_map<string, vector<pair<string, int>>>& adjList){
	priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> prior_q;
	unordered_map<string, int> distance;
	for(auto& [node, vector] : adjList) {
		distance[node] = INT_MAX;
	}
	distance[input] = 0;
	prior_q.push({0, input});
	while(!prior_q.empty()){
		string min_node = prior_q.top().second;
		prior_q.pop();
		for(auto& [neighbour, weight] : adjList[min_node]){
			if(distance[neighbour] > distance[min_node] + weight){
				distance[neighbour] = distance[min_node] + weight;
				prior_q.push({distance[neighbour], neighbour});
			}
		}
	}
	return distance[output];
}

int main(int argc, char* argv[]){
	unordered_map<string, vector<pair<string, int>>> adjList;
	adjList = parser(argv[1]);
	int short_path = dijkstra_shortest_path(argv[3], argv[2], adjList);
	cout << "Shortest path: " << short_path << endl;
	return 0;
}
