#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include "work.h"
using namespace std;

B_Tree<student, student_comparator, student_printer> stus;
string helpDoc;

void getHelpDocAndShow(string& helpDoc) {
	ifstream in("help.txt");
	string t_cmd;
	helpDoc = "";
	if (in) {
		while (true) {
			getline(in, t_cmd);
			if (in.fail()) {
				in.close();
				break;
			}
			helpDoc += t_cmd + '\n';
		}
	}

	in.close();
}

string cmd;
student tmp;
student *res;
student t;
string json;
string t_cmd;
ifstream in;
ifstream in1;
ofstream out;

void finish_work() {
	in.close();
	out.close();
}

bool work(int &lineCount)
{
	++lineCount;
	if (!lineCount)
		return true;
	app.str = std::to_string(lineCount)+" ";
	
	getline(in1, app.nxt_str);
	in >> t_cmd;
	if (in.fail()) {
		finish_work();
		return false;
	}
	cmd = "";
	transform(t_cmd.begin(), t_cmd.end(), back_inserter(cmd), ::tolower);
	
	student_printer pr;
	app.str += t_cmd +" ";

	try {
		if ("exit" == cmd || "quit" == cmd) {
			finish_work();
			return false;
		}
		else if ("insert" == cmd || "add" == cmd) {
			in >> tmp;
			app.str += tmp.name + " " + std::to_string(tmp.score);
			if (!lineCount) return true;
			stus.insert(tmp);
			out << tmp << "�Ѿ��ɹ�����" << endl;
			app.res_str = pr.to_string(tmp) + "�Ѿ��ɹ�����";
		}
		else if ("remove" == cmd || "delete" == cmd) {
			in >> tmp.name;
			app.str += (tmp.name) +" ";
			if (!lineCount) return true;
			stus.remove(tmp);
			out << "�Ѿ��ɹ�ɾ��" << tmp.name << endl;
			app.res_str = "�Ѿ��ɹ�ɾ��" + tmp.name;
		}
		else if ("find" == cmd || "query" == cmd) {
			in >> tmp.name;
			app.str += (tmp.name) + " ";
			if (!lineCount) return true;
			res = stus.find(tmp);
			if (nullptr == res) {
				out << tmp.name << "������" << endl;
				app.res_str = tmp.name+ "������";
			}
			else {
				out << "�ѳɹ��ҵ�" << *res << endl;
				app.res_str = "�ѳɹ��ҵ�" + pr.to_string(*res);
			}
		}
		else if ("change" == cmd) {
			in >> tmp;
			app.str += tmp.name + " " + std::to_string(tmp.score);
			if (!lineCount) return true;
			res = stus.find(tmp);
			if (nullptr == res) {
				out << tmp.name << "�����ڣ�";
				out << "��ֱ�Ӳ���������¼��" << tmp;
				app.res_str = tmp.name + "�����ڣ�" + "��ֱ�Ӳ���������¼��\n";
				stus.insert(tmp);
				out << "�Ѿ��ɹ�����" << endl;
				app.res_str += "�Ѿ��ɹ�����";
			}
			else {
				t = *res;
				stus.remove(t);
				stus.insert(tmp);
				out << "�ѳɹ���" << tmp.name << "�ĳɼ���" << t.score << "�޸�Ϊ" << tmp.score << endl;
				app.res_str = "�ѳɹ���" + tmp.name + "�ĳɼ���" + std::to_string(t.score) + "�޸�Ϊ" + std::to_string(tmp.score);
			}
		}
		else if ("help" == cmd) {
			out << helpDoc << endl;
			app.res_str = helpDoc;
		}
		else {
			out << cmd << "�������" << endl;
			app.res_str = "�������";
		}
	}
	catch (exception e) {
		out << e.what() << endl;
		app.res_str = e.what();
	}
	stus.paint(app,0,0,1200,/*0,-1,*/stus.get_root());
	return true;
}

bool work_Initialize()
{
	getHelpDocAndShow(helpDoc);
	in.open("input.txt");
	if (!in)
		return false;
	in1.open("input.txt");
	getline(in1, app.nxt_str);
	out.open("out.txt");
	if (!out) {
		in.close();
		return false;
	}
	return true;
}
