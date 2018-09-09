#include "stdafx.h"
#include "Lexer.h"


Lexer::Lexer(std::string& source) {
	this->source = source;
	current = 0;
}


Lexer::~Lexer()
{
}

Lexer::Token Lexer::nextToken() {

	while (source[current] == ' ' || source[current] == '\t') current++;

	switch (source[current++])
	{
	case 0:
		return token=Token::None;
	case '"': {
		str = "";

		while (source[current] != '"'&&source[current]!=0) {
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
		return token=Token::RealString;
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
		return token=Token::RealString;
	}
	case '-': {
		int start = current;
		while (source[current] != ' '&&source[current] != '\t'&&source[current]!=0)current++;
		str.assign(source,start, current - start);
		return token=Token::Command;
	}
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9': {
		num = source[current - 1] - '0';
		while (source[current] >= '0'&&source[current] <= '9') {
			num = num*10+ source[current - 1] - '0';
			current++;
		}
		return token=Token::Num;
	}
	default: {
		int start = current-1;
		while (source[current] != ' '&&source[current] != '\t'&&source[current] != 0)current++;
		str.assign(source,start, current - start);
		return token=Token::String;
	}
	}

}