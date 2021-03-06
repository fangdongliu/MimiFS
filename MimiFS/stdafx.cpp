// stdafx.cpp : 只包括标准包含文件的源文件
// MimiFS.pch 将作为预编译标头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用

#pragma comment(lib,"winmm.lib")

bool Helper::isRightFilename(std::string&filename) {

	bool isNull=true;

	for (auto i : filename) {
		if (i == '\\' || i == '/' || i == ':') {
			return false;
		}
		else if (i != ' '&&i != '\t'&&i!='\n') {
			isNull = false;
		}
	}

	return !isNull;
}

void Helper::ASCIIToUTF8(const char cACSII[], char cUTF8[])
{
	int nlen = MultiByteToWideChar(CP_ACP, 0, cACSII, -1, NULL, NULL);
	wchar_t *pUnicode = new wchar_t[1000];
	memset(pUnicode, 0, nlen * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, cACSII, -1, (LPWSTR)pUnicode, nlen);
	std::wstring wsUnicode = pUnicode;
	nlen = WideCharToMultiByte(CP_UTF8, 0, wsUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wsUnicode.c_str(), -1, cUTF8, nlen, NULL, NULL);
	delete pUnicode;
}
void Helper::UTF8ToUnicode(const char *cUTF8, wchar_t wcUnicode[])
{
	std::string sUTF8 = cUTF8;
	int nLen = MultiByteToWideChar(CP_UTF8, 0, sUTF8.c_str(), -1, NULL, 0);
	MultiByteToWideChar(CP_UTF8, 0, sUTF8.c_str(), -1, (LPWSTR)wcUnicode, nLen);
}

void Helper::cutPathFromString(std::string pathname, std::vector<std::string>&out) {

	out.clear();
	int last = 0;
	int cur = 0;

	for (auto i : pathname) {
		if (i == '/' || i == '\\') {
			if (cur > last) {
				out.push_back(pathname.substr(last, cur - last));
			}
			last = cur + 1;
		}
		cur++;
	}

	if (cur > last) {
		out.push_back(pathname.substr(last, cur - last));
	}

}

bool Helper::blurMatch(const std::string& pattern,const std::string& str)
{

	int lenA = pattern.length();
	int lenB = str.length();
	auto strA = pattern.c_str();
	auto strB = str.c_str();

	int i = pattern.find_first_of('*');

	if (i == pattern.npos) {
		if (lenB != lenA) {
			return false;
		}
		for (int j = 0; j < lenA; j++) {
			if (strA[j] != strB[j] && strA[j] != '?')
				return false;
		}
		return true;
	}
	else {
		if (lenB < i) {
			return false;
		}
		for (int j = 0; j < i; j++) {
			if (strA[j] != strB[j] && strA[j] != '?')
				return false;
		}
	}

	bool dp[100][100];
	for (int k = i; k <= lenB; k++) {
		dp[i + 1][k] = true;
	}
	for (int k = i + 1; k <= lenA; k++) {
		dp[k + 1][i] = false;
	}

	for (int j = i + 1; j < lenA; j++) {

		bool match = false;

		for (int k = i; k < lenB; k++) {
			if (strA[j] == '*') {
				dp[j + 1][k + 1] = dp[j][k] || dp[j][k + 1] || dp[j + 1][k];
			}
			else if (strA[j] == '?') {
				dp[j + 1][k + 1] = dp[j][k];
			}
			else {
				dp[j + 1][k + 1] = dp[j][k] ? strA[j] == strB[k] : false;
			}
			if (!match&&dp[j + 1][k + 1])
				match = true;

		}

		if (!match)
			return false;

	}



	return dp[lenA][lenB];

}