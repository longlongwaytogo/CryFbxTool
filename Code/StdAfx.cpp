// CryEngine Source File
// Copyright (C), Crytek, 1999-2016

#include "StdAfx.h"
//#include <CryCore/Typeinfo_impl.h>
//#include <Cry3DEngine/CGF/CryHeaders_info.h>

//#include <Cry3DEngine/CGF/CryHeaders.h>
//#include <Cry3DEngine/CGF/CGFContent.h>
//#include <Cry3DEngine/CGF/CGFContent_info.h>

//#include <CryMath/CryHalf_info.h>
//#include <CryMath/Cry_Math.h>
#include <CryCore/Common_TypeInfo.h>
#include <CryCore/Common_TypeInfo2.h>

//template struct Color_tpl<unsigned char>;



//SMeshTexCoord

#include <CryCore/TypeInfo_impl.h>
#include <Cry3DEngine/IIndexedMesh_info.h>
#include <CryMath/Cry_Color.h>
//template struct Color_tpl<uint8>;
//STRUCT_INFO_T_INSTANTIATE(Color_tpl, <uint8>)
STRUCT_INFO_T_INSTANTIATE(Color_tpl, <unsigned char>)

#if ENABLE_TYPE_INFO_NAMES

#pragma message ( "ENABLE_TYPE_INFO " )
#else
#pragma message ( " no ENABLE_TYPE_INFO " )
#endif 
