cmake_minimum_required (VERSION 3.6.0)
project (Game VERSION 1.0 LANGUAGES CXX)
SET(CMAKE_MODULE_PATH $ENV{CryEngineRoot}/Tools/cmake )
# visual studio version vs2015
set(CMAKE_GENERATOR "Visual Studio 14 2015" CACHE INTERNAL "" FORCE)



if (${CMAKE_GENERATOR} MATCHES "^Visual Studio")
	# Set startup project to launch Game.exe with this project
	configure_file ("Game.vcxproj.user.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.vcxproj.user")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

option(OPTION_PCH "Enable Precompiled headers" ON)



find_package (CryCommon REQUIRED)
find_package (CryAction REQUIRED)




set (SourceGroup_PluginMain
	"StdAfx.cpp"
	"StdAfx.h"
	"GamePlugin.cpp"
	"GamePlugin.h"
)
source_group("Main" FILES ${SourceGroup_PluginMain})

set (SourceGroup_Game
	"Game/GameRules.cpp"
	"Game/GameRules.h"
)
source_group("Game" FILES ${SourceGroup_Game})

set (SourceGroup_EntityHelpers
	"Entities/Helpers/ISimpleExtension.h"
)
source_group("Entities" FILES ${SourceGroup_EntityHelpers})

set (SourceGroup_Player
	"Player/Player.cpp"
	"Player/Player.h"
	"Player/ISimpleActor.h"
)
source_group("Player" FILES ${SourceGroup_Player})

set (SourceGroup_PlayerInput
	"Player/Input/PlayerInput.cpp"
	"Player/Input/PlayerInput.h"
	"Player/Input/InputEvent.h"
	"Player/Input/InputEvent.cpp"
	
)
source_group("Player\\Input" FILES ${SourceGroup_PlayerInput})

set (SourceGroup_PlayerMovement
	"Player/Movement/PlayerMovement.cpp"
	"Player/Movement/PlayerMovement.h"
)
source_group("Player\\Movement" FILES ${SourceGroup_PlayerMovement})

set (SourceGroup_PlayerView
	"Player/View/PlayerView.cpp"
	"Player/View/PlayerView.h"
)
source_group("Player\\View" FILES ${SourceGroup_PlayerView})

set(SourceGroup_MeshImporter
"MeshImporter/AssetImporterFBX.h"
"MeshImporter/AssetImporterFBX.cpp"
"MeshImporter/NodeProperties.cpp"
"MeshImporter/NodeProperties.h"
"MeshImporter/FbxUtil.h"
"MeshImporter/FbxUtil.cpp"
"MeshImporter/FbxScene.h"
"MeshImporter/FbxScene.cpp"
"MeshImporter/FbxMetaData.h"
"MeshImporter/FbxMetaData.cpp"
"MeshImporter/EditorMetaData.h"
"MeshImporter/EditorMetaData.cpp"
"MeshImporter/FbxSdkInclude.h"
"MeshImporter/AutoLodSettings.h"
"MeshImporter/AutoLodSettings.cpp"
"MeshImporter/GlobalImportSettings.h"
"MeshImporter/GlobalImportSettings.cpp"
"MeshImporter/SceneUserData.h"
"MeshImporter/SceneuserData.cpp"
"MeshImporter/CreateEngineMesh.cpp"
"MeshImporter/RcCaller.h"
"MeshImporter/RcCaller.cpp"
"MeshImporter/ImporterUtil.h"
"MeshImporter/ImporterUtil.cpp"
"MeshImporter/PathUtil.h"
"MeshImporter/PathUtil.cpp"
#"MeshImporter/FileUtil.cpp"
# "MeshImporter/FileUtil.h"
"MeshImporter/TextureHelpers.h"
"MeshImporter/TextureHelpers.cpp"
"MeshImporter/FileUtilNew.cpp"
"MeshImporter/FileUtilNew.h"


)

source_group("Importer" FILES ${SourceGroup_MeshImporter})


SET(SourceGroup_CGF 
	"MeshImporter/CGF/CGFLoader.cpp"
	"MeshImporter/CGF/CGFLoader.h"
	"MeshImporter/CGF/CGFSaver.cpp"
	"MeshImporter/CGF/CGFSaver.h"
	"MeshImporter/CGF/ChunkData.h"
	"MeshImporter/CGF/ChunkFile.cpp"
	"MeshImporter/CGF/ChunkFile.h"
	"MeshImporter/CGF/ChunkFileComponents.h"
	"MeshImporter/CGF/ChunkFileReaders.cpp"
	"MeshImporter/CGF/ChunkFileReaders.h"
	"MeshImporter/CGF/ChunkFileWriters.cpp"
	"MeshImporter/CGF/ChunkFileWriters.h"
	"MeshImporter/CGF/ReadOnlyChunkFile.cpp"
	"MeshImporter/CGF/ReadOnlyChunkFile.h"
	"MeshImporter/CGF/VClothSaver.cpp"
	"MeshImporter/CGF/VClothSaver.h"
	
	"MeshImporter/CGF/MeshCompiler.h"
	"MeshImporter/CGF/MeshCompiler.cpp"
	"MeshImporter/CGF/TangentSpaceCalculation.h"
	"MeshImporter/CGF/TangentSpaceCalculation.cpp"
	#"MeshImporter/CGF/TransformHelpers.cpp"
	"MeshImporter/CGF/TransformHelpers.h"
	"MeshImporter/CGF/ForsythFaceReorderer.h"
	"MeshImporter/CGF/ForsythFaceReorderer.cpp"
	"MeshImporter/CGF/mikktspace.cpp"
	
	)


SOURCE_GROUP("CGF" FILES ${SourceGroup_CGF})

set (SOURCE
	${SourceGroup_PluginMain}
	${SourceGroup_Game}
	${SourceGroup_EntityHelpers}
	${SourceGroup_Player}
	${SourceGroup_PlayerInput}
	${SourceGroup_PlayerMovement}
	${SourceGroup_PlayerView}
	${SourceGroup_MeshImporter}
	${SourceGroup_CGF}
)


if(OPTION_PCH AND MSVC)
	set (PRECOMPILED_SOURCE ${SOURCE})
	list(REMOVE_ITEM PRECOMPILED_SOURCE "StdAfx.cpp")

	set (PRECOMPILED_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pch)
	set_source_files_properties (${PRECOMPILED_SOURCE} PROPERTIES COMPILE_FLAGS "/Yu\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_DEPENDS "${PRECOMPILED_BINARY}")
	set_source_files_properties (StdAfx.cpp PROPERTIES COMPILE_FLAGS "/Yc\"StdAfx.h\" /Fp\"${PRECOMPILED_BINARY}\"" OBJECT_OUTPUTS "${PRECOMPILED_BINARY}")
endif (OPTION_PCH AND MSVC)

INCLUDE_Directories("D:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\include")
INCLUDE_Directories("D:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\include\\QtCore")
include_directories("D:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2017.1\\include")
include_directories("E:\\project\\CryEngine\\CRYENGINE-release5.3\\CRYENGINE-release\\Code\\SDKs\\yasli")
link_directories("D:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2017.1\\lib\\vs2015\\x64\\release")
LINK_DIRECTORIES("D:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\lib")

add_library(${PROJECT_NAME} MODULE ${SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE CryCommon CryAction)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE 
		Ntdll 
		"D:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2017.1\\lib\\vs2015\\x64\\debug\\libfbxsdk-md.lib"
		"E:\\project\\CryEngine\\CRYENGINE-release5.3\\CRYENGINE-release\\solutions_cmake\\win64\\Libs\\yasli\\debug\\yasli.lib"
		"D:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\lib\\Qt5Core.lib"
		"Qt5Gui.lib"
		"Qt5Widgets.lib"
		)
endif(WIN32)

#link_libraries(${CMAKE_PROJECT_NAME} "libfbxsdk-mt.lib")