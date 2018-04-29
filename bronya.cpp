#include "stdafx.h"
#include"hyouka.h"
#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<fstream>
#include<string>
using namespace std;
namespace satoshi {
	void Table::write_table() {
		ofstream fout(f_name, ios_base::out);
		int i;
		for (i = 0; i <table[(*table.begin()).first].size(); i++) {
			for (auto x : table) {
				fout << x.second[i] << "  ";
			}
			fout << endl;
		}
		fout.clear();
		fout.close();
	}
	Table::Table(vector<string>&cols, string filename) {
		f_name = filename;
		int i;
		for (i = 0; i < cols.size(); i++) {
			vector<string>temp;
			temp.push_back(cols[i]);
			table[cols[i]] = temp;
		}
	}
	Table::Table(string filename) {
		f_name = filename;
		ifstream fin(f_name, ios_base::in);
		vector<string>raw;
		while (fin.peek() != '\n') {
			string temp;
			fin >> temp;
			raw.push_back(temp);
			fin.get();
			fin.get();
		}
		for (int i = 0; i < raw.size(); i++) {
			vector<string>temp;
			table[raw[i]] = temp;
		}
		int i = 0;
		for (auto &x : table) {
			x.second.push_back(raw[i]);
			i++;
		}
		raw.clear();
		while (fin) {
			string temp;
			fin >> temp;
			if (!fin) {
				break;
			}
			raw.push_back(temp);
			fin.get();
			fin.get();
			while (fin.peek() != '\n') {
				fin >> temp;
				raw.push_back(temp);
				fin.get();
				fin.get();
			}
			i = 0;
			for (auto &x : table) {
				x.second.push_back(raw[i]);
				i++;
			}
			raw.clear();
		}
		fin.clear();
		fin.close();
	}
	void Table::show_table() {
		cout << "(" << table.size() << "," << (*table.begin()).second.size() << ")"<<"  ";
		for (auto x : table) {
			cout << x.first << "  ";
		}
		cout << endl;
	}
	void Table::insert(vector<string>&vals) {
		if (vals.size() != table.size()) {
			cout << " inlegile input" << endl;
			return;
		}
		else {
			int i = 0;
			for (auto &x : table) {
				x.second.push_back(vals[i]);
				i++;
			}
		}
	}
	void Table::insert(vector<string>&cols, vector<string>&vals) {
		if (cols.size() != vals.size()) {
			cout << "inlegile input" << endl;
			return;
		}
		else {
			int i = 0;
			for (; i < vals.size(); i++) {
				try {
					table.at(cols[i]);
				}
				catch (exception) {
					cout << "no such column" << endl;
					return;
				}
			}
			for (auto &x : table) {
				x.second.push_back("NONE");
			}
			for (i = 0; i < cols.size(); i++) {
				table[cols[i]][table[cols[i]].size() - 1] = vals[i];
			}
		}
	}
	void Table::del() {
		int i;
		for (auto &x : table) {
			x.second.erase(x.second.begin()+1, x.second.end());
		}
	}
	void Table::update(vector<string>&cols, vector<string>&vals) {
		if (cols.size() != vals.size()) {
			cout << "illegal input" << endl;
			return;
		}
		else {
			int i=0;
			for (i = 0; i < cols.size(); i++) {
				for (int j = 1; j < table[cols[i]].size(); j++) {
					table[cols[i]][j] = vals[i];
				}
			}
		}
	}
	void Table::select(vector<string>&cols, bool distinct) {
		int i = 0;
		for (; i < cols.size(); i++) {
			try {
				table.at(cols[i]);
			}
			catch (exception) {
				cout << "no such column" << endl;
				return;
			}
		}
		if (!distinct) {
			for (i = 0; i < table[cols[0]].size(); i++) {
				for (int j = 0; j < cols.size(); j++) {
					cout << table[cols[j]][i]<<"  ";
				}
				cout << endl;
			}
		}
		else {
			map<string, vector<string> > temp = table;
			int i = 0;
			for (; i < cols.size(); i++) {
				auto mark = unique(table[cols[i]].begin(), table[cols[i]].end());
				for (; mark != table[cols[i]].end(); mark++) {
					*mark = "   ";
				}
			}
			for (i = 0; i < table[cols[0]].size(); i++) {
				for (int j = 0; j < cols.size(); j++) {
					cout << table[cols[j]][i] << "  ";
				}
				cout << endl;
			}
			table = temp;
		}
	}
	void Table::select() {
		auto x = table.begin();
		string col_name = (*x).first;
		for (int i = 0; i < table[col_name].size(); i++) {
			for (auto &y : table) {
				cout << y.second[i] << "  ";
			}
			cout << endl;
		}
	}

	void Table::sorted_select(vector<string>&cols, bool order) {
		int i = 0;
		int j = 0;
		for (; i < cols.size(); i++) {
			try {
				table.at(cols[i]);
			}
			catch (exception) {
				cout << "no such column" << endl;
				return;
			}
		}
		vector<int>index;
		for (i = 1; i < table.size(); i++) {
			index.push_back(i);
		}
		if (!order) {
			for (i = index.size()-1; i >=0; i--) {
				for (j = 0; j < i; j++) {
					double index_j = 0;
					for (int p = 0; p < cols.size(); p++) {
						if (table[cols[p]][j] < table[cols[p]][j + 1]) {
							index_j -= 1 / (p + 1) / (p + 1);
						}
						else if (table[cols[p]][j] > table[cols[p]][j + 1]) {
							index_j += 1 / (p + 1) / (p + 1);
						}
						else {
							index_j += 0;
						}
					}
					if (index_j > 0) {
						swap(index[j], index[j + 1]);
					}
				}
			}
		}
		else {
			for (i = index.size() - 1; i >= 0; i--) {
				for (j = 0; j < i; j++) {
					double index_j = 0;
					for (int p = 0; p < cols.size(); p++) {
						if (table[cols[p]][j] < table[cols[p]][j + 1]) {
							index_j -= 1 / (p + 1) / (p + 1);
						}
						else if (table[cols[p]][j] > table[cols[p]][j + 1]) {
							index_j += 1 / (p + 1) / (p + 1);
						}
						else {
							index_j += 0;
						}
					}
					if (index_j < 0) {
						swap(index[j], index[j + 1]);
					}
				}
			}
		}
		for (auto &x : table) {
			cout << x.second[0] << "  ";
		}
		cout << endl;
		for (i = 0; i < index.size(); i++) {
			for (auto y : table) {
				cout << y.second[index[i]] << "  ";
			}
			cout << endl;
		}
	}
	void Table::select(vector<string>&cols, string filename) {
		int i=0;
		for (; i < cols.size(); i++) {
			try {
				table.at(cols[i]);
			}
			catch (exception) {
				cout << "no such column" << endl;
				return;
			}
		}
		ofstream fout(filename, ios_base::out);
		for (i = 0; i < table[cols[0]].size(); i++) {
			for (int j = 0; j < cols.size(); j++) {
				fout << table[cols[j]][i] << "  ";
			}
			fout << endl;
		}
	}
	void Table::select(string filename) {
		string temp = f_name;
		f_name = filename;
		write_table();
		f_name = temp;
	}
}
