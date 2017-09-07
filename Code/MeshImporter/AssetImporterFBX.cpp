// Copyright 2001-2016 Crytek GmbH. All rights reserved.

#include "StdAfx.h"
#include "AssetImporterFBX.h"

#include "FbxScene.h"
#include "FbxMetaData.h"
#include "RcCaller.h"
#include "TextureHelpers.h"
#include <Cry3DEngine/I3DEngine.h>

#include <windows.h>

#include <QTemporaryFile>
#include "ImporterUtil.h"

using namespace QtUtil;
// REGISTER_ASSET_IMPORTER(CAssetImporterFBX)

struct CAsset
{

};
namespace MeshImporter
{

	std::string CreateCHPARAMS(const std::string& path)
	{

			const std::string animPath = !path.empty() ? path : std::string(".");

			std::string szFormat = 
				"<Params>\n"
				" <AnimationList>\n"
				"  <Animation name=\"#filepath\" path=\""  + path + "\"/>\n"
				"  <Animation name=\"*\" path=\"*/*.caf\"/>\n"
				"  <Animation name=\"*\" path=\"*/*.bspace\"/>\n"
				"  <Animation name=\"*\" path=\"*/*.comb\"/>\n"
				" </AnimationList>\n"
				"</Params>\n";
			return szFormat;
		}


} // namespace MeshImporter

bool ReadMetaDataFromFile(const std::string& filePath, FbxMetaData::SMetaData& metaData)
// Reads meta data from either .cgf or .json file (in that order).
{
	//if (!metaData.pEditorMetaData)
	//{
	//	metaData.pEditorMetaData.reset(new SEditorMetaData());
	//}

	//CChunkFile cf;
	//if (cf.Read(filePath.c_str()))
	//{
	//	// Reading JSON data from ChunkType_ImportSettings chunk of a chunk file.
	//	const IChunkFile::ChunkDesc* const pChunk = cf.FindChunkByType(ChunkType_ImportSettings);
	//	if (pChunk)
	//	{
	//		const string json((const char*)pChunk->data, (size_t)pChunk->size);
	//		return metaData.FromJson(json);
	//	}
	//	else
	//	{
	//		LogPrintf("ChunkType_ImportSettings chunk (JSON) is missing in '%s'", QtUtil::ToString(filePath).c_str());
	//	}
	//}
	//else
	//{
	//	// Reading JSON data from .json file.
	//	QFile file(filePath);
	//	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	//	{
	//		QTextStream inStream(&file);
	//		const string json = QtUtil::ToString(inStream.readAll());

	//		file.close();
	//		return metaData.FromJson(json);
	//	}
	//	else
	//	{
	//		LogPrintf("unable to open file '%s' for reading.\n", QtUtil::ToString(filePath).c_str());
	//	}
	//}

	//CQuestionDialog::SWarning("Cannot open .cgf file", "Only .cgf files that have been created from .fbx can be opened.");
	return false;

}


namespace Private_AssetImporterFBX
{

	class CRcListener : public CRcCaller::IListener
	{
	public:
		static CRcListener& GetInstance()
		{
			static CRcListener theInstance;
			return theInstance;
		}

		virtual void ShowWarning(const string& message) override
		{
			CryWarning(VALIDATOR_MODULE_ASSETS, VALIDATOR_WARNING, "%s", message.c_str());
		}

		virtual void ShowError(const string& message) override {}
	};

	string ImportFbxFile(const string& inputFilePath,  string& outputDirectoryPath)
	{
		if (inputFilePath.empty() || outputDirectoryPath.empty())
		{
			return string();
		}

		string outputFilePath = PathUtil::Make(outputDirectoryPath, PathUtil::GetFile(inputFilePath.c_str()));
		if (CopyNoOverwrite(string(inputFilePath.c_str()), string(outputFilePath.c_str())))
		{
			CryWarning(VALIDATOR_MODULE_ASSETS, VALIDATOR_ERROR, "Importing FBX file '%s' failed.", inputFilePath.c_str());
			return string();
		}

		return outputFilePath;
	}

	std::vector<const FbxTool::SNode*> FindAllSkinNodes(const FbxTool::CScene* pFbxScene)
	{
		std::vector<const FbxTool::SNode*> skinNodes;
		for (int i = 0, N = pFbxScene->GetNodeCount(); i < N; ++i)
		{
			const FbxTool::SNode* const pFbxNode = pFbxScene->GetNodeByIndex(i);
			bool bHasSkin = false;
			for (int j = 0; !bHasSkin && j < pFbxNode->numMeshes; ++j)
			{
				bHasSkin = bHasSkin || pFbxNode->ppMeshes[j]->bHasSkin;
			}
			if (bHasSkin)
			{
				skinNodes.push_back(pFbxNode);
			}
		}
		return skinNodes;
	}

	// Returns a list of named nodes. The names are unique among all nodes and can be used as filenames.
	static std::vector<std::pair<const FbxTool::SNode*, string>> GetNamedNodes(const FbxTool::CScene* pFbxScene, const std::vector<const FbxTool::SNode*>& nodes)
	{
		std::vector<std::pair<const FbxTool::SNode*, string>> out;
		for (const FbxTool::SNode* pNode : nodes)
		{
			out.emplace_back(pNode, string(pNode->szName));
		}
		return out;
	}

	static std::vector<const FbxTool::SAnimationTake*> GetAnimationTakes(const FbxTool::CScene* pScene)
	{
		std::vector<const FbxTool::SAnimationTake*> takes;
		takes.reserve(pScene->GetAnimationTakeCount());
		for (int i = 0, N = pScene->GetAnimationTakeCount(); i < N; ++i)
		{
			takes.push_back(pScene->GetAnimationTakeByIndex(i));
		}
		return takes;
	}

	// Returns a list of named takes. The names are unique among all takes and can be used as filenames.
	static std::vector<std::pair<const FbxTool::SAnimationTake*, string>> GetNamedTakes(const std::vector<const FbxTool::SAnimationTake*>& takes)
	{
		typedef std::pair<const FbxTool::SAnimationTake*, string> NamedTake;

		std::vector<NamedTake> namedTakes;
		namedTakes.reserve(takes.size());
		std::transform(takes.begin(), takes.end(), std::back_inserter(namedTakes), [](const FbxTool::SAnimationTake* pTake)
		{
			return std::make_pair(pTake, MakeAlphaNum(pTake->name));
		});
		std::sort(namedTakes.begin(), namedTakes.end(), [](const NamedTake& lhp, const NamedTake& rhp)
		{
			return lhp.second < rhp.second;
		});
		std::vector<int> count(namedTakes.size(), 0);
		for (size_t i = 1; i < namedTakes.size(); ++i)
		{
			if (namedTakes[i].second == namedTakes[i - 1].second)
			{
				count[i] = count[i - 1] + 1;
			}
		}
		for (size_t i = 1; i < namedTakes.size(); ++i)
		{
			if (count[i] > 0)
			{
				namedTakes[i].second = string().Format("%s%d", namedTakes[i].second, count[i]);
			}
		}
		return namedTakes;
	}

	static bool WriteAssetMetaData(const FbxMetaData::SMetaData& metaData, const string& outputDirectory, const string& filename)
	{
		using namespace Private_AssetImporterFBX;

		// Write meta-data to file.
		std::unique_ptr<QTemporaryFile> pTempFile = WriteTemporaryFile(QtUtil::ToQString(outputDirectory), metaData.ToJson(), QtUtil::ToQString(filename));
		if (!pTempFile)
		{
			return false;
		}

		// Call RC.
		CRcCaller rcCaller;
		rcCaller.SetEcho(false);
		rcCaller.SetListener(&CRcListener::GetInstance());
		rcCaller.OverwriteExtension("fbx");
		rcCaller.SetAdditionalOptions(CRcCaller::OptionOverwriteFilename(PathUtil::ReplaceExtension(filename, metaData.outputFileExt)));
		return rcCaller.Call(string(QtUtil::ToString(pTempFile->fileName()).c_str()));
	}

	static std::vector<string> FindAssetMetaDataPaths(const string& dir)
	{
		const size_t dirLen = PathUtil::AddSlash(dir).size();
		std::vector<string> assetPaths;

		//todo 
		assert(0);
		//assetPaths = dir;
		//QDirIterator iterator(string(dir), QStringList() << "*.cryasset", QDir::Files, QDirIterator::Subdirectories);
		//while (iterator.hasNext())
		//{
		//	const string filePath = QtUtil::ToString(iterator.next()).substr(dirLen); // Remove leading path to search directory.
		//	assetPaths.push_back(filePath);
		//}
		return assetPaths;
	}

	static std::vector<string> FindAllTextures(const FbxTool::CScene* pFbxScene)
	{
		std::vector<string> texs;
		for (int i = 0; i < pFbxScene->GetMaterialCount(); ++i)
		{
			const FbxTool::SMaterial* const pMaterial = pFbxScene->GetMaterialByIndex(i);

			for (int j = 0; j < FbxTool::eMaterialChannelType_COUNT; ++j)
			{
				const FbxTool::SMaterialChannel& channel = pMaterial->m_materialChannels[j];
				if (channel.m_bHasTexture)
				{
					texs.push_back(channel.m_textureFilename);
				}
			}
		}
		return texs;
	}

	static std::vector<std::pair<string, string>> TranslateTexturePaths(const std::vector<string>& texs, const string& sourceFilePath)
	{
		std::vector<std::pair<string, string>> relTexs;
		relTexs.reserve(texs.size());
		for (const string& tex : texs)
		{
			relTexs.emplace_back(tex, TextureHelpers::TranslateFilePath(sourceFilePath, sourceFilePath, tex));
		}
		return relTexs;
	}

	static void InitScene(FbxTool::CScene* pFbxScene)
	{
		for (int i = 0, N = pFbxScene->GetMaterialCount(); i < N; ++i)
		{
			const FbxTool::SMaterial* const pFbxMaterial = pFbxScene->GetMaterialByIndex(i);
			pFbxScene->SetMaterialSubMaterialIndex(pFbxMaterial, i);
			pFbxScene->SetMaterialPhysicalizeSetting(pFbxMaterial, FbxTool::eMaterialPhysicalizeSetting_None);
		}
	}

	static void InitializeMaterial(CMaterial* pEditorMaterial, const FbxTool::SMaterial& fbxMaterial, const std::vector<std::pair<string, string>>& relTexs, const string& outDir)
	{
		assert(0);
		//SInputShaderResources& inputRes = pEditorMaterial->GetShaderResources();

		//// Lighting settings of fabric (non-metal).
		//// Setting specular from file might mess up rendering if it does not respect PBS conventions.
		//inputRes.m_LMaterial.m_Diffuse = ColorGammaToLinear(ColorF(255, 255, 255));
		//inputRes.m_LMaterial.m_Specular = ColorGammaToLinear(ColorF(61, 61, 61));
		//inputRes.m_LMaterial.m_Smoothness = 255.0f;

		//for (int i = 0; i < FbxTool::eMaterialChannelType_COUNT; ++i)
		//{
		//	const char* szSemantic = GetTextureSemanticFromChannelType(FbxTool::EMaterialChannelType(i));
		//	if (!szSemantic)
		//	{
		//		continue;
		//	}

		//	const EEfResTextures texId = MaterialHelpers::FindTexSlot(szSemantic);

		//	if (fbxMaterial.m_materialChannels[i].m_bHasTexture)
		//	{
		//		const string tex = fbxMaterial.m_materialChannels[i].m_textureFilename;
		//		auto it = std::find_if(relTexs.begin(), relTexs.end(), [&tex](const std::pair<string, string>& relTex)
		//		{
		//			return relTex.first == tex;
		//		});
		//		if (it != relTexs.end())
		//		{
		//			const string relTif = PathUtil::ReplaceExtension(it->second, "tif");
		//			inputRes.m_Textures[texId].m_Name = PathUtil::Make(outDir, relTif);
		//		}
		//	}
		//}
	}

	static void WriteCHRPARAMS(CAsset* pSkeletonAsset, CAssetImportContext& ctx)
	{
		/*if (!pSkeletonAsset->GetFilesCount())
		{
			return;
		}

		const string skeletonFilePath = pSkeletonAsset->GetFile(0);
		bool bWroteChrParams = false;
		const string chrParamsAssetPath = PathUtil::ReplaceExtension(skeletonFilePath, "chrparams");
		const string chrParamsAbsFilePath = PathUtil::Make(PathUtil::GetGameProjectAssetsPath(), chrParamsAssetPath);
		const QString chrparams = MeshImporter::CreateCHPARAMS(QtUtil::ToQString(ctx.GetOutputDirectoryPath()));
		if (!chrparams.isEmpty())
		{
			if (WriteToFile(QtUtil::ToQString(chrParamsAbsFilePath), chrparams))
			{
				CEditableAsset editableSkeletonAsset = ctx.CreateEditableAsset(*pSkeletonAsset);
				editableSkeletonAsset.AddFile(chrParamsAssetPath);
				editableSkeletonAsset.WriteToFile();
				bWroteChrParams = true;
			}
		}
		if (!bWroteChrParams)
		{
			CryWarning(VALIDATOR_MODULE_ASSETS, VALIDATOR_WARNING, string().Format("Unable to write .chrparams file '%s'", PathUtil::ToUnixPath(chrParamsAbsFilePath).c_str()));
		}*/
		assert(0);
	}

	//! Returns a list of all files that make up an asset which cannot be automatically regenerated.
	//! This list includes, for example, the meta-data (cryasset) file, source file, and all data files.
	//! The list does not include thumbnails.
	//! The list of independent files is used when moving an asset from a temporary folder to the game project folder.
	static std::vector<string> GetIndependentFiles(const CAsset* pAsset)
	{
		assert(0);
		std::vector<string> files;

		/*files.push_back(pAsset->GetMetadataFile());
		files.push_back(pAsset->GetSourceFile());
		for (size_t i = 0, N = pAsset->GetFilesCount(); i < N; ++i)
		{
			files.push_back(pAsset->GetFile(i));
		}*/

		return files;
	}

	//! Convenience function that imports a single asset type to an output directory and passes along the parent importing context.
	std::vector<CAsset*> ImportDependency(const string& sourceFilePath, const string& outputDirectory, const char* assetType, CAssetImportContext& parentCtx)
	{
	/*	string ext(PathUtil::GetExt(sourceFilePath.c_str()));
		ext.MakeLower();
		CAssetImporter* pAssetImporter = CAssetManager::GetInstance()->GetAssetImporter(ext, assetType);
		if (!pAssetImporter)
		{
			return{};
		}
		CAssetImportContext ctx;
		ctx.SetInputFilePath(sourceFilePath);
		ctx.SetOutputDirectoryPath(outputDirectory);
		ctx.SetOverwriteAll(parentCtx.IsOverwriteAll());
		return pAssetImporter->Import({ assetType }, ctx);*/

		assert(0);
		std::vector<CAsset*> vAsset;
		return vAsset;
	}

} // namespace Private_AssetImporterFBX

bool CAssetImporterFBX::Init(const string& inputFilePath)
{
	using namespace Private_AssetImporterFBX;

	m_fbxFilePath.clear();
	m_pFbxScene.release();
	m_pTempDir.clear();
	m_bCreateMaterial = false;

	char buf[MAX_PATH];
		::GetTempPathA(MAX_PATH, buf);
	m_pTempDir = buf;//  CreateTemporaryDirectory();
	const string importedFilePath = ImportFbxFile(inputFilePath, m_pTempDir);
	if (importedFilePath.empty())
	{
		CryWarning(VALIDATOR_MODULE_EDITOR, VALIDATOR_WARNING, "Importing file '%s' failed.", inputFilePath.c_str());
		return false;
	}
	m_fbxFilePath = importedFilePath;
	return true;
}

FbxTool::CScene* CAssetImporterFBX::GetScene()
{
	using namespace Private_AssetImporterFBX;

	// Lazy scene loading.
	if (!m_pFbxScene)
	{
		m_pFbxScene = FbxTool::CScene::ImportFile(m_fbxFilePath.c_str());
		if (!m_pFbxScene)
		{
			CryWarning(VALIDATOR_MODULE_EDITOR, VALIDATOR_WARNING, "Importing scene from failed.");
			return nullptr;
		}
	//	InitScene(m_pFbxScene.get());
	}
	return m_pFbxScene.get();
}

void CAssetImporterFBX::Import(const std::vector<string>& assetTypes, CAssetImportContext& ctx)
{

}
