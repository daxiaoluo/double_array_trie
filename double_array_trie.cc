#include "double_array_trie.h"
#include <cctype>
#include <cassert>

DoubleArrayTrie::DoubleArrayTrie(bool igore_case) {
	this->igore_case = igore_case;
	BaseItem item(this->tail_array);
	cur = this->tail_array.end();
	check_array.resize(2, 0);
	base_array.resize(2, item);
	base_array[1].val = 1;

	int index = 1;
	for(int i = 0; i < 26; i++) {
		dict[char('a' + i)] = index++;
	}
	for(int i = 0; i < 26; i++) {
		dict[char('A' + i)] = index++;
	}
}

bool DoubleArrayTrie::isLegalStr(const string &str) {
	for(int i = 0; i < str.size(); i++) {
		if(dict.find(str[i]) == dict.end())
		  return false;
	}
	return true;
}

bool DoubleArrayTrie::FindStr(const string &str) {
	if(!isLegalStr(str))
	  return false;
	assert(base_array.size() == check_array.size());
	int index = 1;
	for(int i = 0; i < str.size(); i++) {
		char s = str[i];
		if(igore_case)
		  s = tolower(str[i]);
		int pre_index = index;
		index = base_array[index].val + dict[s];
		if(index >= base_array.size() || check_array[index] != pre_index)
			return false;
		if(base_array[index].val < 0) { //compare the tail array with str
			assert(base_array[index].tail != tail_array.end());
			int k = i + 1;
			list<char>::iterator iter = base_array[index].tail;
			while(k < str.size() && *iter != '#') {
				if(str[k] != *iter)
				  return false;
				k++;
				iter++;
				assert(iter != tail_array.end());
			}
			if(iter == tail_array.end())
			  return false;
			if(k >= str.size() && *iter == '#')
			  return true;
			return false;
		} else {
			if(i + 1 == str.size() && base_array[index].tail != tail_array.end() && *(base_array[index].tail) == '#') {
				return true;
			}
		}
	}
	return false;
}

void DoubleArrayTrie::insertStr(const string &str) {
	if(!isLegalStr(str))
	  return;
	int index = 1;
	for(int i = 0; i < str.size(); i++) {
		char s = str[i];
		if(igore_case)
		  s = tolower(str[i]);
		int pre_index = index;
		index = base_array[index].val + dict[s];
		if(index >= base_array.size()) { //increase array capacity
			BaseItem item(tail_array);
			check_array.resize(index + 1, 0);
			base_array.resize(index + 1, item);
		}

		if(check_array[index] == 0) { //have no conflicts
			check_array[index] = pre_index;
			base_array[pre_index].out.push_back(s);//save the out edge
			base_array[index].val = -1;
			insertTailArray(index, str, i + 1);
			break;
		} else { // resolve the conflicts
			if(check_array[index] == pre_index) {
				if(resolvePrefixConflict(index, str, i + 1)) // resolve the same prefix conflicts
				  break;
			} else { // resolve the base conflicts
				resolveBaseConflict(pre_index, check_array[index]);
				break;
			}
		}
	}
}

void DoubleArrayTrie::insertTailArray(int index, const string &str, int str_index) {
	assert(str_index <= str.size())
	if(str_index == str.size()) {
		storeTailHead(index, '#');
	} else {
		storeTailHead(index, str[str_index]);
		for(int k = str_index + 1; k < str.size(); k++) {
			tail_array.push_back(igore_case ? tolower(str[k]) : str[k]);
			cur++;
		}
		tail_array.push_back('#');
		cur++;
	}
}

void DoubleArrayTrie::storeTailHead(int index, char item) {
	if(igore_case)
	  item = tolower(item);
	tail_array.push_back(item);
	if(tail_array.empty()) {
		base_array[index].tail = tail_array.begin();
		cur = tail_array.begin(); //inital cur pointer
	} else {
		cur++;
		base_array[index].tail = cur;
	}
}

bool DoubleArrayTrie::resolveBaseConflict(int base_index, int check_index) {
	assert(base_array.size() == check_array.size());
	assert(base_index < base_array.size());
	assert(check_index < base_array.size());

	int index = base_array[base_index].out.size() + 1 <= base_array[check_index].out.size() ? base_index : check_index;

	int q = 1;
	bool flag = true
	while(flag) {
		flag = false;
		for(vector<char>::iterator v_iter = base_array[index].out.begin(); v_iter != base_array[index].out.end(); v_iter++) {
			int cur_index = q + dict[*v_iter];
			if(cur_index < check_array.size() && check_array[cur_index] != 0)
			  flag = true;
		}
		q++;
	}

	base_array[index].val = q;
}

bool DoubleArrayTrie::resolvePrefixConflict(int index, const string& str, int str_index) {
	assert(index < base_array.size());
	assert(base_array.size() == check_array.size());

	if(base_array[index].val > 0) {
		if(base_array[index].tail == tail_array.end() && str_index == str.size()) {
			storeTailHead(index, '#');
			return true;
		}
		return false;
	}

	assert(base_array[index].tail != tail_array.end());

	vector<char> prefix; // save the same prefix
	list<char>::iterator iter = base_array[index].tail;
	while(str_index < str.size() && *iter != '#') {
		char s = str[str_index];
		if(ignore_case)
		  s = tolower(s);
		if(s != *iter)
		  break;
		prefix.push_back(s);
		iter++;
		assert(iter != tail_array.end());
		str_index++;
	}
	if(str_index == str.size() && *iter == '#')
	  return false;


	//resolve the same prefix conflicts

	list<char>::iterator temp_iter = base_array[index].tail;
	int cur_index = index;
	for(vector<char>::iterator v_iter = prefix.begin(); v_iter != prefix.end(); v_iter++) {
		cur_index = getNewBase(cur_index, *v_iter);
	}

	if(str_index >= str.size() && *iter != '#') {
		int tmp_index = cur_index;
		cur_index = getNewBase(cur_index, *iter);
		storeTailHead(tmp_index, '#');// is leaf node
		iter++;
		while(temp_iter != tail_array.end() && temp_iter != iter) {
			temp_iter = tail_array.erase(temp_iter);
		}

		assert(temp_iter != tail_array.end());

		base_array[cur_index].val = -1;
		base_array[cur_index].tail = iter;
	} else if(*iter == '#' && str_index < str.size()){
		while(temp_iter != tail_array.end() && temp_iter != iter) {
			temp_iter = tail_array.erase(temp_iter);
		}
		assert(temp_iter != tail_array.end());

		cur_index = getNewBase(cur_index, str[str_index++]);
		insertTailArray(cur_index, str, str_index);		
	} else {
		int prefix1_index = -1;
		int prefix2_index = -1;
		getNewBase(cur_index, *iter, str[str_index], prefix1_index, prefix2_index);
		assert(prefix1_index != -1 && prefix2_index != -1);
		iter++;
		str_index++;

		while(temp_iter != tail_array.end() && temp_iter != iter) {
			temp_iter = tail_array.erase(temp_iter);
		}
		assert(temp_iter != tail_array.end());

		base_array[prefix1_index].val = -1;
		base_array[prefix1_index].tail = iter;

		insertTailArray(prefix2, str, str_index);
	}
	return true;
}

int DoubleArrayTrie::getNewBase(int cur_index, char s) {
	assert(base_array.size() == check_array.size());
	if(ignore_case)
	  s = tolower(s);
	assert(dict.find(s) != dict.end());
	int q = 1;
	int str_index = dict[s];
	int prefix_index = q + str_index;
	while(prefix_index < check_array.size() && check_array[prefix_index] != 0) {
		q++;
		prefix_index = q + str_index;
	}
	if(prefix_index >= check_array.size()) {
		BaseItem item(this->tail_array);
		check_array.resize(prefix_index + 1, 0);
		base_array.resize(prefix_index + 1, item);
	}
	base_array[cur_index].val = q;
	base_array[cur_index].tail = tail_array.end();
	base_array[cur_index].out.push_back(s); //add out edge
	check_array[prefix_index] = cur_index;
	return prefix_index;
}

void DoubleArrayTrie::getNewBase(int cur_index, char s1, char s2, int &prefix1_index, int &prefix2_index) {
	assert(base_array.size() == check_array.size());
	if(ignore_case) {
		s1 = tolower(s1);
		s2 = tolower(s2);
	}
	assert(dict.find(s1) != dict.end() && dict.find(s2) != dict.end());

	int q = 1;
	int s1_index = dict[s1];
	int s2_index = dict[s2];
	prefix1_index = q + s1_index;
	prefix2_index = q + s2_index;
	while(prefix1_index < check_array.size() && check_array[prefix1_index] != 0 && prefix2_index < check_array.size() && check_array[prefix2_index] != 0) {
		q++;
		prefix1_index = q + s1_index;
		prefix2_index = q + s2_index;
	}
	int max_index = prefix1_index >= prefix2_index ? prefix1_index : prefix2_index;
	if(max_index >= check_array.size()) {
		BaseItem item(this->tail_array);
		check_array.resize(max_index + 1, 0);
		base_array.resize(max_index + 1, item);
	}
	base_array[cur_index].val = q;
	base_array[cur_index].tail = tail_array.end();
	base_array[cur_index].out.push_back(s1);
	base_array[cur_index].out.push_back(s2);
	check_array[prefix1_index] = cur_index;
	check_array[prefix2_index] = cur_index;
}

void DoubleArrayTrie::deleteStr(const string &str) {
}
