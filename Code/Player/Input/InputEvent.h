#ifndef __INPUTEVENT_INC_H__
#define __INPUTEVENT_INC_H__
#include "Entities/Helpers/ISimpleExtension.h"
#include <CrySystem/IWindowMessageHandler.h>
#include <CryInput/IInput.h>

#include "../../MeshImporter/AssetImporterFBX.h"


class CInputEvent :public IInputEventListener,
	public CGameObjectExtensionHelper<ISimpleExtension, ISimpleExtension>,
	public IWindowMessageHandler
{
private:
	CAssetImporterFBX m_importFBX;
public:

	virtual bool OnInputEvent(const SInputEvent& event);
	virtual bool OnInputEventUI(const SUnicodeEvent& event) { return false; }

	//! Used to sort the listeners based on priority
	//! \return Priority associated with the listener (Higher the priority, the earlier it will be processed relative to other listeners, default = 0)
	virtual int GetPriority() const { return 0; }

	// ISimpleExtension interface
	virtual void PostInit(IGameObject* pGameObject) override;
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;


	// system event
	virtual void PreprocessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

};
#endif // __INPUTEVENT_INC_H__