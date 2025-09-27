#include "Battle_Terrain.h"
#include "GameInstance.h"
#include "ContainerObject.h"
#include "Battle_Manager.h"
#include "Digimon_Manager.h"
#include "Angewomon.h"
#include "Blackwargreymon.h"
#include "Devilmon.h"
#include "Ladydevimon.h"
#include "Leomon.h"
#include "Metalgarumon.h"
#include "Metalgreymon.h"
#include "Omegamon.h"
#include "Wargreymon.h"

CBattle_Terrain::CBattle_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CBattle_Terrain::CBattle_Terrain(const CBattle_Terrain& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CBattle_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBattle_Terrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pBattle_Manager = CBattle_Manager::GetInstance();
	m_pBattle_Manager->Set_Battle_Terrain(this);

	m_pDigimon_Manager = CDigimon_Manager::GetInstance();
	m_pDigimon_Manager->Player(this);

	m_iMaxDigimon = 5;

	for (_int i = 0; i < m_iMaxDigimon; ++i)
	{
		_float fX = 15.f * static_cast<_float>(i);

		m_vPlayerDigimonPos[i] = XMVectorSet(130.f - fX, 0.f, 130.f, 1.f);
		m_vMonsterDigimonPos[i] = XMVectorSet(130.f - fX, 0.f, 70.f, 1.f);
		m_vPlayerDigimonAttackPos[i] = XMVectorSet(130.f - fX, 0.f, 80.f, 1.f);
		m_vMonsterDigimonAttackPos[i] = XMVectorSet(130.f - fX, 0.f, 120.f, 1.f);
	}

	m_vPlayerPos = XMVectorSet(100.f, 0.f, 135.f, 1.f);

	m_pBattle_Manager->Set_Battle_Pos(m_vMonsterDigimonPos, m_vPlayerDigimonPos, m_vPlayerPos, m_vMonsterDigimonAttackPos, m_vPlayerDigimonAttackPos);
	return S_OK;
}


void CBattle_Terrain::Priority_Update(_float fTimeDelta)
{
}

void CBattle_Terrain::Update(_float fTimeDelta)
{
	m_bBattle = m_pBattle_Manager->Get_Battle();
}

void CBattle_Terrain::Late_Update(_float fTimeDelta)
{
	if (m_bBattle)
		m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBattle_Terrain::Render()
{
	if (m_bBattle)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Resources()))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Render()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBattle_Terrain::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Battle_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBattle_Terrain::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CBattle_Terrain* CBattle_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBattle_Terrain* pInstance = new CBattle_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBattle_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBattle_Terrain::Clone(void* pArg)
{
	CBattle_Terrain* pInstance = new CBattle_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBattle_Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBattle_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
