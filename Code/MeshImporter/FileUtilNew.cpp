// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#include "stdAfx.h"
#include "FileUtilNew.h"

 bool CFileUtil::CompareFiles(const string& strFilePath1, const string& strFilePath2)
{
		// Get the size of both files.  If either fails we say they are different (most likely one doesn't exist)
		uint64 size1, size2;
		if (!GetDiskFileSize(strFilePath1, size1) || !GetDiskFileSize(strFilePath2, size2))
			return false;

		// If the files are different sizes return false
		if (size1 != size2)
			return false;

		// Sizes are the same, we need to compare the bytes.  Try to open both files for read.
		CCryFile file1, file2;
		if (!file1.Open(strFilePath1, "rb") || !file2.Open(strFilePath2, "rb"))
			return false;

		const uint64 bufSize = 4096;

		char buf1[bufSize], buf2[bufSize];

		for (uint64 i = 0; i < size1; i += bufSize)
		{
			size_t amtRead1 = file1.ReadRaw(buf1, bufSize);
			size_t amtRead2 = file2.ReadRaw(buf2, bufSize);

			// Not a match if we didn't read the same amount from each file
			if (amtRead1 != amtRead2)
				return false;

			// Not a match if we didn't read the amount of data we expected
			if (amtRead1 != bufSize && i + amtRead1 != size1)
				return false;

			// Not a match if the buffers aren't the same
			if (memcmp(buf1, buf2, amtRead1) != 0)
				return false;
		}

		return true;
	}

bool CFileUtil::GetDiskFileSize(const char* pFilePath, uint64& rOutSize)
	{
		BOOL bOK;
		WIN32_FILE_ATTRIBUTE_DATA fileInfo;

#ifndef MAKEQWORD
#define MAKEQWORD(lo, hi) ((uint64)(((uint64) ((DWORD) (hi))) << 32 | ((DWORD) (lo))))
#endif

		bOK = ::GetFileAttributesEx(pFilePath, GetFileExInfoStandard, (void*)&fileInfo);

		if (!bOK)
		{
			return false;
		}

		rOutSize = MAKEQWORD(fileInfo.nFileSizeLow, fileInfo.nFileSizeHigh);

		return true;
	}


