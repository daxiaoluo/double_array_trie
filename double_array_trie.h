#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

struct BaseItem {
	int val;
	vector<char> out;
	bool isLeaf;
	list<char>::iterator tail;
	BaseItem(list<char> &tail_list): val(0), isLeaf(false), tail(tail_list.end()) {}
};

#ifndef DOUBLE_ARRAY_TRIE_H
#define DOUBLE_ARRAY_TRIE_H
class DoubleArrayTrie {
private:
	bool ignore_case;
	list<char>::iterator cur;
	list<char> tail_array;
	vector<int> check_array;
	vector<BaseItem> base_array;
	map<char, int> dict;
	bool isLegalStr(const string &str);
	void storeTailHead(int index, char item);
	void insertTailArray(int index, const string &str, int str_index);
	void resolvePrefixConflict(int index, const string& str, int str_index);
	void resolveBaseConflict(int pre_index, int old_base_index, int check_index, int str_index, const string& str);
	int getNewBase(int cur_index, char s);
	void getNewBase(int cur_index, char s, bool isAdded);
	void getNewBase(int cur_index, char s1, char s2, int &prefix1_index, int &prefix2_index);
	bool compareStr(int str_index, const string& str, int index);
public:
	DoubleArrayTrie(bool igore_case = true);
	bool FindStr(const string &str);
	void insertStr(const string &str);
	void deleteStr(const string &str);
	bool isEmptyTail();
};

#endif
