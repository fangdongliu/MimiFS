#include "stdafx.h"
#include "HandleAtt.h"

using namespace std;
REGISTER_HANDLER(HandleAtt, "att", "显示文件(夹)属性", "att [-b] \"filepath\"", "[-b]:开启模糊匹配\nfilepath:目标路径")
namespace {

#define  addMusic(str) {str,FileType::Music}
#define  addTxt(str) {str,FileType::Txt}
#define  addCode(str) {str,FileType::Code}
#define  addImg(str) {str,FileType::Img}
#define  addSystem(str) {str,FileType::System}
#define  addData(str) {str,FileType::Data}
#define  addExe(str) {str,FileType::Exe}
#define  addZip(str) {str,FileType::Zip}
#define addVideo(str) {str,FileType::Video}

	enum class FileType {
		Unknown,
		Zip,
		Music,
		Img,
		Video,
		Data,
		Exe,
		Font,
		System,
		Txt,
		Code,
		Other
	};

	static std::unordered_map<std::string, FileType>typeMap = {
		addMusic("map3"),addMusic("aac"),addMusic("wav"),
		addMusic("wma"),addMusic("cda"),addMusic("flac"),
		addMusic("m4a"),addMusic("mid"),addMusic("mka"),
		addMusic("mp2"),addMusic("mpa"),addMusic("mpc"),addMusic("ape"),
		addMusic("ofr"),addMusic("ogg"),addMusic("ra"),addMusic("wv"),
		addMusic("tta"),addMusic("ac3"),addMusic("dts"),

		addTxt("odt"),addTxt("txt"),
		addTxt("ascii"),addTxt("chm"),addTxt("rtf"),addTxt("tex"),
		addTxt("ltx"),addTxt("doc"),addTxt("docx"),addTxt("ppt"),
		addTxt("pptx"),addTxt("inf"),addTxt("hlp"),addTxt("wps"),
		addTxt("odf"),addTxt("mht"),addTxt("mhtml"),addTxt("pdf"),

		addCode("htm"),addCode("html"),addCode("xhtml"),addCode("shtml"),
		addCode("cpp"),addCode("c"),addCode("hpp"),addCode("h"),
		addCode("php"),addCode("py"),addCode("cs"),addCode("asp"),
		addCode("jsp"),addCode("java"),addCode("class"),addCode("o"),
		addCode("hs"),addCode("fs"),addCode("css"),addCode("js"),addCode("less"),
		addCode("xml"),addCode("grail"),addCode("lui"),
		
		addImg("bmp"),addImg("gif"),addImg("ico"),addImg("ilbm"),addImg("lbm"),
		addImg("iff"),addImg("jpg"),addImg("jpeg"),addImg("png"),addImg("ppm"),
		addImg("psd"),addImg("tif"),addImg("tiff"),addImg("webp"),addImg("gifv"),
		addImg("svg"),addImg("ddt"),

		addSystem("lnk"),addSystem("tmp"),

		addData("db"),addData("csv"),addData("xls"),

		addExe("exe"),addExe("msi"),addExe("bin"),addExe("app"),
		addExe("dmg"),

		addVideo("avi"),addVideo("mp4"),addVideo("mov"),
		addVideo("flv"),addVideo("mpg"),addVideo("wmv"),
		addVideo("3gp"),addVideo("3g2"),addVideo("asf"),
		addVideo("f4v"),addVideo("ismv"),addVideo("m4v"),
		addVideo("mkv"),addVideo("mpeg"),

		addZip("zip"),addZip("rar"),addZip("7z"),
		addZip("tar"),addZip("gz")
	};
}

HandleAtt::HandleAtt()
{
}


HandleAtt::~HandleAtt()
{

}

void HandleAtt::onHandleCommand(Lexer&param) {

	string filepath;

	param >= filepath >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	bool blur = param.findSwitch("b");


	vector<string>out;
	Helper::cutPathFromString(filepath, out);
	if (blur) {
		string filename;

		if (out.size()) {
			filename = out.back();
			out.pop_back();

			auto folder = ConsoleApp::getInstance()->getFolderByPath(out);

			if (folder) {
				std::vector<MiniFile*>files;
				folder->findMatchFiles(filename, files);
				cout << "匹配文件数：" << files.size() << '\n';

				int maxFilenameLength = 0;

				for (auto i : files) {
					auto head = i->getFileHead();
					
					tm timeInfo;

					localtime_s(&timeInfo,&head.createTime);
					timeInfo.tm_year += 1900;

					printf("\n名称：%s\n", i->getFilename().c_str());
					if (!i->isFolder()) {
						const auto& str = i->getFilename();
						size_t pos = str.find_last_of('.');
						if (pos != str.npos) {
							auto suffix = str.substr(pos + 1);
							printf("后缀：\"%s\" ", suffix.c_str());

							for (auto&i : suffix) {
								if (i >= 'A'&&i <= 'Z') {
									i -= 'A' - 'a';
								}
							}
							switch (typeMap[suffix])
							{
							case FileType::Unknown:
								printf("未知格式");
								break;
							case FileType::Zip:
								printf("压缩文件");
								break;
							case FileType::Music:
								printf("音频文件");
								break;
							case FileType::Img:
								printf("图片");
								break;
							case FileType::Video:
								printf("视频文件");
								break;
							case FileType::Data:
								printf("数据文件");
								break;
							case FileType::Exe:
								printf("可执行文件");
								break;
							case FileType::Font:
								printf("字体文件");
								break;
							case FileType::System:
								printf("系统文件");
								break;
							case FileType::Txt:
								printf("文本文件");
								break;
							case FileType::Code:
								printf("代码文件");
								break;
							case FileType::Other:
								printf("其他文件");
								break;
							default:
								break;
							}

							printf("\n");
						}
						else {
							printf("后缀为空\n");
						}

						
					}
					printf("类型：文件%s\n", i->isFolder() ? "夹" : "");
					if (head.size > (1 << 20)) {
						printf("大小：%.2lf MB\n", head.size / 1024.0/1024.0);
					}
					else if (head.size > 1024) {
						printf("大小：%.2lf KB\n", head.size / 1024.0);
					}
					else {
						printf("大小：%d B\n", head.size);
					}
					printf("创建日期：%d-%-02d-%-02d %-02d:%-02d:%-02d\n", timeInfo.tm_year, timeInfo.tm_mon, timeInfo.tm_mday
						, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec
					);
				}
			}
			else {
				cout << "目标目录不存在\n";
			}
		}
	}
	else {
		try{
		auto file = ConsoleApp::getInstance()->getFileByPath(out);
			if (file) {
				auto head = file->getFileHead();

				tm timeInfo;

				localtime_s(&timeInfo, &head.createTime);
				timeInfo.tm_year += 1900;

				printf("名称：%s\n", file->getFilename().c_str());
				if (!file->isFolder()) {
					const auto& str = file->getFilename();
					size_t pos = str.find_last_of('.');
					if (pos != str.npos) {
						auto suffix = str.substr(pos + 1);
						printf("后缀：\"%s\" ", suffix.c_str());
						for (auto&i : suffix) {
							if (i >= 'A'&&i <= 'Z') {
								i -= 'A' - 'a';
							}
						}
						switch (typeMap[suffix])
						{
						case FileType::Unknown:
							printf("未知格式");
							break;
						case FileType::Zip:
							printf("压缩文件");
							break;
						case FileType::Music:
							printf("音频文件");
							break;
						case FileType::Img:
							printf("图片");
							break;
						case FileType::Video:
							printf("视频文件");
							break;
						case FileType::Data:
							printf("数据文件");
							break;
						case FileType::Exe:
							printf("可执行文件");
							break;
						case FileType::Font:
							printf("字体文件");
							break;
						case FileType::System:
							printf("系统文件");
							break;
						case FileType::Txt:
							printf("文本文件");
							break;
						case FileType::Code:
							printf("代码文件");
							break;
						case FileType::Other:
							printf("其他文件");
							break;
						default:
							break;
						}

						printf("\n");
					}
					else {
						printf("后缀为空\n");
					}


				}
				printf("类型：文件%s\n", file->isFolder() ? "夹" : "");
				if (head.size > (1 << 20)) {
					printf("大小：%.2lf MB\n", head.size / 1024.0 / 1024.0);
				}
				else if (head.size > 1024) {
					printf("大小：%.2lf KB\n", head.size / 1024.0);
				}
				else {
					printf("大小：%d B\n", head.size);
				}
				printf("创建日期：%d-%-02d-%-02d %-02d:%-02d:%-02d\n", timeInfo.tm_year, timeInfo.tm_mon, timeInfo.tm_mday
					, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec
				);
			}
		}
		catch (const PathNotExist&) {
			cout << "目标路径不存在\n";
		}
		catch (const exception&) {
			cout << "未找到目标文件\n";
		}


	}
	
}fdgffdkldfdnlkmdfgfhgsdfghsdsfghjsdfdgfhjbv,mdfghj,.,mndbnabsdjfgadfgfadfgnkmledefjirottremkeodeeirtuhyjtugtriekfoewwkirjtjhnrjekeopwejrthnrtkjekwoqwejrngjhgkfmlwmendfgmjdkmlmenrgnfngjkdmdlenfgbjkdljfgnrfdfdghjhgfddsfghjlrmyg,b vskeritbhiojmfd smedfngtnbhnthmkrltg,fkn bggjtnasdfdgfhgfdsdfgfhgjhgfdsdfghjhhgfdsdfghjhjghffdxdsfdrjkrml,vfmg ntgrmkl, mngtrgkmlrm, mngtgrjkml,f mvgntgkml,f