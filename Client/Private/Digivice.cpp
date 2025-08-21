#include "Digivice.h"
#include "GameInstance.h"
#include "Digivice_Slot.h"
#include "Digimon_Manager.h"

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
	Desc.fY = 400.f;
	Desc.fSizeX = 700.f;
	Desc.fSizeY = 500.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	m_iDigivice_Battle_Slot_Number = 8;

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();
	m_pDigimon_Manager->Digivice(this);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Create_Slot(L"Layout_Digivice_Slot")))
		return E_FAIL;

	return S_OK;
}

void CDigivice::Priority_Update(_float fTimeDelta)
{
}

void CDigivice::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_V))
		Set_Active();
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

		__super::Begin();

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;

		for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
		{
			m_pBattle_Slot[i]->Render();
		}


		__super::End();

	}
	return S_OK;
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

		m_pBattle_Slot.push_back(m_pDigivice_Slot);
	}

	for (_uint i = 0; i < m_iDigivice_Battle_Slot_Number; ++i)
	{

		_float row = i % 3;

		_float startX = -75.f;
		_float startY = -80.f + row * 43;

		m_pBattle_Slot[i]->Set_Move(startX, startY);

	}

	for (_uint i = 3; i < m_iDigivice_Battle_Slot_Number; ++i)
	{

		_float row = i % 5;

		_float startX = -75.f;
		_float startY = 95.f + row * 43;

		m_pBattle_Slot[i]->Set_Move(startX, startY);

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

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}