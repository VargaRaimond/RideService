// Copyright Varga Raimond 2019

#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include<list>
#include<iostream>
#include<string>
#include<limits>

template <typename Tkey, typename Tvalue>
class info {
 public:
	Tkey key;
	Tvalue value;
};

template <typename Tkey, typename Tvalue>
class Hashtable {
	std :: list <info<Tkey, Tvalue> > *H;
	int size;
	int capacity;
	int (*Hfunction)(Tkey);

 public:
	Hashtable(int capacity, int (*Hf)(Tkey)) {
		H = new std :: list <info<Tkey, Tvalue> >[capacity];
		this -> capacity = capacity;
		size = 0;
		Hfunction = Hf;
	}
	~Hashtable() {
		delete[] H;
	}
	Hashtable() {
		size = 0;
		capacity = 0;
	}

	Hashtable &operator=(const Hashtable &other) {
		size = other.size;
		capacity = other.capacity;
		Hfunction = other.Hfunction;
		H = new std :: list <info<Tkey, Tvalue> >[capacity];
		return *this;
	}

	void add(Tkey key, Tvalue value) {
		bool found = false;
		int pos = Hfunction(key) % capacity;
		for(auto it : H[pos]) {
			if(it.key == key) {
				it.value = value;
				found = true;
			}
		}
		if(!found) {
			info<Tkey, Tvalue> temp;
			temp.key = key;
			temp.value = value;
			H[pos].push_back(temp);
			size++;
		}
	}

	void remove(Tkey key) {
		int pos = Hfunction(key) % capacity;
		typename std :: list<info < Tkey, Tvalue> > :: iterator it;
		for(it = H[pos].begin(); it != H[pos].end(); it++) {
			if(it.key == key) {
				break;
			}
		}
		if(it != H[pos].end()) {
			H[pos].erase(it);
			size--;
		}
	}

	Tvalue get(Tkey key) {
		int pos = Hfunction(key) % capacity;
		for(auto it : H[pos]) {
			if(it.key == key) {
				return it.value;
			}
		}
		return Tvalue();
	}

	bool has_key(Tkey key) {
		int pos = Hfunction(key) % capacity;
		for(auto it : H[pos]) {
			if(it.key == key) {
				return true;
			}
		}
		return false;
	}
};

int smart_hash(std::string str) {
	unsigned int hash = 5381;
    for (std:: string :: iterator it = str.begin(); it != str.end(); it++) {
        hash = ((hash << 5) + hash) + *it;
    }
	return hash % std :: numeric_limits<int> :: max();
}
#endif  // HASHTABLE_H_
