//Author :- Nish [nishforever@vsnl.com]

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#define TAR_MAX_FILES 1024
#define NBUFFSIZE 8092
class CFileTar  
{
public:
	struct TarIndex
	{
		long Start;
		long Size;
		char FileName[_MAX_FNAME];
		char Description[256];
	};
	struct TarHeader
	{
	private:
		char TarDescription[512];
		int FileCount;
	public:
		TarHeader();
		int IncrementCount();
		int GetCount();
		const char * GetDescription();
		BOOL SetDescription(char *strdesc);
	};
public:	
	static int UnTar(char *TarFile, char *dpath);
	static int UnTar(char *TarFile, int index, char *fpath);
	static int GetFileInfo(char *TarFile, TarIndex *pTarIndex, int index);
	static int GetTarInfo(char *TarFile,TarHeader *pTarHeader);
	BOOL SetHeaderDescription(char *strdesc);
	const char * GetHeaderDescription();
	int CreateTar(char *TarFName, char *TarPath = NULL);
	int AddFile(char *fname, char *desc);
	void SetFilePath(char *path);
	CFileTar();
	virtual ~CFileTar();
private:
	TarHeader m_TarHeader;
	TarIndex* m_pTarIndex[TAR_MAX_FILES];
	char FilePath[_MAX_PATH];
	int AppendFile(int fdout, char *fpath);
};
