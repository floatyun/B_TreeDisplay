#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>
using namespace std;
struct student {
	string name;
	int score;
	friend istream& operator >>(istream&in, student&obj);
	friend ostream& operator <<(ostream&out, const student&obj);
	bool operator <(const student&y) const {
		return name < y.name;
	}
	bool operator <=(const student&y) const {
		return name <= y.name;
	}
	bool operator >(const student&y) const {
		return name > y.name;
	}
	bool operator >=(const student&y) const {
		return name >= y.name;
	}
	bool operator ==(const student&y) const {
		return name == y.name;
	}
	bool operator !=(const student&y) const {
		return name != y.name;
	}
};

struct student_comparator {
	int operator ()(const student&x, const student&y) {
		return strcmp(x.name.c_str(), y.name.c_str());
		//return x.name - y.name;
	}
};

struct student_printer {
	void print(ostream&f, const student&stu) {
		f << stu;
	}
	string to_string(const student&stu) {
		return stu.name + " " + std::to_string(stu.score);
	}
};