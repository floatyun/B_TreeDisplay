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
#include "B_Tree.h"
#include "student.h"
using namespace std;
extern B_Tree<student, student_comparator, student_printer> stus;
extern string helpDoc;
void getHelpDocAndShow(string& helpDoc);
bool work(int &lineCount);
bool work_Initialize();
void finish_work();