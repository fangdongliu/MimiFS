#pragma once



class Lexer
{
public:
	struct End {
		int d;
	};
	static End end;
	Lexer(std::string& source);
	~Lexer();
	
	const std::string& getName()const { return name; }

	bool matchCountInRange(int a, int b=INT_MAX)const;
	bool findSwitch(std::string&&);

	bool matchSuccess() { return !notMatch; }

	Lexer& operator>=(Lexer::End);
	Lexer& operator>=(int&);
	Lexer& operator>(int&);
	Lexer& operator>(long&);
	Lexer& operator>(std::string&);
	Lexer& operator>=(long&);
	Lexer& operator>=(std::string&);

private:
	bool notMatch;
	int matchCount;
	int current;

	enum class Token {
		None,
		RealString,
		String,
		Command
	}token;

	std::string name;
	std::string str;
	std::string source;
	

	std::unordered_map<std::string,bool> switchs;
	std::vector<std::string> params;

	Token nextToken();
	
	bool nextTokenMatchString();
	bool nextTokenMatchEnd();
	bool nextTokenMatchCommand();
	bool matchPureString();
	bool matchString();
	bool matchCommand();
	bool matchEnd();

};

