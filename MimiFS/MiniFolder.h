#pragma once

class ConsoleApp;

class MiniFolder:public MiniFile
{
	friend ConsoleApp;
	friend MiniFile;
protected:
	MiniFolder();
	
public:
	~MiniFolder();
	void load();
	void show()override;

	std::string getAbsolutePath();

	int deleteForever()override;
	void findMatchFiles(std::string&pattern,std::vector<MiniFile*>&out);

	bool moveFrom(MiniFolder*other,std::string&filename);

	MiniFile* createChildFile(std::string& filename);
	MiniFolder* createChildFolder(std::string& filename);

	void printTree(int depth);

	void clear();

	bool isChildOf(MiniFolder*);

	void updateDir();
	bool isLoaded() { return loaded;}
	MiniFile*& atChild(std::string&filename);
	MiniFile*& getChild(std::string&filename);

	MiniFolder* getFolderByRelativePath(std::vector<std::string>&nameList,bool queryCreateNew=true);

private:
	bool loaded;
	std::unordered_map<std::string, MiniFile*>childs;
};

