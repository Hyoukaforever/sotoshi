#include "stdafx.h"
#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<fstream>
#include<string>
#include"hyouka.h"
using namespace std;
namespace satoshi {
	Database::Database() {
		database = {};
		cur_instructor = "";
		mode = -1;
		instructors["CREATETABLETO"] = 1;
		instructors["CREATETABLEFROM"] = 2;
		instructors["DROPTABLE"] = 3;
		instructors["TABLELIST"] = 4;
		instructors["INSERTINTOVALUES"] = 5;
		instructors["INSERTINTOCOLVALUES"] = 6;
		instructors["DELETEFROMWHERE"] = 7;
		instructors["DELETE*FROM"] = 8;
		instructors["UPDATESET"] = 9;
		instructors["UPDATESETWHERE"] = 10;
		instructors["SELECTFROM"] = 11;
		instructors["SELECT*FROM"] = 12;
		instructors["SELECTDISTINCTFROM"] = 13;
		instructors["SELECT*FROMORDERBY"] = 14;
		instructors["SELECTFROMWHERE"] = 15;
		instructors["SELECTFROMTO"] = 16;
		instructors["SELECT*FROMTO"] = 17;
	}
	bool Database::check_instructor() {
		string tem_inst;
		getline(cin, tem_inst);
		cur_instructor = tem_inst;
		auto indictor=remove_if(tem_inst.begin(), tem_inst.end(), [](char ch) {if ((ch >= 'A'&&ch <= 'Z') || ch == '*')return false; else return true; });
		tem_inst.erase(indictor, tem_inst.end());
		try {
			mode=instructors.at(tem_inst);
		}
		catch (exception) {
			cout << "no such instructors" << endl;
			return false;
		}
		return true;
	}
	void Database::operation() {
		if (mode == 1) {
			vector<string>temp_memory;
			char temp_ch = cur_instructor[13];
			string temp_string;
			int i = 13;
			while (temp_ch != ' ') {
				temp_string.push_back(temp_ch);
				i++;
				temp_ch = cur_instructor[i];
			}
			pair<string, vector<string> > temp_pair;
			temp_pair.first = temp_string;
			while(cur_instructor[i]!='('){
				i++;
			}
			i += 1;
			temp_string.clear();
			while (1) {
				while (cur_instructor[i] != ','&&cur_instructor[i] != ')') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				temp_pair.second.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ')') {
					break;
				}
				i++;
			}
			i += 5;
			while (i < cur_instructor.size()) {
				temp_string.push_back(cur_instructor[i]);
				i++;
			}
			Table cur_table(temp_pair.second, temp_string);
			pair<string, Table> cur_pair;
			cur_pair.first = temp_pair.first;
			cur_pair.second = cur_table;
			database.insert(cur_pair);
		}
		else if (mode == 2) {
			string temp_table_name;
			string temp_file_name;
			int i = 13;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			i += 6;
			for (; i < cur_instructor.size(); i++) {
				temp_file_name.push_back(cur_instructor[i]);
			}
			Table temp_table(temp_file_name);
			pair<string, Table>temp_dat;
			temp_dat.first = temp_table_name;
			temp_dat.second = temp_table;
			database.insert(temp_dat);
		}
		else if (mode == 3) {
			int i = 11;
			string temp_table_name;
			for (; i < cur_instructor.size(); i++) {
				temp_table_name.push_back(cur_instructor[i]);
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				cout << "success" << endl;
				database.erase(iter);
			}
		}
		else if (mode == 4) {
			cout << "total" << database.size()<<endl;
			for (auto i : database) {
				cout << i.first << ":";
				i.second.show_table();
			}
		}
		else if (mode == 5) {
			int i = 12;
			string temp_table_name;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			while (cur_instructor[i] != '(') {
				i++;
			}
			i++;
			vector<string> val_list;
			while (cur_instructor[i]!=')') {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i] != ')') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				val_list.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ')') {
					break;
				}
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].insert(val_list);
			}
		}
		else if (mode == 6) {
			int i = 12;
			string temp_table_name;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			while (cur_instructor[i] != '(') {
				i++;
			}
			i++;
			vector<string> col_list;
			while (1) {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i]!=')') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				col_list.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ')') {
					break;
				}
				i++;
			}
			while (cur_instructor[i] != '(') {
				i++;
			}
			i++;
			vector<string> val_list;
			while (cur_instructor[i] != ')') {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i] != ')') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				val_list.push_back(temp_string);
				if (cur_instructor[i] == ')') {
					break;
				}
				temp_string.clear();
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].insert(col_list, val_list);
			}
		}
		else if (mode == 7) {
			int i = 12;
			string temp_table_name;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			string temp_col;
			i++;
			while (cur_instructor[i] != ' ') {
				i++;
			}
			i++;
			while (cur_instructor[i] != ' ') {
				temp_col.push_back(cur_instructor[i]);
				i++;
			}
			i++;
			string judge_ch;
			while (cur_instructor[i] != ' ') {
				judge_ch.push_back(cur_instructor[i]);
				i++;
			}
			i++;
			string temp_val;
			while (i<cur_instructor.size()) {
				temp_val.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else if (judge_ch== "=") {
					database[temp_table_name].del([](string str1, string str2) {return str1 == str2; },temp_col,temp_val);
			}
		}
		else if (mode == 8) {
			int i = 12;
			string temp_table_name;
			while (i<cur_instructor.size()) {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}			
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].del();
			}
		}
		else if (mode == 9) {
			int i = 7;
			string temp_table_name;
			vector<string>col_list;
			vector<string>val_list;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			i += 5;
			while (i < cur_instructor.size()) {
				string col;
				string val;
				while (cur_instructor[i] != ' ') {
					col.push_back(cur_instructor[i]);
					i++;
				}
				i += 3;
				while (cur_instructor[i] != ','&&i<cur_instructor.size()) {
					val.push_back(cur_instructor[i]);
					i++;
				}
				i++;
				col_list.push_back(col);
				val_list.push_back(val);
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].update(col_list, val_list);
			}
		}
		else if (mode == 10) {
			int i = 7;
			string temp_table_name;
			vector<string>col_list;
			vector<string>val_list;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			i += 5;
			while (1) {
				string col;
				string val;
				while (cur_instructor[i] != ' ') {
					col.push_back(cur_instructor[i]);
					i++;
				}
				i += 3;
				while (cur_instructor[i] != ','&&cur_instructor[i]!=' ') {
					val.push_back(cur_instructor[i]);
					i++;
				}
				col_list.push_back(col);
				val_list.push_back(val);
				if (cur_instructor[i] == ' ') {
					break;
				}
				i++;
			}
			i += 7;
			string temp_col;
			while (cur_instructor[i] != ' ') {
				temp_col.push_back(cur_instructor[i]);
				i++;
			}
			i++;
			string judge_ch;
			while (cur_instructor[i] != ' ') {
				judge_ch.push_back(cur_instructor[i]);
				i++;
			}
			i++;
			string temp_val;
			while (i<cur_instructor.size()) {
				temp_val.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				if (judge_ch == "=") {
					database[temp_table_name].update(col_list, val_list, [](string str1, string str2) {return str1 == str2; }, temp_col, temp_val);
				}
			}
		}
		else if (mode == 11) {
			int i = 7;
			vector<string> col_list;
			while (1) {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i] != ' ') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				col_list.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ' ') {
					break;
				}
				i++;
			}
			i += 6;
			string temp_table_name;
			while (i < cur_instructor.size()) {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].select(col_list);
			}
		}
		else if (mode == 12) {
			int i = 12;
			string temp_table_name;
			while (i < cur_instructor.size()) {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].select();
			}
		}
		else if (mode == 13) {
			int i = 16;
			vector<string> col_list;
			while (1) {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i] != ' ') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				col_list.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ' ') {
					break;
				}
				i++;
			}
			i += 6;
			string temp_table_name;
			while (i < cur_instructor.size()) {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].select(col_list,true);
			}
		}
		else if (mode == 14) {
			int i = 12;
			string temp_table_name;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			i += 10;
			vector<string> col_list;
			while (1) {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i] != ' ') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				col_list.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ' ') {
					break;
				}
				i++;
			}
			i++;
			string tag;
			while (i < cur_instructor.size()) {
				tag.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				if (tag == "asc") {
					database[temp_table_name].sorted_select(col_list,false);
				}
				else if (tag == "desc") {
					database[temp_table_name].sorted_select(col_list, true);
				}
				else {
					cout << "no such order" << endl;
					return;
				}
			}
		}
		else if (mode == 15) {
			int i = 7;
			vector<string> col_list;
			while (1) {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i] != ' ') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				col_list.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ' ') {
					break;
				}
				i++;
			}
			i += 6;
			string temp_table_name;
			while (cur_instructor[i]!=' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			i+=7;
			string temp_col;
			while (cur_instructor[i] != ' ') {
				temp_col.push_back(cur_instructor[i]);
				i++;
			}
			i++;
			string judge_ch;
			while (cur_instructor[i] != ' ') {
				judge_ch.push_back(cur_instructor[i]);
				i++;
			}
			i++;
			string temp_val;
			while (i<cur_instructor.size()) {
				temp_val.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				if (judge_ch == "=") {
					database[temp_table_name].select(col_list, [](string str1, string str2) {return str1 == str2; }, temp_col, temp_val);
				}
			}
		}
		else if (mode == 16) {
			int i = 7;
			vector<string> col_list;
			while (1) {
				string temp_string;
				while (cur_instructor[i] != ','&&cur_instructor[i] != ' ') {
					temp_string.push_back(cur_instructor[i]);
					i++;
				}
				col_list.push_back(temp_string);
				temp_string.clear();
				if (cur_instructor[i] == ' ') {
					break;
				}
				i++;
			}
			i += 6;
			string temp_table_name;
			while (cur_instructor[i] != ' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			i += 4;
			string filename;
			while (i < cur_instructor.size()) {
				filename.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].select(col_list, filename);
			}
		}
		else if (mode == 17) {
			int i = 12;
			string temp_table_name;
			while (cur_instructor[i]!=' ') {
				temp_table_name.push_back(cur_instructor[i]);
				i++;
			}
			i += 4;
			string filename;
			while (i< cur_instructor.size()) {
				filename.push_back(cur_instructor[i]);
				i++;
			}
			auto iter = database.find(temp_table_name);
			if (iter == database.end()) {
				cout << "no such table" << endl;
				return;
			}
			else {
				database[temp_table_name].select(filename);
			}
		}
	}
}
