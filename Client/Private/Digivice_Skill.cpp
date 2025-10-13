#include "Digivice_Skill.h"
#include "GameInstance.h"
#include "Digimon_Manager.h"
#include "Digivice_Skill_Info.h"

CDigivice_Skill::CDigivice_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CDigivice_Skill::CDigivice_Skill(const CDigivice_Skill& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CDigivice_Skill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDigivice_Skill::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100.f;
	Desc.fY = 100.f;
	Desc.fSizeX = 50.f;
	Desc.fSizeY = 50.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pManager = CDigimon_Manager::GetInstance();

	return S_OK;
}

void CDigivice_Skill::Priority_Update(_float fTimeDelta)
{
}

void CDigivice_Skill::Update(_float fTimeDelta)
{
}

void CDigivice_Skill::Late_Update(_float fTimeDelta)
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

	//m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CDigivice_Skill::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CDigivice_Skill::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CDigivice_Skill::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;
}

void CDigivice_Skill::Set_Info(DIGIMON_INFO* pInfo)
{
	m_Info = pInfo;
}

void CDigivice_Skill::OnClick()
{
}

void CDigivice_Skill::OnHover()
{
}

void CDigivice_Skill::Set_Digimon_Skill_Info_Pos(_float fX, _float fY)
{
	m_pSkill_Info->Set_Hover(true);
	m_pSkill_Info->Set_Move(fX, fY);
}

void CDigivice_Skill::Set_Hover()
{
	m_pSkill_Info->Set_Hover(false);
}

HRESULT CDigivice_Skill::Set_Digimon_SkillSet(_int ID, _int Digimon_Skill)
{
	//Safe_Release(m_pDigimonSKillTextureCom);
	//m_pDigimonSKillTextureCom = nullptr;


	m_iDigimon_ID = ID;

	if (m_pDigimonSKillTextureCom != nullptr)
	{
		Safe_Release(m_pDigimonSKillTextureCom);
		m_pDigimonSKillTextureCom = nullptr;

		// 이름 중복 방지를 위해 ComponentMap에서도 지우는 게 안전
		m_Components.erase(TEXT("Com_Digimon_Skill"));
	}

	/* Com_Digimon_Skill*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), m_pManager->Search_Digimon(m_iDigimon_ID)->DigimonSkillImage,
		TEXT("Com_Digimon_Skill"), reinterpret_cast<CComponent**>(&m_pDigimonSKillTextureCom))))
		return E_FAIL;


	m_iDigimon_Skill = Digimon_Skill;

	Create_Info();

	return S_OK;
}

HRESULT CDigivice_Skill::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Digivice_Skill*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Digivice_Skill"),
		TEXT("Com_Digivice_Skill"), reinterpret_cast<CComponent**>(&m_pSlotTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Digivice_Mask"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Skill::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pSlotTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", 0)))
		return E_FAIL;
	if (FAILED(m_pDigimonSKillTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture2", m_iDigimon_Skill)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDigivice_Skill::Create_Info()
{
	m_pSkill_Info = static_cast<CDigivice_Skill_Info*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(
		ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Digivice_Skill_Info"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Skill_Info")));

	if (m_pSkill_Info == nullptr)
		return E_FAIL;

	m_pSkill_Info->Set_Digimon_Skill_Info(m_iDigimon_ID, m_iDigimon_Skill);

	return S_OK;
}

CDigivice_Skill* CDigivice_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDigivice_Skill* pInstance = new CDigivice_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDigivice_Skill ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject* CDigivice_Skill::Clone(void* pArg)
{
	CDigivice_Skill* pInstance = new CDigivice_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created :CDigivice_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDigivice_Skill::Free()
{
	__super::Free();

	Safe_Release(m_pDigimonSKillTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pSlotTextureCom);
	Safe_Release(m_pShaderCom);
}
