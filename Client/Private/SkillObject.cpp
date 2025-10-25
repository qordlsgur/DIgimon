#include "SkillObject.h"
#include "GameInstance.h"

CSkillObject::CSkillObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CContainerObject{ pDevice, pContext }
{
}

CSkillObject::CSkillObject(const CSkillObject& Prototype)
    : CContainerObject{ Prototype }
{
}

HRESULT CSkillObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkillObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CSkillObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CSkillObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CSkillObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CSkillObject::Render()
{
    return S_OK;
}

void CSkillObject::Update_Position(_fvector Position)
{
    m_pTransformCom->Set_State(STATE::POSITION, Position);
}

void CSkillObject::Set_Move(_vector Position)
{
}

void CSkillObject::Move(_float fTimeDatel)
{
}

void CSkillObject::Free()
{
    __super::Free();
}
