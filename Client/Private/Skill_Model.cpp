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

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(250.f, 10.f, 500.f, 1.f));

	m_pTransformCom->Set_Scale(10.f, 10.f, 10.f);

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

	if (m_pGameInstance->Key_Down(DIK_3))
		m_bOn = !m_bOn;

	if (m_bOn)
		m_fTime += fTimeDelta;
	else
		m_fTime = 0.f;

	if (m_pGameInstance->Key_Down(DIK_L))
		a++;

	if (m_pGameInstance->Key_Down(DIK_K))
		a--;

	if (a < 0)
		a = 8;

	if (a > 8)
		a = 0;
}

void CSkill_Model::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
	m_pGameInstance->Add_RenderGroup(RENDER::BLUR, this);
}

HRESULT CSkill_Model::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pModel1Com->Render(0)))
		return E_FAIL;

	if (FAILED(m_pModel2Com->Render(0)))
		return E_FAIL;

	if (FAILED(m_pModel3Com->Render(0)))
		return E_FAIL;

	//if (FAILED(m_pModel4Com->Render(0)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Model::Ready_Components()
{
	/* Com_Model1 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_1"),
		TEXT("Com_Model1"), reinterpret_cast<CComponent**>(&m_pModel1Com))))
		return E_FAIL;

	/* Com_Model2 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_2"),
		TEXT("Com_Model2"), reinterpret_cast<CComponent**>(&m_pModel2Com))))
		return E_FAIL;

	/* Com_Model3 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_3"),
		TEXT("Com_Model3"), reinterpret_cast<CComponent**>(&m_pModel3Com))))
		return E_FAIL;

	/* Com_Model4 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Model_Angewomon_Skill2_4"),
		TEXT("Com_Model4"), reinterpret_cast<CComponent**>(&m_pModel4Com))))
		return E_FAIL;

	/* Com_Texture1 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill2_Image"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTexture1Com))))
		return E_FAIL;

	/* Com_Texture2 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill2_Image"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTexture2Com))))
		return E_FAIL;

	/* Com_Texture3 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill2_Image"),
		TEXT("Com_Texture3"), reinterpret_cast<CComponent**>(&m_pTexture3Com))))
		return E_FAIL;

	/* Com_Texture4 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_AngewomonSkill2_Image"),
		TEXT("Com_Texture4"), reinterpret_cast<CComponent**>(&m_pTexture4Com))))
		return E_FAIL;

	/* Com_mTexture5 */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_mTexture5"), reinterpret_cast<CComponent**>(&m_pTexture5Com))))
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

	if (FAILED(m_pTexture1Com->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTexture2Com->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTexture3Com->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 1)))
		return E_FAIL;

	if (FAILED(m_pTexture4Com->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 2)))
		return E_FAIL;

	if (FAILED(m_pTexture5Com->Bind_ShaderResource(m_pShaderCom, "g_Dissolve", 7)))
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

	Safe_Release(m_pModel1Com);
	Safe_Release(m_pTexture1Com);
	Safe_Release(m_pModel2Com);
	Safe_Release(m_pTexture2Com);
	Safe_Release(m_pModel3Com);
	Safe_Release(m_pTexture3Com);
	Safe_Release(m_pModel4Com);
	Safe_Release(m_pTexture4Com);

	Safe_Release(m_pShaderCom);
}

