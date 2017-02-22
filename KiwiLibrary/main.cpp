//

#include "stdafx.h"
#include "locale.h"
#include <memory>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

#include "KTrie.h"
#include "KForm.h"
#include "Utils.h"

vector<KForm> formList(2500);

shared_ptr<KTrie> buildTrie()
{
	shared_ptr<KTrie> kt = make_shared<KTrie>();
	FILE* file;
	if (fopen_s(&file, "../ModelGenerator/model.txt", "r")) throw exception();
	char buf[2048];
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	int noForm = 0;
	while (fgets(buf, 2048, file))
	{
		auto wstr = converter.from_bytes(buf);
		auto p = wstr.find('\t');
		if (p == wstr.npos) continue;
		int i = 0;
		for (auto w : wstr.substr(0, p))
		{
			if (w < 0x3130) continue;
			buf[i++] = w - 0x3130;
		}
		if (!i) continue;
		buf[i] = 0;
		formList[noForm] = {buf}; // duplication of forms possible. should remove
		kt->build(buf, &formList[noForm++]);
	}
	fclose(file);
	kt->fillFail();
	return kt;
}



void printJM(const KChunk& c)
{
	if (c.isStr()) return printJM(&c.str[0], 16);
	return printJM(c.form->form);
}

int main()
{
	system("chcp 65001");
	_wsetlocale(LC_ALL, L"korean");

	shared_ptr<KTrie> kt = buildTrie();

	FILE* file;
	if (fopen_s(&file, "../TestFiles/01.txt", "r")) throw exception();
	char buf[2048];
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	vector<vector<char>> wordList;
	wordList.emplace_back();
	while (fgets(buf, 2048, file))
	{
		auto wstr = converter.from_bytes(buf);
		for (auto c : wstr)
		{
			if (0xAC00 <= c && c < 0xD7A4)
			{
				splitJamo(c, wordList.back());
			}
			else
			{
				if (wordList.back().empty()) continue;
				else wordList.emplace_back();
			}
		}
	}
	for (auto w : wordList) 
	{
		if (w.empty()) continue;
		printJM(&w[0], w.size());
		printf("\n");
		auto ss = kt->split(w);
		for (auto s : ss)
		{
			for (auto p : s)
			{
				printJM(p);
				printf(", ");
			}
			printf("\n");
		}
		printf("\n\n");
	}
	fclose(file);
    return 0;
}

