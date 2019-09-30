// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_
#include<fstream>
#include<string>
#include<limits>
#include<vector>
#include<utility>
#include<list>
#include "./Graph.h"
#include "./Hashtable.h"
#include "./Driver.h"
#include "./sorted_pair_list.h"

class solver {
	Graph gr;
	Hashtable<std::string, int>hash;
	std::vector<std::string> positions;
	int m, n;
	std::vector<std::vector<int> > dist_matrix;
	std::vector<Driver> drivers;
	int nrDrivers;
	sorted_list<int> top_d, top_ri;
	sorted_list<float> top_ra;

 public:
	 	void read_data(std::ifstream& fin) {
			fin >> n >> m;
	 		gr = Graph(n, m);
	 		hash = Hashtable<std::string, int>(2 * n, &smart_hash);
			std::string name, start, end;
			for(int i = 0; i < n; i++) {
				fin >> name;
				hash.add(name, i);
				positions.push_back(name);
			}
			for(int i = 0; i < m; i++) {
				fin >> start >> end;
				gr.addEdge(hash.get(start), hash.get(end));
			}
	 	}
		void task1_solver(std::ifstream& fin, std::ofstream& fout) {
			int Q1;
			read_data(fin);
			std :: string s1, s2;
			fin >> Q1;
			for(int i = 1; i <= Q1; i++) {
				fin >> s1 >> s2;
				if(gr.minPath(hash.get(s1), hash.get(s2)) != -1) {
					fout << "y\n";
				} else {
					fout << "n\n";
				}
			 }
		}

		void task2_solver(std::ifstream& fin, std::ofstream& fout) {
			int Q2;
			fin >> Q2;
			std::string s1, s2;
			for(int i = 1; i <= Q2; i++) {
				fin >> s1 >> s2;
				fout << gr.minPath(hash.get(s1), hash.get(s2)) << '\n';
			}
		}

		void task3_solver(std::ifstream& fin, std::ofstream& fout) {
			char cq;
			int t_case, Q3, s1_index, s2_index;
			std::string s1, s2;
			fin >> Q3;
			for(int i = 1; i <= Q3; i++) {
				fin >> cq >> s1 >> s2 >> t_case;
				s1_index = hash.get(s1);
				s2_index = hash.get(s2);
				if(cq == 'c') {
					switch(t_case) {
						case 0:
							gr.addEdge(s1_index, s2_index);
							break;
						case 1:
							gr.removeEdge(s1_index, s2_index);
							gr.removeEdge(s2_index, s1_index);
							break;
						case 2:
							gr.addEdge(s1_index, s2_index);
							gr.addEdge(s2_index, s1_index);
							break;
						case 3:
							if(gr.hasEdge(s1_index, s2_index)) {
								if(!gr.hasEdge(s2_index, s1_index)) {
									gr.removeEdge(s1_index, s2_index);
									gr.addEdge(s2_index, s1_index);
								}
							} else {
								if(gr.hasEdge(s2_index, s1_index)) {
									gr.removeEdge(s2_index, s1_index);
									gr.addEdge(s1_index, s2_index);
								}
							}
							break;
					}
				}
				if(cq == 'q') {
					switch(t_case) {
						case 0:
							if(gr.minPath(s1_index, s2_index) != -1) {
								fout << "y\n";
							} else {
								fout << "n\n";
							}
							break;
						case 1:
							fout << gr.minPath(s1_index, s2_index) << '\n';
							break;
						case 2:
							std::string s3;
							fin >> s3;
							int s3_index = hash.get(s3);
							int dist13 = gr.minPath(s1_index, s3_index);
							int dist32 = gr.minPath(s3_index, s2_index);
							if(dist13 != -1 && dist32 != -1) {
								fout << dist13 + dist32 << '\n';
							} else {
								fout << dist32 << '\n';
							}
							break;
					}
				}
			}
			for(int i = 0; i < n; i++) {
				dist_matrix.push_back(gr.allDist(i));
			}
		}

		void task4_solver(std::ifstream& fin, std::ofstream& fout) {
			int Q4, idx;
			int nrD_print = 0;
			std::string q_type, position, name;
			Driver new_driver;
			nrDrivers = 0;
			fin >> Q4;
			for(int it = 1; it <= Q4; it++) {
				fin >> q_type;
				if(q_type == "d") {
					fin >> name >> position;
					if(!hash.has_key(name)) {
						new_driver = Driver();
						new_driver.name = name;
						new_driver.current_pos = hash.get(position);
						drivers.push_back(new_driver);
						hash.add(name, nrDrivers++);
						top_d.insert(std::make_pair(new_driver.name, 0));
						top_ri.insert(std::make_pair(new_driver.name, 0));
						top_ra.insert(std::make_pair(new_driver.name, 0));
					} else {
						idx = hash.get(name);
						drivers[idx].current_pos = hash.get(position);
						drivers[idx].ready = true;
					}
				}
				if(q_type == "b") {
					fin >> name;
					drivers[hash.get(name)].ready = false;
				}
				if(q_type == "r") {
					simulate_drive(fin, fout);
				}
				if(q_type == "info") {
					fin >> name;
					idx = hash.get(name);
					drivers[idx].printInfo(fout, positions[drivers[idx].current_pos]);
				}
				if(q_type == "top_rating") {
					fin >> nrD_print;
					top_ra.print(nrD_print, fout);
					// make_top(1, nrD_print, fout);
				}
				if(q_type == "top_dist") {
					fin >> nrD_print;
					top_d.print(nrD_print, fout);
					// make_top(2, nrD_print, fout);
				}
				if(q_type == "top_rides") {
					fin >> nrD_print;
					top_ri.print(nrD_print, fout);
					// make_top(3, nrD_print, fout);
				}
			}
		}

	void simulate_drive(std::ifstream& fin, std::ofstream& fout) {
		std::string start, end;
		int st_idx, end_idx, rating, min_idx = -1, pos;
		fin >> start >> end >> rating;
		st_idx = hash.get(start);
		end_idx = hash.get(end);
		// find the closest driver
		int min = std::numeric_limits<int>::max(), neighbour;
		min_idx = -1;
		for(unsigned int i = 0; i < drivers.size(); i++) {
			if(!drivers[i].ready) {
				continue;
			}
			pos = drivers[i].current_pos;
			if(dist_matrix[pos][st_idx] != -1) {
				if(dist_matrix[pos][st_idx] < min) {
					min_idx = i;
					min = dist_matrix[pos][st_idx];
				} else {
					if(dist_matrix[pos][st_idx] == min) {
						if(drivers[min_idx].rating_med < drivers[i].rating_med) {
							min_idx = i;
						} else {
							if(drivers[min_idx].rating_med == drivers[i].rating_med) {
								if(drivers[min_idx].name > drivers[i].name) {
									min_idx = i;
								}
							}
						}
					}
				}
			}
		}
		// check if we found available driver
		if(min == std::numeric_limits<int>::max()) {
			fout << "Soferi indisponibili\n";
		} else {
			if(dist_matrix[st_idx][end_idx] != -1) {
				drivers[min_idx].update(st_idx, 1, rating, min);
				drivers[min_idx].update(end_idx, 0, 0, dist_matrix[st_idx][end_idx]);
				top_ri.update(drivers[min_idx].nr_races, drivers[min_idx].name);
				top_d.update(drivers[min_idx].nav_dist, drivers[min_idx].name);
				top_ra.update(drivers[min_idx].rating_med, drivers[min_idx].name);
			} else {
				// find a neighbour we can reach
				neighbour = std::numeric_limits<int>::max();
				std::vector<int> neigh = gr.getNeigh(end_idx);
				for(auto i : neigh) {
					if(dist_matrix[st_idx][i] != -1) {
						neighbour = i;
						break;
					}
				}
				if(neighbour == std::numeric_limits<int>::max()) {
					fout << "Destinatie inaccesibila\n";
				} else {
					drivers[min_idx].update(st_idx, 1, rating, min);
					drivers[min_idx].update(neighbour, 0, 0, dist_matrix[st_idx][neighbour]);
					top_ri.update(drivers[min_idx].nr_races, drivers[min_idx].name);
					top_d.update(drivers[min_idx].nav_dist, drivers[min_idx].name);
					top_ra.update(drivers[min_idx].rating_med, drivers[min_idx].name);
				}
			}
		}
	}

	// void make_top(int top_type, int k, std::ofstream& fout) {
	// 	switch(top_type) {
	// 		case 1: {
	// 			sorted_list<float> top1;
	// 			std::pair<std::string, float> elem1;
	// 			for(unsigned int i = 0; i < drivers.size(); i++) {
	// 				elem1 = std::make_pair(drivers[i].name, drivers[i].rating_med);
	// 				top1.insert(elem1);
	// 			}
	// 			top1.print(k, fout);
	// 			break;
	// 		}
	// 		case 2: {
	// 			sorted_list<int> top2;
	// 			std::pair<std::string, int> elem2;
	// 			for(unsigned int i = 0; i < drivers.size(); i++) {
	// 				elem2 = std::make_pair(drivers[i].name, drivers[i].nav_dist);
	// 				top2.insert(elem2);
	// 			}
	// 			top2.print(k, fout);
	// 			break;
	// 		}
	// 		case 3: {
	// 			sorted_list<int> top3;
	// 			std::pair<std::string, int> elem3;
	// 			for(unsigned int i = 0; i < drivers.size(); i++) {
	// 				elem3 = std::make_pair(drivers[i].name, drivers[i].nr_races);
	// 				top3.insert(elem3);
	// 			}
	// 			top3.print(k, fout);
	// 			break;
	// 		}
	// 	}
	// }

	void task5_solver(std::ifstream& fin, std::ofstream& fout) {
		std::string name, str;
		int fuel, idx, nr, ok;
		std::list<std::string> dest;
		std::list<std::string> :: iterator it;
		fin >> fuel >> name >> nr;
		idx = drivers[hash.get(name)].current_pos;
		for(int i = 0; i < nr; i++) {
			fin >> str;
			if(dist_matrix[idx][hash.get(str)] != -1) {
				if(dist_matrix[idx][hash.get(str)] <=fuel) {
					ok = 0;
					for(it = dest.begin(); it != dest.end(); it++) {
						if(dist_matrix[idx][hash.get(str)] < dist_matrix[idx][hash.get(*it)]) {
							break;
						} else {
							if(dist_matrix[idx][hash.get(str)] == dist_matrix[idx][hash.get(*it)]) {
								if(str < *it) {
									break;
								} else {
									if(str == *it) {
										ok = 1;
									}
								}
							}
						}
					}
					if(ok == 0) {
						dest.insert(it, str);
					}
				}
			}
		}
		for(it = dest.begin(); it != dest.end(); it++) {
			fout << *it << ' ';
		}
		fout << '\n';
	}
};
#endif  // SOLVER_H_
