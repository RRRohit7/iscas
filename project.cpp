#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<algorithm>
#include<vector>
#include<queue>
#include<climits>
#include<cstdlib>
#include<unordered_map>


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

int distance_between_nodes(string node1, string node2, unordered_map<string, vector<pair<string, int>>>& adjList){
	for(auto& [node, dist] : adjList[node1]){
		if(node == node2)
			return dist;
	}
	return INT_MAX;
}

int dijkstra_shortest_path(string input, string output, unordered_map<string, vector<pair<string, int>>>& adjList){
	priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> prior_q;
	unordered_map<string, pair<int, string>> distance;
	for(auto& [node, vector] : adjList) {
		distance[node].first = INT_MAX;
	}
	distance[input].first = 0;
	distance[input].second = input;
	prior_q.push({0, input});
	while(!prior_q.empty()){
		string min_node = prior_q.top().second;
		prior_q.pop();
		for(auto& [neighbour, weight] : adjList[min_node]){
			if(distance[neighbour].first > distance[min_node].first + weight){
				distance[neighbour].first = distance[min_node].first + weight;
				distance[neighbour].second = min_node;
				prior_q.push({distance[neighbour].first, neighbour});
			}
		}
	}
	string temp = output;
	while(distance[temp].first != INT_MAX && temp != input){
		string nextnode = distance[temp].second;
		int dist_nextnode = distance_between_nodes(nextnode, temp, adjList);
		cout << temp << " --" << dist_nextnode << "--> ";
		if(nextnode == input){
			cout << nextnode << endl;
			break;
		}
		else { temp = nextnode; }
	}
	return distance[output].first;
}


int main(int argc, char* argv[]){
	unordered_map<string, vector<pair<string, int>>> adjList;
	if(argc != 4){
		cout << "Incorrect number of arguments." << endl;
		return 0;
	} 
	adjList = parser(argv[1]);
	bool invalid_input = find(inputs.begin(), inputs.end(), argv[2]) == inputs.end() ? true : false;
	bool invalid_output = find(outputs.begin(), outputs.end(), argv[3]) == outputs.end() ? true : false;
	if(invalid_input || invalid_output){
		if(invalid_input){
			if(find(outputs.begin(), outputs.end(), argv[2]) == outputs.end()){
				cout << "Signal " << argv[2] << " not found in " << argv[1] << endl;
			}
			else{
				cout << "Signal " << argv[2] << " is not an input pin" << endl;
			}
		}
		if(invalid_output){
			if(find(inputs.begin(), inputs.end(), argv[3]) == inputs.end()){
				cout << "Signal " << argv[3] << " not found in " << argv[1] << endl;
			}
			else{
				cout << "Signal " << argv[3] << " is not an output pin" << endl;
			}
		}
	return 0;
	}
	int short_path = dijkstra_shortest_path(argv[3], argv[2], adjList);
	if(short_path != INT_MAX)
		cout << "Path Length: " << short_path << endl;
	else
		cout << "No path found from " << argv[2] << " to " << argv[3] <<  endl;

	return 0;
}
