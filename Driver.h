// Copyright Varga Raimond 2019

#ifndef DRIVER_H_
#define DRIVER_H_
#include<string>
#include<iomanip>

class Driver {
 public:
	int current_pos;
	bool ready;
	int rating;
	float rating_med;
	std::string name;
	int nr_races;
	int nav_dist;
	Driver() {
		ready = true;
		rating = 0;
		nr_races = 0;
		nav_dist = 0;
		current_pos = -1;
		rating_med = 0;
	}
	void printInfo(std::ofstream& fout, std:: string position) {
		fout << std::fixed;
		fout << std::setprecision(3);
		fout << name << ": " << position << " " << rating_med << " ";
		fout << nr_races << " " << nav_dist << " ";
		if(ready) {
			fout << "online" << '\n';
		} else {
			fout << "offline" << '\n';
		}
	}

	void update(int new_pos, int races, int rating, int dist) {
		current_pos = new_pos;
		nr_races+= races;
		nav_dist += dist;
		this -> rating += rating;
		rating_med = static_cast<float>(this -> rating) / nr_races;
	}
};
#endif  // DRIVER_H_
