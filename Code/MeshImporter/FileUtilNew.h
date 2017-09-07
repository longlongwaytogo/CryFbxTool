// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////
//  CryENGINE Source File
//  Copyright (C) 2000-2012, Crytek GmbH, All rights reserved
//////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <CryCore\Compiler\MSVCspecific.h>
#include <CryString/CryString.h>
#include <windows.h>

//#include <CryString/StringUtils.h>
//#include "dll_string.h"
//#include <CryCore/functor.h>
//#include "FileSystem/FileSystem_FileFilter.h"
//#include "FileDialogs/ExtensionFilter.h"

//#include <windows.h>

//! File types used for File Open dialogs
//enum ECustomFileType
//{
//	EFILE_TYPE_ANY,
//	EFILE_TYPE_GEOMETRY,
//	EFILE_TYPE_TEXTURE,
//	EFILE_TYPE_SOUND,
//	EFILE_TYPE_GEOMCACHE,
//	EFILE_TYPE_XML,
//	EFILE_TYPE_MATERIAL,
//	EFILE_TYPE_LAST,
//};

class  CFileUtil
{
public:
	//struct FileDesc
	//{
	//	string      filename;
	//	unsigned int attrib;
	//	time_t       time_create; //! -1 for FAT file systems
	//	time_t       time_access; //! -1 for FAT file systems
	//	time_t       time_write;
	//	int64        size;
	//};
	//enum ETextFileType
	//{
	//	FILE_TYPE_SCRIPT,
	//	FILE_TYPE_SHADER,
	//	FILE_TYPE_BSPACE,
	//};

	//enum ECopyTreeResult
	//{
	//	ETREECOPYOK,
	//	ETREECOPYFAIL,
	//	ETREECOPYUSERCANCELED,
	//	ETREECOPYUSERDIDNTCOPYSOMEITEMS,
	//};

	//enum EColumnType
	//{
	//	COLUMN_TYPE_FILELIST = 0,
	//	COLUMN_TYPE_DATE,
	//	COLUMN_TYPE_SIZE
	//};

public:

	//// Returns true if the files have the same content, false otherwise
	static bool CompareFiles(const string& strFilePath1, const string& strFilePath2);

	static bool GetDiskFileSize(const char* pFilePath, uint64& rOutSize);
	

};