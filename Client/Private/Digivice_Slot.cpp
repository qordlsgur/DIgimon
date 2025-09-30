#include "Digivice_Slot.h"
#include "Digimon_Manager.h"
#include "GameInstance.h"

CDigivice_Slot::CDigivice_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Slot::CDigivice_Slot(const CDigivice_Slot& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Slot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Slot::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = -100.f;
	Desc.fY = 0.f;
	Desc.fSizeX = 241.f;
	Desc.fSizeY = 50.f;

	m_fOffSetName = 34.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pManager = CDigimon_Manager::GetInstance();

	m_bHasDigimon = false;

	return S_OK;
}

void CDigivice_Slot::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Slot::Update(_float fTimeDelta)
{
	_itow_s(m_Info.Lv, m_szDigimonLv, MAX_PATH, 10);
	wcscpy_s(m_szLv, MAX_PATH, L"LV");
	wcscat_s(m_szLv, MAX_PATH, m_szDigimonLv);     

}

void CDigivice_Slot::Late_Update(_float fTimeDelta)
{
	_vector vWorldPos = XMVectorSet(m_fX + m_fParent_WorldPos.x, -(m_fY)+m_fParent_WorldPos.y, 0.f, 1.f);

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vWorldPos);

	_float4 Pos;
	XMStoreFloat4(&Pos, vWorldPos);

	m_pRect = {
		long(Pos.x - m_fSizeX * 0.5f),
		long(-Pos.y - m_fSizeY * 0.5f),
		long(Pos.x + m_fSizeX * 0.5f),
		long(-Pos.y + m_fSizeY * 0.5f)
	};

	m_fPos.x = m_pTransformCom->Get_State(STATE::POSITION).m128_f32[0];
	m_fPos.y = m_pTransformCom->Get_State(STATE::POSITION).m128_f32[1];
}

HRESULT CDigivice_Slot::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if (m_bHasDigimon)
	{
		_float fontSizeX = (m_pGameInstance->FontSizeX(TEXT("12"), m_Info.DigimonName.c_str()) - m_fOffSetName) * 0.5f;
		m_pGameInstance->Render_Text(TEXT("10"), m_szLv, _float2(660.f + m_fPos.x, 340.f + (-m_fPos.y)), XMVectorSet(1.f, 1.f, 1.f, 1.f));
		m_pGameInstance->Render_Text(TEXT("12"), m_Info.DigimonName.c_str(), _float2((660.f - fontSizeX) + m_fPos.x, 365.f + (-m_fPos.y)), XMVectorSet(1.f, 1.f, 1.f, 1.f));
	}

	return S_OK;
}

void CDigivice_Slot::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CDigivice_Slot::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

void CDigivice_Slot::Set_Info(DIGIMON_INFO* pInfo)
{
	m_Info = *pInfo;
}

void CDigivice_Slot::OnClick()
{
	int a = 10;
}

HRESULT CDigivice_Slot::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Digivice_Slot_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Slot"),
		TEXT("Com_Digivice_Slot_Texture"), reinterpret_cast<CComponent**>(&m_pSlotTextureCom))))
		return E_FAIL;

	/* Com_Digivice_Mask_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Mask"),
		TEXT("Com_Digivice_Mask_Texture"), reinterpret_cast<CComponent**>(&m_pMaskTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Slot::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pSlotTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", 0)))
		return E_FAIL;
	return S_OK;
}


CDigivice_Slot* CDigivice_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Slot* pInstance = new CDigivice_Slot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Slot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Slot::Clone(void* pArg)
{
	CDigivice_Slot* pInstance = new CDigivice_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pSlotTextureCom);
	Safe_Release(m_pShaderCom);
}
