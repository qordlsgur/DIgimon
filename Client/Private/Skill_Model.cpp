#include "Skill_Model.h"
#include "GameInstance.h"

CSkill_Model::CSkill_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSkill_Model::CSkill_Model(const CSkill_Model& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CSkill_Model::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Model::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(10.f, 10.f, 200.f, 1.f));


	return S_OK;
}

void CSkill_Model::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Model::Update(_float fTimeDelta)
{
	//m_pTransformCom->SizeUp(fTimeDelta, fTimeDelta, fTimeDelta);

	//_float a = m_pTransformCom->Get_State(STATE::RIGHT).m128_f32[0];

	//if(a >= 5.f)
	//	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

	//m_fTime += fTimeDelta * 5.f;
}

void CSkill_Model::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CSkill_Model::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;


	if (FAILED(m_pModelCom->Render(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Model::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_1"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill2_Image"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_VtxSkillMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Model::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_State("Time", m_fTime)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CSkill_Model* CSkill_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkill_Model* pInstance = new CSkill_Model(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkill_Model");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_Model::Clone(void* pArg)
{
	CSkill_Model* pInstance = new CSkill_Model(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_Model::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}

