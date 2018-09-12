#pragma once

class ConsoleApp;


//一个基本的MiniFS 文件夹
class MiniFolder:public MiniFile
{
	friend ConsoleApp;
	friend MiniFile;
protected:
	MiniFolder();
	
public:
	virtual ~MiniFolder();

	//创建子文件
	MiniFile* createChildFile(std::string& filename,int blockCount);

	//创建子文件夹
	MiniFolder* createChildFolder(std::string& filename);

	MiniFile*& atChild(std::string&filename);//获取一个子文件(安全)
	MiniFile*& getChild(std::string&filename);//获取一个子文件(不安全)

public:
	//目录操作

	void load();				//从外存加载文件目录
	void show();				//显示文件目录	
	void updateDir();			//更新外存目录
	void printTree(int depth);	//打印目录树	
	void clear();				//清空目录

	bool isLoaded()const { return loaded; }//判断是否加载

public:
	//自身操作

	void showAtt()		override;	//显示文件属性(创建日期，大小)
	int deleteForever()	override;	//删除文件
	int computeSize()	override;	//计算文件大小
	std::string getAbsolutePath();	//获取文件夹绝对路径

	//使用通配符找到所有匹配的文件
	void findMatchFiles(std::string&pattern,std::vector<MiniFile*>&out);

	//移动另一个文件夹的子文件至当前文件夹
	bool moveFrom(MiniFolder*other,std::string&filename);
	
	bool isChildOf(MiniFolder*);//判断关系

private:
	bool loaded;
	std::unordered_map<std::string, MiniFile*>childs;
};

