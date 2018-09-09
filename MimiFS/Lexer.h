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


	bool nextTokenMatchString();
	bool nextTokenMatchEnd();
	bool nextTokenMatchCommand();
	bool nextTokenMatchNum();
	bool matchNum();
	bool matchString();
	bool matchCommand();
	bool matchEnd();

	long num;
	std::string str;

private:
	std::string source;
	int current;

};

