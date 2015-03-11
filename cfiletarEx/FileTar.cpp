//Author :- Nish [nishforever@vsnl.com]

#include "stdafx.h"
#include "FileTar.h"


CFileTar::CFileTar()
{

}

CFileTar::~CFileTar()
{
	for(int i=1; i <= m_TarHeader.GetCount();i++)
		delete m_pTarIndex[i];

}

BOOL CFileTar::TarHeader::SetDescription(char *strdesc)
{
	if(strlen(strdesc)>511)
		return false;
	strcpy(TarDescription,strdesc);
	return true;
}

const char * CFileTar::TarHeader::GetDescription()
{	
	return TarDescription;
}

int CFileTar::TarHeader::GetCount()
{
	return FileCount;
}

int CFileTar::TarHeader::IncrementCount()
{
	if(FileCount<TAR_MAX_FILES)
		return ++FileCount;
	else
		return 0;
}

CFileTar::TarHeader::TarHeader()
{
	FileCount=0;
}

void CFileTar::SetFilePath(char *path)
{
	strcpy(FilePath,path);
	if(path[strlen(path)-1]!='\\')
		strcat(FilePath,"\\");

}

int CFileTar::AddFile(char *fname, char *desc)
{
	char fullpath[_MAX_PATH];
	strcpy(fullpath,FilePath);
	strcat(fullpath,fname);
	HANDLE hFile;
	DWORD size;
	hFile=CreateFileA(fullpath,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return 1;
	}
	size=GetFileSize(hFile,NULL);
	if(size==0xFFFFFFFF)
	{
		CloseHandle(hFile);
		return 1;
	}
	
	if(m_TarHeader.IncrementCount()==0)
	{
		CloseHandle(hFile);
		return 1;
	}
	TarIndex *pTarIndex=new TarIndex;
	strcpy(pTarIndex->FileName,fname);
	strcpy(pTarIndex->Description,desc);	
	pTarIndex->Size=size;
	
	m_pTarIndex[m_TarHeader.GetCount()]=pTarIndex;

	CloseHandle(hFile);
	return 0;
}

int CFileTar::CreateTar(char *TarFName, char *TarPath)
{
	char tarfullpath[_MAX_PATH];
	if(TarPath==NULL)
		strcpy(tarfullpath,FilePath);
	else
		strcpy(tarfullpath,TarPath);
	if(tarfullpath[strlen(tarfullpath)-1]!='\\')
		strcat(tarfullpath,"\\");
	strcat(tarfullpath,TarFName);
	int fdout;
	int ret;
	int i;
	if((fdout=_open(tarfullpath,_O_CREAT|_O_WRONLY|_O_BINARY|_O_TRUNC,
		_S_IWRITE))<0)
	{
		_close(fdout);
		return 1;
	}	
	ret=_write(fdout,&m_TarHeader,sizeof TarHeader);
	int ssize=m_TarHeader.GetCount()*sizeof (TarIndex)+sizeof(TarHeader);
	for(i=1;i<=m_TarHeader.GetCount();i++)
	{
		m_pTarIndex[i]->Start=ssize;
		ret=_write(fdout,m_pTarIndex[i],sizeof(TarIndex));
		ssize += m_pTarIndex[i]->Size;
	}
	for(i=1;i<=m_TarHeader.GetCount();i++)
	{
		char filetoadd[_MAX_PATH];
		strcpy(filetoadd,FilePath);
		strcat(filetoadd,m_pTarIndex[i]->FileName);		
		AppendFile(fdout,filetoadd);
	}
	
	_close(fdout);
	return 0;
}

int CFileTar::AppendFile(int fdout, char *fpath)
{
	int fdin;
	int len;
	if((fdin=_open(fpath,_O_RDONLY|_O_BINARY))<0)
	{
		_close(fdin);
		return 1;
	}
	char buffer[NBUFFSIZE];
	while((len=_read(fdin,buffer,NBUFFSIZE))>0)
	{
		_write(fdout,buffer,len);
	}
	_close(fdin);
	return 0;
}

const char * CFileTar::GetHeaderDescription()
{
	return m_TarHeader.GetDescription();

}

BOOL CFileTar::SetHeaderDescription(char *strdesc)
{
	return m_TarHeader.SetDescription(strdesc);
}


int CFileTar::GetTarInfo(char *TarFile,TarHeader *pTarHeader)
{
	int fdin;
	if((fdin=_open(TarFile,_O_BINARY|_O_RDONLY))<0)
	{
		_close(fdin);
		return 1;
	}
	memset(pTarHeader,0,sizeof TarHeader);
	_read(fdin,pTarHeader,sizeof TarHeader);

	_close(fdin);
	return 0;
}

int CFileTar::GetFileInfo(char *TarFile, TarIndex *pTarIndex, int index)
{
	int fdin;
	if((fdin=_open(TarFile,_O_BINARY|_O_RDONLY))<0)
	{
		_close(fdin);
		return 1;
	}
	TarHeader tmpTarHeader;	
	_read(fdin,&tmpTarHeader,sizeof TarHeader);	
	for(int i=1;i<=tmpTarHeader.GetCount();i++)
	{		
		_read(fdin,pTarIndex,sizeof TarIndex);
		
		if(i==index)
			break;
	}

	_close(fdin);
	return 0;

}

int CFileTar::UnTar(char *TarFile, int index, char *fpath)
{
	TarIndex ti;
	if(GetFileInfo(TarFile,&ti,index))
		return 1;
	int fdin,fdout;
	if((fdin=_open(TarFile,_O_BINARY|_O_RDONLY))<0)
	{
		_close(fdin);
		return 1;
	}
	if((fdout=_open(fpath,_O_CREAT|_O_WRONLY|_O_BINARY|_O_TRUNC,
		_S_IWRITE))<0)
	{
		_close(fdin);
		_close(fdout);
		return 1;
	}
	_lseek(fdin,ti.Start,SEEK_SET);
	long rem;
	char buff[NBUFFSIZE];
	rem=ti.Size;
	
	while(rem > NBUFFSIZE)
	{		
		_read(fdin,buff,NBUFFSIZE);
		_write(fdout,buff,NBUFFSIZE);
		rem -= NBUFFSIZE;
	}
	if(rem>0)
	{
		_read(fdin,buff,rem);
		_write(fdout,buff,rem);
	}

	_close(fdin);
	_close(fdout);
	return 0;
}

int CFileTar::UnTar(char *TarFile, char *dpath)
{
	TarHeader th;
	GetTarInfo(TarFile,&th);
	TarIndex ti;
	char fpath[_MAX_PATH];
	for(int i=1;i<=th.GetCount();i++)
	{
		GetFileInfo(TarFile,&ti,i);
		strcpy(fpath,dpath);
		if(fpath[strlen(fpath)-1]!='\\')
			strcat(fpath,"\\");
		strcat(fpath,ti.FileName);
		UnTar(TarFile,i,fpath);		
	}

	return 0;
}
