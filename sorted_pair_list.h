// Copyright Varga Raimond 2019

#ifndef SORTED_PAIR_LIST_H_
#define SORTED_PAIR_LIST_H_
#include<list>
#include<utility>
#include<iomanip>
#include<type_traits>
#include<string>

template <typename T>
class sorted_list {
	std::list<std::pair<std::string, T> > my_list;

 public:
	void insert(std::pair<std::string, T> elem) {
		typename std::list<std::pair<std::string, T> > :: iterator it;
		for(it = my_list.begin(); it != my_list.end(); it++) {
			if(elem.second > it -> second) {
				break;
			} else {
				if(elem.second == it -> second) {
					if(elem.first < it -> first) {
						break;
					}
				}
			}
		}
		my_list.insert(it, elem);
	}

	void print(int k, std::ofstream& fout) {
		if(my_list.empty()) {
			fout << '\n';
			return;
		}
		typename std::list<std::pair<std::string, T> > :: iterator it;
		for(it = my_list.begin(); it != my_list.end(); it++) {
			fout << it -> first << ':';
			if(std::is_same<T, float>::value) {
				fout << std::fixed;
				fout << std::setprecision(3);
				fout << it -> second << ' ';
			} else {
				fout << it -> second << ' ';
			}
			k--;
			if(k == 0) {
				break;
			}
		}
		fout << '\n';
	}

	void update(T new_val, std::string name) {
		typename std::list<std::pair<std::string, T> > :: iterator it;
		for(it = my_list.begin(); it != my_list.end(); it++) {
			if(it -> first == name) {
				my_list.erase(it);
				insert(std::make_pair(name, new_val));
				break;
			}
		}
	}
};
#endif  // SORTED_PAIR_LIST_H_
