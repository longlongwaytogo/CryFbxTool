////////////////////////////////////////////////////////////////////////////
//
//  Crytek Engine Source File.
//  Copyright (C), Crytek Studios, 2002.
// -------------------------------------------------------------------------
//  File name:   pathutil.h
//  Version:     v1.00
//  Created:     5/11/2002 by Timur.
//  Compilers:   Visual Studio.NET
//  Description: Utility functions to simplify working with paths.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __pathutil_h__
#define __pathutil_h__
#pragma once

#include <CrySystem/file/ICryPak.h>
#include <shlwapi.h>

#include <CrySystem/IValidator.h>
#define SANDBOX_API 

namespace Path
{
	//! converts path to lowercase given the cvar for ed_lowercasepaths
	inline string CaselessPaths(const string& strPath )
	{
		
		ICVar * pCvar = gEnv->pConsole->GetCVar("ed_lowercasepaths");
		if ( pCvar )
		{
			int uselowercase = pCvar->GetIVal();
			if ( uselowercase )
			{
				string str = strPath;
				str = str.MakeLower();
				return str;
			}
		}
		return strPath;
	}

	//! Convert a path to the uniform form.
	inline string ToUnixPath( const string& strPath )
	{
		string str = strPath;
		str.Replace( '\\','/' );
		return CaselessPaths(str);
	}

	//! Split full file name to path and filename
	//! @param filepath [IN] Full file name inclusing path.
	//! @param path [OUT] Extracted file path.
	//! @param file [OUT] Extracted file (with extension).
	
	inline void Split( const string &filepath,string &path,string &file )
	{
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath( filepath,drive,dir,fname,ext );
		_makepath( path_buffer,drive,dir,0,0 );
		path = path_buffer;
		_makepath( path_buffer,0,0,fname,ext );
		file = path_buffer;
	}

	//! Split full file name to path and filename
	//! @param filepath [IN] Full file name inclusing path.
	//! @param path [OUT] Extracted file path.
	//! @param filename [OUT] Extracted file (without extension).
	//! @param ext [OUT] Extracted files extension.

	inline void Split( const string &filepath,string &path,string &filename,string &fext )
	{
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath( filepath,drive,dir,fname,ext );
		_makepath( path_buffer,drive,dir,0,0 );
		path = path_buffer;
		filename = fname;
		fext = ext;
	}

	//! Split path into segments
	//! @param filepath [IN] path
	inline std::vector<string> SplitIntoSegments( const string &path )
	{
		std::vector<string> splittedPath;
		int currentPos = 0;
		for (string token = path.Tokenize( "/\\", currentPos ); !token.IsEmpty();
			token = path.Tokenize( "/\\", currentPos ) ) 		
			splittedPath.push_back(token);
		return splittedPath;
	}

	//! Extract extension from full specified file path.
	inline string GetExt( const string &filepath )
	{
		char ext[_MAX_EXT];
		_splitpath( filepath,0,0,0,ext );
		if (ext[0] == '.')
			return ext+1;
		
		return ext;
	}

	//! Extract path from full specified file path.
	inline string GetPath( const string &filepath )
	{
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		_splitpath( filepath,drive,dir,0,0 );
		_makepath( path_buffer,drive,dir,0,0 );
		return CaselessPaths(path_buffer);
	}

	//! Extract file name with extension from full specified file path.
	inline string GetFile( const string &filepath )
	{
		char path_buffer[_MAX_PATH];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		_splitpath( filepath,0,0,fname,ext );
		_makepath( path_buffer,0,0,fname,ext );
		return CaselessPaths(path_buffer);
	}

	//! Extract file name without extension from full specified file path.
	inline string GetFileName( const string &filepath )
	{
		char fname[_MAX_FNAME];
		_splitpath( filepath,0,0,fname,0 );
		return fname;
	}

	//! Removes the trailing backslash from a given path.
	inline string RemoveBackslash( const string &path )
	{
		if (path.IsEmpty())
			return path;

		int iLenMinus1 = path.GetLength()-1;
		char cLastChar = path[iLenMinus1];
		
		if(cLastChar=='\\' || cLastChar == '/')
			return path.Mid( 0,iLenMinus1 );

		return CaselessPaths(path);
	}

	//! add a backslash if needed
	inline string AddBackslash( const string &path )
	{
		if(path.IsEmpty() || path[path.GetLength()-1] == '\\')
			return path;

		return CaselessPaths(path + "\\");
	}
	//! add a slash if needed
	inline string AddSlash( const string &path )
	{
		if(path.IsEmpty() || path[path.GetLength()-1] == '/')
			return path;

		return CaselessPaths(path + "/");
	}
	
	template<size_t size>
	inline void AddBackslash( CryStackStringT<char, size>* path )
	{
		if (path->empty() || (*path)[path->size() - 1] != '\\')
			(*path) += '\\';
	}

	template<size_t size>
	inline void AddSlash( CryStackStringT<char, size>* path )
	{
		if (path->empty() || (*path)[path->size() - 1] != '/')
			(*path) += '/';
	}

	inline string AddPathSlash( const string& path )
	{
		if((path.IsEmpty()) || (path[path.GetLength()-1] == '/') || (path[path.GetLength()-1] == '\\'))
			return path;

		return CaselessPaths(path + "/");
	}

	//! Replace extension for given file.
	inline string ReplaceExtension( const string &filepath,const string &ext )
	{
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		_splitpath( filepath,drive,dir,fname,0 );
		_makepath( path_buffer,drive,dir,fname,ext );
		return CaselessPaths(path_buffer);
	}

	//! Replace extension for given file.
	inline string RemoveExtension( const string &filepath )
	{
		char path_buffer[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		_splitpath( filepath,drive,dir,fname,0 );
		_makepath( path_buffer,drive,dir,fname,0 );
		return path_buffer;
	}

	//! Makes a fully specified file path from path and file name.
	inline string Make( const string &path,const string &file )
	{
		if (gEnv->pCryPak->IsAbsPath(file))
			return file;
		return CaselessPaths(AddBackslash(path) + file);
	}

	//! Makes a fully specified file path from path and file name.
	inline string Make( const string &dir,const string &filename,const string &ext )
	{
		char path_buffer[_MAX_PATH];
		_makepath( path_buffer,NULL,dir,filename,ext );
		return CaselessPaths(path_buffer);
	}

	//////////////////////////////////////////////////////////////////////////
	inline string GetRelativePath( const string &fullPath, bool bRelativeToGameFolder = false )
	{
		if (fullPath.IsEmpty())
			return "";

		char rootpath[_MAX_PATH];
		GetCurrentDirectory(sizeof(rootpath),rootpath);
		if( bRelativeToGameFolder == true )
		{
			string gameFolder = PathUtil::GetGameFolder().c_str();
			if( gameFolder.Find(':') == -1 )
			{
				strcat(rootpath, "\\");
				strcat(rootpath, gameFolder);
			}
			else
			{
				strcpy_s(rootpath, gameFolder);
			}
		}

		// Create relative path
		char path[_MAX_PATH];
		char srcpath[_MAX_PATH];
		strcpy( srcpath,fullPath );
		if (FALSE == PathRelativePathTo( path,rootpath,FILE_ATTRIBUTE_DIRECTORY,srcpath, NULL))
			return fullPath;

		string relPath = path;
		relPath.TrimLeft("\\/.");
		return CaselessPaths(relPath);
	}

	//////////////////////////////////////////////////////////////////////////
	// Description:
	// given the source relative game path, constructs the full path to the file according to the flags.
	// Ex. Objects/box will be converted to C:\Test\Game\Objects\box.cgf
	inline string GamePathToFullPath( const string &path, bool bForWriting = false )
	{
		char szAdjustedFile[ICryPak::g_nMaxPath];
		const char *szTemp = gEnv->pCryPak->AdjustFileName( path,szAdjustedFile,bForWriting ? ICryPak::FLAGS_FOR_WRITING : 0);

		return CaselessPaths(szAdjustedFile);
	}

	//////////////////////////////////////////////////////////////////////////
	inline string FullPathToGamePath( const string &path )
	{
		return CaselessPaths(GetRelativePath(path, true));
	}

	//////////////////////////////////////////////////////////////////////////
	// Description:
	//    Make a game correct path out of any input path.
	//	  bRelativeToGameFolder = false; Relative to the root directory
			// e.g input: J:\Games\CrysisWars2\GameCrysis2\Objects\TestObject\TestObjectDiff.dds
			// output: GameCrysis2\Objects\TestObject\TestObjectDiff.dds
	//    bRelativeToGameFolder = true; relative to the project specific game folder
			// e.g input: J:\Games\CrysisWars2\GameCrysis2\Objects\TestObject\TestObjectDiff.dds
			// output: Objects\TestObject\TestObjectDiff.dds

	inline string MakeGamePath( const string &path, bool bRelativeToGameFolder = false )
	{
		string fullpath = Path::GamePathToFullPath(path);
		string rootDataFolder = Path::AddBackslash(PathUtil::GetGameFolder().c_str());
		if (fullpath.GetLength() > rootDataFolder.GetLength() && strnicmp(fullpath,rootDataFolder,rootDataFolder.GetLength()) == 0)
		{
			fullpath = fullpath.Right(fullpath.GetLength()-rootDataFolder.GetLength());
			fullpath.Replace('\\','/'); // Slashes use for game files.
			return fullpath;
		}
		fullpath = GetRelativePath(path,bRelativeToGameFolder);
		if (fullpath.IsEmpty())
		{
			fullpath = path;
		}
		fullpath.Replace('\\','/'); // Slashes use for game files.
		return CaselessPaths(fullpath);
	}

	inline string GetGameFolder()
	{
		return gEnv->pCryPak->GetGameFolder();
	}

	inline string GetAudioLocalizationFolder(bool const bIncludeGameFolder) 
	{
		// Omit the trailing slash!
		string sLocalizationFolder(string(PathUtil::GetLocalizationFolder()).Left(static_cast<int>(PathUtil::GetLocalizationFolder().size()) - 1));

		if (!sLocalizationFolder.IsEmpty())
		{
			if (sLocalizationFolder.CompareNoCase("languages") != 0)
			{
				// If a different localization folder is set it is a requirement
				// that this folder resides right next to the game folder
				// hence rendering the include of the game folder unnecessary.
				sLocalizationFolder += "/dialog/";
			}
			else
			{
				sLocalizationFolder = bIncludeGameFolder ? (Path::GetGameFolder() + "/languages/dialog/") : "/languages/dialog/";
			}
		}
		else
		{
			CryLog("err:%s", "The localization folder is not set!Please make sure it is by checking the setting of cvar \"sys_localization_folder");
			//gEnv->pSystem->Warning(VALIDATOR_MODULE_EDITOR, VALIDATOR_WARNING, "errfile", "The localization folder is not set! Please make sure it is by checking the setting of cvar \"sys_localization_folder\"!");
		}

		return sLocalizationFolder;
	}

	string GetEditingGameDataFolder();
	string GetModDataFolder();
	//! creates a MOD path from a relative game path, used for saving game files to current mod folder
	//! Example: Libs/Some/tokens.xml to Mods/MyMod/GameCrysis2/Libs/Some/tokens.xml
	string MakeModPathFromGamePath( const string& rRelGamePath );
	string GetUserSandboxFolder();
	string RemoveRelativeParts(const string& path);

	// This had to be created because _splitpath is too dumb about console drives.
	SANDBOX_API void SplitPath(const string& rstrFullPathFilename, string& rstrDriveLetter, string& rstrDirectory,string& rstrFilename,string& rstrExtension);

	// Requires a path from Splithpath: no drive letter and backslash at the end.
	SANDBOX_API void GetDirectoryQueue(const string& rstrSourceDirectory,std::vector<string>& rcstrDirectoryTree);

	// Converts all slashes to backslashes so MS things won't complain.
	SANDBOX_API void ConvertSlashToBackSlash(string& rstrStringToConvert);

	// Converts backslashes into forward slashes.
	SANDBOX_API void ConvertBackSlashToSlash(string& rstrStringToConvert);

	// Surrounds a string with quotes if necessary. This is useful for calling other programs.
	SANDBOX_API void SurroundWithQuotes(string& rstrSurroundString);

	// This function takes a directory string input and returns the parent director string.
	SANDBOX_API void GetParentDirectoryString(string& strInputParentDirectory);

  // Gets the temporary directory path (which may not exist).
	SANDBOX_API string GetWindowsTempDirectory();

	// This function returns the full path used to run the editor.
	SANDBOX_API string GetExecutableFullPath();

	// This function returns the directory above Bin32 or Bin64 from which the editor was run (does not work with Unicode paths).
	SANDBOX_API string GetExecutableParentDirectory();

	// This function returns the directory above Bin32 or Bin64 from which the editor was run (also works with Unicode paths).
	SANDBOX_API wstring GetExecutableParentDirectoryUnicode();

	// This function replaces the filename from a path, keeping extension and directory/drive path.
	// WARNING: do not use the same variable in the last parameter and in any of the others.
	SANDBOX_API string& ReplaceFilename( const string &strFilepath,const string &strFilename,string& strOutputFilename);

	//! \return true if the given path is a folder and not a file
	SANDBOX_API bool IsFolder(const char* pPath);
};

inline string operator /( const string &first, const string &second )
{
	return Path::Make(first, second);
}


#endif // __pathutil_h__
