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
	Desc.fSizeX = 385.f;
	Desc.fSizeY = 500.f;

	m_iSlotCount = 40;


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
	for (_uint i = 0; i < m_iSlotCount; ++i)
	{
		m_vSlots[i]->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));
	}

	m_pExit->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));

	m_pItemType->Set_Parent_WorldPos(m_pTransformCom->Get_State(STATE::POSITION));

	if (m_pGameInstance->Key_Down(DIK_I))
		Set_Active();


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

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		for (_uint i = 0; i < m_iSlotCount; ++i)
		{
			m_vSlots[i]->Render();
		}

		m_pExit->Render();

		m_pItemType->Render();

		__super::End();
	}

	return S_OK;
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
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

		_float startX = -230.f + col * (85 + 10);
		_float startY = -200.f + row * (85 + 10);

		m_vSlots[i]->Set_Move(startX, startY);
	}

	m_pExit = static_cast<CExit_Button*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Exit_Button"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));

	m_pExit->Set_Move(425, -370);

	m_pItemType = static_cast<CItemType_Button*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_ItemType_Button"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag));

	m_pItemType->Set_Move(-230.f, -300.f);

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

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
