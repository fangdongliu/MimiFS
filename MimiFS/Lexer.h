#pragma once



class Lexer
{
public:
	Lexer(std::string& source);
	~Lexer();

	enum class Token {
		None,
		Num,
		RealString,
		String,
		Command
	}token;

	Token nextToken();

	long num;
	std::string str;

private:
	std::string source;
	int current;

};

