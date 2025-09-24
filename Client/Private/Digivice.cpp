#include "Digivice.h"
#include "GameInstance.h"
#include "Digivice_Slot.h"
#include "Digimon_Manager.h"
#include "Digivice_Mask.h"

CDigivice::CDigivice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice::CDigivice(const CDigivice& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 700.f;
	Desc.fY = 350.f;
	Desc.fSizeX = 900.f;
	Desc.fSizeY = 600.f;

	m_iDigivice_Battle_Slot_Number = 8;

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();
	m_pDigimon_Manager->Digivice(this);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Create_Slot(L"Layout_Digivice_Slot")))
		return E_FAIL;

	Acquire_Digimon(7);

	return S_OK;
}

void CDigivice::Priority_Update(_float fTimeDelta)
{
}

void CDigivice::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_V))
		Set_Active();

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		m_pBattle_Slot[i]->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
		m_pBattle_Mask[i]->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
	}

	if (m_bActive)
		OnClick();

	if (m_pGameInstance->Key_Down(DIK_C))
		Acquire_Digimon(7);

	if (m_pGameInstance->Key_Down(DIK_B))
		Release_Digimon(0);
}

void CDigivice::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CDigivice::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
		{
			m_pBattle_Slot[i]->Render();
			if (m_pBattle_Mask[i]->Get_HasDigimon())
				m_pBattle_Mask[i]->Render();
		}

		m_pGameInstance->Render_Text(TEXT("18"), TEXT("다지바이스"), _float2(600.f, 70.f), XMVectorSet(0.f, 0.f, 1.f, 1.f));


	}
	return S_OK;
}

void CDigivice::Acquire_Digimon(_int ID)
{
	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		if (false == m_pBattle_Mask[i]->Get_HasDigimon())
		{
			m_pBattle_Mask[i]->Set_Digimon_ID(ID);
			m_pBattle_Mask[i]->Set_HasDigimon(true);
			m_pBattle_Slot[i]->Set_Info(ID);
			m_pBattle_Slot[i]->Set_HasDigimon(true);
			m_pDigimon_Manager->Set_Digivice_Slot(i, true, ID);
			return;
		}
	}
}

void CDigivice::Release_Digimon(_int ID)
{
	if (true == m_pBattle_Mask[ID]->Get_HasDigimon())
	{
		m_pBattle_Mask[ID]->Set_Digimon_ID(-1);
		m_pBattle_Mask[ID]->Set_HasDigimon(false);
		m_pBattle_Slot[ID]->Set_HasDigimon(false);
		m_pDigimon_Manager->Set_Digivice_Slot(ID, false, -1);

	}
	else
		return;
}

void CDigivice::OnClick()
{
	POINT pPt = m_pGameInstance->Get_Mouse();

	pPt.x -= m_fWinSizeX * 0.5f;
	pPt.y -= m_fWinSizeY * 0.5f;

	for (_int i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		if (m_pGameInstance->Mouse_Down(MOUSEKEYSTATE::LBUTTON))
		{
			if (PtInRect(m_pBattle_Slot[i]->Get_Pos(), pPt))
			{
				m_pBattle_Slot[i]->OnClick();
				return;
			}
		}
	}
}

HRESULT CDigivice::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Inventory"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice::Create_Slot(const _wstring& strLayerTag)
{
	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		m_pDigivice_Slot = static_cast<CDigivice_Slot*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Slot"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));

		m_pDigivice_Mask = static_cast<CDigivice_Mask*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Mask"),
			ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Mask")));

		m_pBattle_Slot.push_back(m_pDigivice_Slot);
		m_pBattle_Mask.push_back(m_pDigivice_Mask);
	}

	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i);

		_float startX = -300.f;
		_float startY = -170.f + row * 55;

		_float maskstartX = -50.f;

		m_pBattle_Slot[i]->Set_Move(startX, startY);
	}

	for (_uint i = 3; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i - 3);

		_float startX = -300.f;
		_float startY = 30.f + row * 55;

		_float maskstartX = -50.f;

		m_pBattle_Slot[i]->Set_Move(startX, startY);
	}

	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i);

		_float startX = 100.f;
		_float startY = 10.f + row * 110;

		_float maskstartX = -50.f;

		m_pBattle_Mask[i]->Set_Move(maskstartX, startY);

	}

	for (_uint i = 3; i < m_iDigivice_Battle_Slot_Number; ++i)
	{
		_float row = static_cast<_float>(i - 3);

		_float startX = 100.f;
		_float startY = 411.f + row * 110;

		_float maskstartX = -50.f;

		m_pBattle_Mask[i]->Set_Move(maskstartX, startY);
	}

	return S_OK;
}


CDigivice* CDigivice::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice* pInstance = new CDigivice(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDigivice::Clone(void* pArg)
{
	CDigivice* pInstance = new CDigivice(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDigivice");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice::Free()
{
	__super::Free();

	m_pBattle_Slot.clear();
	m_pBattle_Mask.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}