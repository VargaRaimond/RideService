// Copyright Varga Raimond 2019

#ifndef GRAPH_H_
#define GRAPH_H_
#include<vector>
#include<queue>
#include<limits>


class Graph{
	int nrNodes;
	int nrRoads;
	std::vector<std::vector<int> > neigh;

 public:
	Graph(int nrNodes, int nrRoads) {
		this -> nrNodes = nrNodes;
		this -> nrRoads = nrRoads;
		for(int i = 0; i < nrNodes; i++) {
			neigh.push_back(std::vector<int>());
		}
	}
	Graph() {
		nrNodes = 0;
		nrRoads = 0;
	}
	Graph& operator=(const Graph& other) {
		nrNodes = other.nrNodes;
		nrRoads = other.nrRoads;
		for(int i = 0; i < nrNodes; i++) {
			neigh.push_back(other.neigh[i]);
		}
		return *this;
	}
	void addEdge(int n1, int n2) {
		if(!hasEdge(n1, n2) && n1 != n2) {
			neigh[n1].push_back(n2);
		}
	}
	bool hasEdge(int n1, int n2) {
		std::vector<int> temp = neigh[n1];
		for(unsigned int i = 0; i < temp.size(); i++) {
			if(temp[i] == n2) {
				return true;
			}
		}
		return false;
	}
	void removeEdge(int n1, int n2) {
		std :: vector<int> :: iterator it;
		if(hasEdge(n1, n2)) {
			for(it = neigh[n1].begin(); it != neigh[n1].end(); it++) {
				if(*it == n2) {
					neigh[n1].erase(it);
					break;
				}
			}
		}
	}

	std::vector<int> getNeigh(int n) {
		return neigh[n];
	}

	int minPath(int src, int dst) {
		if(src == dst) {
			return 0;
		}
		std:: queue<int> Q;
		int max = std:: numeric_limits<int>::max();
		std::vector<int> dist(nrNodes, max);
        Q.push(src);
        dist[src] = 0;
		while(!Q.empty()) {
            auto node = Q.front();
            Q.pop();
            for (auto n : neigh[node]) {
            	if(dist[n] == max) {
            		Q.push(n);
            		dist[n] = dist[node] + 1;
            	}
            	if(n == dst) {
                	while(!Q.empty()) {
                  		Q.pop();
                	}
                	return dist[dst];
                }
            }
        }
        return -1;
	}
	std::vector<int> allDist(int src) {
		std::queue<int> Q;
		std::vector<int> dist(nrNodes, -1);
        dist[src] = 0;
        Q.push(src);
        while (!Q.empty()) {
            auto node = Q.front();
            Q.pop();
            for (auto n : neigh[node]) {
                if (dist[n] == -1) {
                    Q.push(n);
                    dist[n] = dist[node] + 1;
                }
            }
        }
        return dist;
	}
};


#endif  // GRAPH_H_
