////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2001-2004.
// -------------------------------------------------------------------------
//  File name:   PathUtil.cpp
//  Version:     v1.00
//  Created:     4/11/2004 by Timur.
//  Compilers:   Visual Studio.NET 2003
//  Description: 
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "PathUtil.h"


namespace Path
{
	//////////////////////////////////////////////////////////////////////////
	void SplitPath(const string& rstrFullPathFilename,string& rstrDriveLetter,string& rstrDirectory,string& rstrFilename,string& rstrExtension)
	{
		string			strFullPathString(rstrFullPathFilename);
		string			strDriveLetter;
		string			strDirectory;
		string			strFilename;
		string			strExtension;

		char*				szPath((char*)strFullPathString.c_str());
		char*				pchLastPosition(szPath);
		char*       pchCurrentPosition(szPath);
		char*				pchAuxPosition(szPath);

		// Directory names or filenames containing ":" are invalid, so we can assume if there is a :
		// it will be the drive name.
		pchCurrentPosition=strchr(pchLastPosition,':');
		if (pchCurrentPosition==NULL)
		{
			rstrDriveLetter="";
		}
		else
		{
			strDriveLetter.assign(pchLastPosition,pchCurrentPosition+1);
			pchLastPosition=pchCurrentPosition+1;
		}

		pchCurrentPosition=strrchr(pchLastPosition,'\\');
		pchAuxPosition=strrchr(pchLastPosition,'/');
		if ((pchCurrentPosition==NULL)&&(pchAuxPosition==NULL))
		{
			rstrDirectory="";
		}
		else
		{
			// Since NULL is < valid pointer, so this will work.
			if (pchAuxPosition>pchCurrentPosition)
			{
				pchCurrentPosition=pchAuxPosition;
			}
			strDirectory.assign(pchLastPosition,pchCurrentPosition+1);
			pchLastPosition=pchCurrentPosition+1;
		}

		pchCurrentPosition=strrchr(pchLastPosition,'.');
		if (pchCurrentPosition==NULL)
		{
			rstrExtension="";
			strFilename.assign(pchLastPosition);
		}
		else
		{
			strExtension.assign(pchCurrentPosition);
			strFilename.assign(pchLastPosition,pchCurrentPosition);
		}

		rstrDriveLetter=strDriveLetter;
		rstrDirectory=strDirectory;
		rstrFilename=strFilename;
		rstrExtension=strExtension;
	}
	//////////////////////////////////////////////////////////////////////////
	void GetDirectoryQueue(const string& rstrSourceDirectory,std::vector<string>& rcstrDirectoryTree)
	{
		string						strCurrentDirectoryName;
		string						strSourceDirectory(rstrSourceDirectory);
		const char*				szSourceDirectory(strSourceDirectory.c_str());
		const char*				pchCurrentPosition(szSourceDirectory);
		const char*				pchLastPosition(szSourceDirectory);
		const char*				pchAuxPosition(szSourceDirectory);

		rcstrDirectoryTree.clear();

		if (strSourceDirectory.empty())
		{
			return;
		}

		// It removes as many slashes the path has in its start...
		// MAYBE and just maybe we should consider paths starting with
		// more than 2 slashes invalid paths...
		while ((*pchLastPosition=='\\')||(*pchLastPosition=='/'))
		{
			++pchLastPosition;
			++pchCurrentPosition;
		}

		do 
		{
			pchCurrentPosition=strpbrk(pchLastPosition,"\\/");
			if (pchCurrentPosition==NULL)
			{
				break;
			}
			strCurrentDirectoryName.assign(pchLastPosition,pchCurrentPosition);
			pchLastPosition=pchCurrentPosition+1;
			// Again, here we are skipping as many consecutive slashes.
			while ((*pchLastPosition=='\\')||(*pchLastPosition=='/'))
			{
				++pchLastPosition;
			}
			

			rcstrDirectoryTree.push_back((string)strCurrentDirectoryName);

		} while (true);
	}
	//////////////////////////////////////////////////////////////////////////
	void ConvertSlashToBackSlash(string& rstrStringToConvert)
	{
		int nCount(0);
		int nTotal(0);

		string	strStringToConvert(rstrStringToConvert);

		nTotal=strStringToConvert.size();
		for (nCount=0;nCount<nTotal;++nCount)
		{
			if (strStringToConvert[nCount]=='/')
			{
				strStringToConvert.replace(nCount,1,1,'\\');
			}
		}
		rstrStringToConvert=strStringToConvert;
		rstrStringToConvert = CaselessPaths(rstrStringToConvert);
	}
	//////////////////////////////////////////////////////////////////////////
	void ConvertBackSlashToSlash(string& rstrStringToConvert)
	{
		int nCount(0);
		int nTotal(0);

		string	strStringToConvert(rstrStringToConvert);

		nTotal=strStringToConvert.size();
		for (nCount=0;nCount<nTotal;++nCount)
		{
			if (strStringToConvert[nCount]=='\\')
			{
				strStringToConvert.replace(nCount,1,1,'/');
			}
		}	
		rstrStringToConvert=strStringToConvert;
		rstrStringToConvert = CaselessPaths(rstrStringToConvert);
	}
	//////////////////////////////////////////////////////////////////////////
	void SurroundWithQuotes(string& rstrSurroundString)
	{
		string strSurroundString(rstrSurroundString);

		if (!strSurroundString.empty())
		{
			if (strSurroundString[0]!='\"')
			{
				strSurroundString.insert(0,"\"");
			}
			if (strSurroundString[strSurroundString.size()-1]!='\"')
			{
				strSurroundString.insert(strSurroundString.size(),"\"");
			}
		}
		else
		{
			strSurroundString.insert(0,"\"");
			strSurroundString.insert(strSurroundString.size(),"\"");
		}
		rstrSurroundString=strSurroundString;
	}
	//////////////////////////////////////////////////////////////////////////
	void GetParentDirectoryString(string& strInputParentDirectory)
	{
		size_t	nLastFoundSlash(string::npos);
		size_t	nFirstFoundNotSlash(string::npos);

		string strTempInputParentDirectory(strInputParentDirectory);

		nFirstFoundNotSlash=strTempInputParentDirectory.find_last_not_of("\\/",nLastFoundSlash);
		// If we can't find a non-slash caracter, this is likely to be a mal formed path...
		// ...so we won't be able to determine a parent directory, if any.
		if (nFirstFoundNotSlash==string::npos)
		{
			return;
		}

		nLastFoundSlash=strTempInputParentDirectory.find_last_of("\\/",nFirstFoundNotSlash);
		// If we couldn't find any slash, this might be the root folder...and the root folder
		// has no parent at all.
		if (nLastFoundSlash==string::npos)
		{
			return;
		}
		
		strTempInputParentDirectory.erase(nLastFoundSlash,string::npos);

		strInputParentDirectory=strTempInputParentDirectory;

		return;
	}
	//////////////////////////////////////////////////////////////////////////
	string RemoveRelativeParts(const string& path)
	{
		string strPath = path;
		// Remove all dir/..
		int nBack;
		while ((nBack = strPath.Find("\\..")) > 0)
		{
			int nPrev;
			for (nPrev = nBack-1; nPrev > 0; nPrev--)
			{
				if (strPath[nPrev] == '\\')
					break;
			}
			strPath.Delete(nPrev, nBack+3 - nPrev);
		}

		// Remove all ./
		strPath.Replace(".\\", "");
		if (!strPath.IsEmpty() && strPath[0] == '\\')
			strPath = strPath.Mid(1);

		// Repeat for unix
		while ((nBack = strPath.Find("/..")) > 0)
		{
			int nPrev;
			for (nPrev = nBack-1; nPrev > 0; nPrev--)
			{
				if (strPath[nPrev] == '/')
					break;
			}
			strPath.Delete(nPrev, nBack+3 - nPrev);
		}

		// Remove all ./
		strPath.Replace("./", "");
		if (!strPath.IsEmpty() && strPath[0] == '/')
			strPath = strPath.Mid(1);
		return strPath;
	}
	//////////////////////////////////////////////////////////////////////////
	string GetExecutableFullPath()
	{
		char moduleFileName[512];
		GetModuleFileName(NULL,moduleFileName,512);
		return RemoveRelativeParts(moduleFileName);
	}
	//////////////////////////////////////////////////////////////////////////
	string GetWindowsTempDirectory()
	{
		DWORD									nCurrentTempPathSize(128);
		DWORD									nReturnedPathSize(0);
		std::vector<TCHAR>		cTempStringBuffer;

		do 
		{
			nCurrentTempPathSize=nCurrentTempPathSize<<2;
			cTempStringBuffer.resize(nCurrentTempPathSize,0);
			nReturnedPathSize=GetTempPath(nCurrentTempPathSize-1,&cTempStringBuffer.front());
		} while (nCurrentTempPathSize<=nReturnedPathSize);
		return &cTempStringBuffer.front();
	}
	//////////////////////////////////////////////////////////////////////////
	string GetExecutableParentDirectory()
	{
		string strExecutablePath;
		string strDriveLetter;
		string strDirectory;
		string strFilename;
		string strExtension;
		string strReturnValue;

		strExecutablePath=GetExecutableFullPath();
		SplitPath(strExecutablePath,strDriveLetter,strDirectory,strFilename,strExtension);
		strReturnValue = strDriveLetter;
		strReturnValue += strDirectory;	
		GetParentDirectoryString(strReturnValue);

		return strReturnValue;
	}
	//////////////////////////////////////////////////////////////////////////
	wstring GetExecutableParentDirectoryUnicode()
	{
		wstring strReturnValue;
		WCHAR sBufferW[MAX_PATH];
		DWORD dwDirLen = GetCurrentDirectoryW(sizeof(sBufferW), sBufferW);
		strReturnValue = sBufferW;

		return strReturnValue;
	}
	//////////////////////////////////////////////////////////////////////////
	string& ReplaceFilename( const string &strFilepath,const string &strFilename,string& strOutputFilename)
	{
		string strDriveLetter;
		string strDirectory;
		string strOriginalFilename;
		string strExtension;

		SplitPath(strFilepath,strDriveLetter,strDirectory,strOriginalFilename,strExtension);

		strOutputFilename=strDriveLetter;
		strOutputFilename+=strDirectory;
		strOutputFilename+=strFilename;
		strOutputFilename+=strExtension;
		
		strOutputFilename = CaselessPaths(strOutputFilename);
		return strOutputFilename;
	}

	bool IsFolder(const char* pPath)
	{
		DWORD attrs = GetFileAttributes(pPath);

		if (attrs == FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}

		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	string GetUserSandboxFolder()
	{
		string outPath;
		char path[ICryPak::g_nMaxPath];
		path[sizeof(path) - 1] = 0;
		gEnv->pCryPak->AdjustFileName("%USER%/Sandbox", path, 
			ICryPak::FLAGS_PATH_REAL|ICryPak::FLAGS_FOR_WRITING|ICryPak::FLAGS_ADD_TRAILING_SLASH);
		const size_t len = strlen(path);
		if (len == 0 || (path[len-1] != '\\' && path[len-1] != '/'))
		{
			strcat_s(path,sizeof(path), "\\");
		}
		if (strstr(path, ":\\") == NULL)
		{
			string fullPath("\\");
			fullPath += path;
			CryGetCurrentDirectory(ICryPak::g_nMaxPath, path);
			outPath = path; 
			outPath += fullPath;
		}
		else
		{
			outPath = path;
		}
		return outPath;
	}





}
