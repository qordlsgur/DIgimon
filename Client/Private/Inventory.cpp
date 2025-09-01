#include "Inventory.h"

#include "GameInstance.h"

CInventory::CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CInventory::CInventory(const CInventory& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CInventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 700.f;
	Desc.fY = 400.f;
	Desc.fSizeX = 382.f;
	Desc.fSizeY = 500.f;


	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Create_Slot(TEXT("Layer_Slot"))))
		return E_FAIL;

	return S_OK;
}

void CInventory::Priority_Update(_float fTimeDelta)
{
}

void CInventory::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(m_fX - m_fWinSizeX * 0.5f, -m_fY + m_fWinSizeY * 0.5f, 0.f, 1.f));
	m_pRect = { long(m_fX - m_fSizeX * 0.5f), long(m_fY - m_fSizeY * 0.5f), long(m_fX + m_fSizeX * 0.5f), long(m_fY + m_fSizeY * 0.5f) };

	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		m_vSlots[i]->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
	}

	m_pExit->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));

	m_pWeapon_Type->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
	m_pSoby_Type->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
	m_pGita_Type->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
	m_pLineUp_Type->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));

	if (m_pGameInstance->Key_Down(DIK_I))
		Set_Active();

	if (m_bActive)
	{
		if (m_pGameInstance->Mouse_Down(MOUSEKEYSTATE::LBUTTON))
			OnClick();
	}

}

void CInventory::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CInventory::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		__super::Begin();
		__super::Blend_Begin();

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		for (_uint i = 0; i < m_iSlotCount; ++i)
		{
			m_vSlots[i]->Render();
		}

		m_pExit->Render();

		m_pWeapon_Type->Render();
		m_pSoby_Type->Render();
		m_pGita_Type->Render();
		m_pLineUp_Type->Render();

		__super::Blend_End();
		__super::End();
	}

	return S_OK;
}

void CInventory::OnClick()
{
	POINT pPt = m_pGameInstance->Get_Mouse();

	if (PtInRect(&m_pRect, pPt))
	{
		for (int i = 0; i < m_iSlotCount; ++i)
		{
			if (PtInRect(m_vSlots[i]->Get_Pos(), pPt))
			{
				m_vSlots[i]->OnClick();
				return;
			}
		}

		if (PtInRect(m_pExit->Get_Pos(), pPt))
			m_pExit->Hover();
	}
}

HRESULT CInventory::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Inventory"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInventory::Bind_ShaderResources()
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

HRESULT CInventory::Create_Slot(const _wstring& strLayerTag)
{
	m_iSlotCount = 40;
	m_iItem_Button_Count = 4;

	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		m_pSlot = static_cast<CSlot*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Slot"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));

		m_vSlots.push_back(m_pSlot);
	}

	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		_float col = i % 8;
		_float row = i / 8;

		_float startX = -165.f + col * (40 + 7);
		_float startY = -140.f + row * (40 + 7);

		m_vSlots[i]->Set_Move(startX, startY);
	}


	m_pExit = static_cast<CExit_Button*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Exit_Button"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));

	m_pExit->Set_Move(1040, -50);




	m_pWeapon_Type = static_cast<CItemType_Button*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ItemType_Button"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	m_pWeapon_Type->Set_Move(-200.f, -300.f);

	m_pSoby_Type = static_cast<CItemType_Button*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ItemType_Button"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	m_pSoby_Type->Set_Move(-80.f, -300.f);

	m_pGita_Type = static_cast<CItemType_Button*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ItemType_Button"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	m_pGita_Type->Set_Move(40.f, -300.f);

	m_pLineUp_Type = static_cast<CItemType_Button*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ItemType_Button"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));
	m_pLineUp_Type->Set_Move(380.f, -300.f);



	return S_OK;
}

CInventory* CInventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInventory* pInstance = new CInventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInventory_Prototype ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
	CInventory* pInstance = new CInventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CInventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInventory::Free()
{
	__super::Free();

	for (auto& vSlots : m_vSlots)
		Safe_Release(vSlots);

	m_vSlots.clear();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);

}
