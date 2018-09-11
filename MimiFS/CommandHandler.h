#pragma once

class CommandFormatError :public std::exception {

};

class PathNotExist :public std::exception {

};

class InvalidFilename :public std::exception {

};

class CommandHandler
{
public:
	virtual void onHandleCommand(Lexer&param)=0;

};

