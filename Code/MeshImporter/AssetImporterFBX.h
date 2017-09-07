// Copyright 2001-2016 Crytek GmbH. All rights reserved.

//#include <AssetSystem/AssetImporter.h>

#include <memory>
#include <fbxsdk.h>
#include <string>
#include "FbxScene.h"
namespace FbxTool
{

class CScene;
//struct SAnimationTake;
struct SNode;

} // namespace FbxTool
//
//
//
struct CAssetImportContext
{

};

class CAssetImporterFBX
{

public:

//
//	 std::vector<CAsset*> ImportAssets(const std::vector<string>& assetPaths, CAssetImportContext& ctx) ;
//	
//
	bool Init(const string& inputFilePath);
//
	FbxTool::CScene* GetScene();
//	string GetTempDirPath() const;
//
	void Import(const std::vector<string>& assetTypes, CAssetImportContext& ctx);
//
private:
	string m_fbxFilePath;
	std::unique_ptr<FbxTool::CScene> m_pFbxScene;
	string m_pTempDir;

	bool m_bCreateMaterial;
};
