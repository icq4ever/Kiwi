#pragma once

vector<char> splitJamo(wstring hangul);
void splitJamo(wchar_t hangul, vector<char>& ret);
void printJM(const char* c, size_t len = -1);
void printJM(const string& c);

template<typename Iter>
vector<char> encodeJamo(Iter begin, Iter end) {
	vector<char> ret;
	for (; begin != end; ++begin)
	{
		assert(*begin > 0x3130 && *begin < 0x3130 + 51);
		ret.push_back(*begin - 0x3130);
	}
	return ret;
}

template<typename Out>
void split(const wstring &s, wchar_t delim, Out result) {
	wstringstream ss;
	ss.str(s);
	wstring item;
	while (getline(ss, item, delim)) {
		*(result++) = item;
	}
}

inline vector<wstring> split(const wstring &s, wchar_t delim) {
	vector<wstring> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

template<typename Out>
void split(const string &s, char delim, Out result) {
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		*(result++) = item;
	}
}

inline vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}