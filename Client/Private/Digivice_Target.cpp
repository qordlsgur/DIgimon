#include "Digivice_Target.h"
#include "Digimon_Manager.h"
#include "GameInstance.h"

CDigivice_Target::CDigivice_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Target::CDigivice_Target(const CDigivice_Target& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Target::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Target::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 270.f;
	Desc.fSizeY = 300.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pManager = CDigimon_Manager::GetInstance();


	return S_OK;
}

void CDigivice_Target::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Target::Update(_float fTimeDelta)
{
}

void CDigivice_Target::Late_Update(_float fTimeDelta)
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
}

HRESULT CDigivice_Target::Render()
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

	}

	return S_OK;
}

void CDigivice_Target::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}


void CDigivice_Target::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

void CDigivice_Target::Set_Info(DIGIMON_INFO* pInfo)
{
	m_Info = *pInfo;
}

void CDigivice_Target::OnClick()
{
}

void CDigivice_Target::Set_Digimon_Target()
{
}

HRESULT CDigivice_Target::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Digivice_Target*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Target"),
		TEXT("Com_Digivice_Target"), reinterpret_cast<CComponent**>(&m_pTargetTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_Target"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Target::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTargetTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;

	return S_OK;
}


CDigivice_Target* CDigivice_Target::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Target* pInstance = new CDigivice_Target(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Target ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Target::Clone(void* pArg)
{
	CDigivice_Target* pInstance = new CDigivice_Target(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Target");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Target::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTargetTextureCom);
	Safe_Release(m_pShaderCom);
}
