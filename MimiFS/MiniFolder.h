#pragma once

class ConsoleApp;


//һ��������MiniFS �ļ���
class MiniFolder:public MiniFile
{
	friend ConsoleApp;
	friend MiniFile;
protected:
	MiniFolder();
	
public:
	virtual ~MiniFolder();

	//�������ļ�
	MiniFile* createChildFile(std::string& filename,int blockCount);

	//�������ļ���
	MiniFolder* createChildFolder(std::string& filename);

	MiniFile*& atChild(std::string&filename);//��ȡһ�����ļ�(��ȫ)
	MiniFile*& getChild(std::string&filename);//��ȡһ�����ļ�(����ȫ)

public:
	//Ŀ¼����

	void load();				//���������ļ�Ŀ¼
	void show();				//��ʾ�ļ�Ŀ¼	
	void updateDir();			//�������Ŀ¼
	void printTree(int depth);	//��ӡĿ¼��	
	void clear();				//���Ŀ¼

	bool isLoaded()const { return loaded; }//�ж��Ƿ����

public:
	//�������

	void showAtt()		override;	//��ʾ�ļ�����(�������ڣ���С)
	int deleteForever()	override;	//ɾ���ļ�
	int computeSize()	override;	//�����ļ���С
	std::string getAbsolutePath();	//��ȡ�ļ��о���·��

	//ʹ��ͨ����ҵ�����ƥ����ļ�
	void findMatchFiles(std::string&pattern,std::vector<MiniFile*>&out);

	//�ƶ���һ���ļ��е����ļ�����ǰ�ļ���
	bool moveFrom(MiniFolder*other,std::string&filename);
	
	bool isChildOf(MiniFolder*);//�жϹ�ϵ

private:
	bool loaded;
	std::unordered_map<std::string, MiniFile*>childs;
};

