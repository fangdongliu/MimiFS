#include "stdafx.h"
#include "Lexer.h"

Lexer::End Lexer::end;

Lexer::Lexer(std::string& source) {
	this->source = source;
	current = 0;
	matchCount = 0;
	notMatch = false;

	if (nextTokenMatchEnd()) {
		return;
	}
	else if (matchPureString()) {
		name = str;
	}

	while (!nextTokenMatchEnd()) {
		if (matchCommand()) {
			switchs[str] = false;
		}
		else if (matchString()) {
			params.push_back(str);
		}
	}

}


Lexer::~Lexer()
{

	bool has = false;

	for (auto i : switchs) {
		if (i.second == false) {
			has = true;
			std::cout << "无效的开关:-" << i.first << '\n';
		}
	}
	if (has) {
		std::cout << '\n';
	}
}
bool Lexer::findSwitch(std::string&&s) {

	try {
		auto &t = switchs.at(s);
		t = true;
		return true;
	}
	catch (std::exception&e) {
		return false;
	}

}

Lexer& Lexer::operator>(std::string&ss) {

	if (notMatch)
		return *this;
	if (matchCount < params.size())
		ss = params[matchCount++];

	return *this;
}

Lexer& Lexer::operator>=(std::string&ss) {

	if (notMatch)
		return *this;
	if (matchCount < params.size())
		ss = params[matchCount++];
	else
		throw CommandFormatError();

	return *this;
}

Lexer& Lexer::operator>(long& ll) {

	if (notMatch)
		return *this;

	if (matchCount < params.size()) {

		long long num = 0;
		bool isNull = true;
		for (auto i : params[matchCount++]) {
			if (i >= '0'&&i <= '9') {
				num = num * 10 + i - '0';
				isNull = false;
			}
			else {
				throw CommandFormatError();
				return *this;
			}
		}
		if (!isNull)
			ll = num;
	}
	return *this;
}

Lexer& Lexer::operator>=(int& ll) {

	if (notMatch)
		return *this;

	if (matchCount < params.size()) {

		long long num = 0;
		for (auto i : params[matchCount++]) {
			if (i >= '0'&&i <= '9')
				num = num * 10 + i - '0';
			else {
				throw CommandFormatError();
				return *this;
			}
		}
		ll = num;
	}
	else
		throw CommandFormatError();

	return *this;
}

Lexer& Lexer::operator>(int& ll) {

	if (notMatch)
		return *this;

	if (matchCount < params.size()) {

		long long num = 0;
		bool isNull = true;
		for (auto i : params[matchCount++]) {
			if (i >= '0'&&i <= '9') {
				num = num * 10 + i - '0';
				isNull = false;
			}
			else {
				throw CommandFormatError();
				return *this;
			}
		}
		if (!isNull)
			ll = num;
	}
	return *this;
}

Lexer& Lexer::operator>=(Lexer::End) {
	if (matchCount != params.size()) {
		throw CommandFormatError();
		notMatch = true;
	}
	return *this;
}

Lexer& Lexer::operator>=(long& ll) {

	if (notMatch)
		return *this;

	if (matchCount < params.size()) {

		long long num = 0;
		for (auto i : params[matchCount++]) {
			if (i >= '0'&&i <= '9')
				num = num * 10 + i - '0';
			else {
				throw CommandFormatError();
			}
		}
		ll = num;
	}
	else
		throw CommandFormatError();

	return *this;
}

Lexer::Token Lexer::nextToken() {

	while (source[current] == ' ' || source[current] == '\t') current++;

	switch (source[current++])
	{
	case 0:
		return token = Token::None;
	case '"': {
		str = "";

		while (source[current] != '"'&&source[current] != 0) {
			if (source[current] == '\\') {
				current++;

				if (source[current] == 0)
					break;

				str += source[current];
			}
			else {
				str += source[current];
			}
			current++;
		}


		if (source[current] != 0)
			current++;
		return token = Token::RealString;
	}

	case '\'': {
		str = "";
		while (source[current] != '\''&&source[current] != 0) {
			if (source[current] == '\\') {
				current++;

				if (source[current] == 0)
					break;

				str += source[current];
			}
			else {
				str += source[current];
			}
			current++;
		}

		if(source[current]!=0)
		current++;
		return token = Token::RealString;
	}
	case '-': {
		int start = current;
		while (source[current] != ' '&&source[current] != '\t'&&source[current] != 0)current++;
		str.assign(source, start, current - start);
		return token = Token::Command;
	}

	default: {
		int start = current - 1;
		while (source[current] != ' '&&source[current] != '\t'&&source[current] != 0)current++;
		str.assign(source, start, current - start);
		return token = Token::String;
	}
	}

}

bool Lexer::nextTokenMatchString() {
	nextToken();
	return token == Token::String || token == Token::RealString;
}
bool Lexer::nextTokenMatchEnd() {
	nextToken();
	return token == Token::None;
}
bool Lexer::nextTokenMatchCommand() {
	nextToken();
	return token == Token::Command;
}
bool Lexer::matchPureString() {
	return token == Token::String;
}

bool Lexer::matchString() {
	return token == Token::String || token == Token::RealString;
}
bool Lexer::matchCommand() {
	return token == Token::Command;
}
bool Lexer::matchEnd() {
	return token == Token::None;
}