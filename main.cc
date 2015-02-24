#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <cassert>
#include <cstdio>
#include <vector>
#include "double_array_trie.h"
using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 2) {
		cout << "Should provide the file path!" << endl;
		return -1;
	}
	DoubleArrayTrie double_array_trie;
	vector<string> words;
	const int LINE_LENGTH =  100;
	char word[LINE_LENGTH];
	ifstream fin(argv[1], ios::in);
	while(fin.getline(word, LINE_LENGTH)) {
		string s(word);
		words.push_back(s);
	}
	fin.close();
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); iter++) {
		double_array_trie.insertStr(*iter);
	}
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); iter++) {
		assert(double_array_trie.findStr(*iter));
	}
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); iter++) {
		printf("%s\n", iter->c_str());
		double_array_trie.deleteStr(*iter);
		assert(!double_array_trie.findStr(*iter));
	}
	assert(double_array_trie.isEmptyTail());
	assert(double_array_trie.isEmptyTrie());
	return 0;
}
