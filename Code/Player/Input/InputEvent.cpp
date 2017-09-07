#include "StdAfx.h"
#include "GamePlugin.h"
#include "InputEvent.h"
#include <vector>
#include <Cry3DEngine/I3DEngine.h>
#include <Cry3DEngine/IMaterial.h>

#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

class CInputEventRegistrator
	: public IEntityRegistrator
{
	virtual void Register() override
	{
		CGamePlugin::RegisterEntityComponent<CInputEvent>("InputEvent");

		//REGISTER_CVAR2("pl_movementSpeed", &m_movementSpeed, 20.0f, VF_NULL, "Player movement speed.");
	}

	virtual void Unregister() override
	{
		IConsole* pConsole = gEnv->pConsole;
		if (pConsole)
		{
		//	pConsole->UnregisterVariable("pl_movementSpeed");
		}
	}

public:
	CInputEventRegistrator() {}

	~CInputEventRegistrator()
	{
		//gEnv->pConsole->UnregisterVariable("pl_movementSpeed", true);
	}

	float m_movementSpeed;
};

CInputEventRegistrator g_pInputEventRegistrator;

bool CInputEvent::OnInputEvent(const SInputEvent& event)
{
	if (event.deviceType == eIDT_Keyboard)
	{
		if (event.state == eIS_Released)
		{
			switch (event.keyId)
			{
				case eKI_1:
				{
					m_importFBX.Init("");
					/*QString path = QFileDialog::getOpenFileName(NULL, ("Open Image"), ".", ("Image Files(*.jpg *.png)"));
					if (path.length() == 0) {
						QMessageBox::information(NULL, ("Path"), ("You didn't select any files."));
					}
					else {
						QMessageBox::information(NULL, ("Path"), ("You selected ") + path);
					}*/
					

					break;
				}
				case eKI_2:
				{
					
					break;
				}

				case eKI_3: // load model 
				{
				
					break;
				}
				case eKI_4: // delete model
				{	
					
					break;
				}

				case eKI_5:
				{
				
					break;
				}
				case eKI_6:
				{
					
					
				}
				default:
				{
					break;
				}
			}
		}
	}

	return true;
}

 void CInputEvent::PostInit(IGameObject* pGameObject)
{
	 gEnv->p3DEngine;
	 // Make sure that this extension is updated regularly via the Update function below
	 pGameObject->EnableUpdateSlot(this, 0);
}
 void CInputEvent::Update(SEntityUpdateContext& ctx, int updateSlot)
{

	// CryLog("Update Per Frame use Time:%f", useTime);
}


// system event
 void CInputEvent::PreprocessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	if (uMsg == WM_SYSKEYDOWN)
	{
		if (wParam == VK_F4 && GetKeyState(VK_F4))
		{
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
	else if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)
	{
		TerminateProcess(GetCurrentProcess(), 0);
	}
	
}
 bool CInputEvent::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	 return false;
}