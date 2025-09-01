#include "Slot.h"

#include "GameInstance.h"

CSlot::CSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CSlot::CSlot(const CSlot& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlot::Initialize(void* pArg)
{
	CUIObject::UIOBJECT_DESC	Desc{};

	Desc.fX = 100;
	Desc.fY = 100;
	Desc.fSizeX = 40.f;
	Desc.fSizeY = 40.f;

	m_pRect = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSlot::Priority_Update(_float fTimeDelta)
{
}

void CSlot::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_Z))
		a = 0;
		//m_bHover = true;

	if (m_pGameInstance->Key_Down(DIK_X))
		a = 1;
	//m_bHover = false;

	// 부모 + 로컬
	_vector vWorldPos = XMVectorSet(m_fX + m_fParent_WorldPos.x,-(m_fY)+m_fParent_WorldPos.y,0.f, 1.f);

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(STATE::POSITION, vWorldPos);

	_float4 Pos;
	XMStoreFloat4(&Pos, vWorldPos);

	// RECT는 화면 좌상단 기준으로 변환
	m_pRect = {
		long(Pos.x - m_fSizeX * 0.5f + m_fWinSizeX * 0.5f),
		long(-Pos.y - m_fSizeY * 0.f - m_fWinSizeY * 0.5f),
		long(Pos.x + m_fSizeX * 0.5f + m_fWinSizeX * 0.5f),
		long(-Pos.y + m_fSizeY * 0.f - m_fWinSizeY * 0.5f)
	};
}

void CSlot::Late_Update(_float fTimeDelta)
{

}

HRESULT CSlot::Render()
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

	__super::End();

	return S_OK;
}

void CSlot::Set_Parent_WorldPos(_vector fParent_World)
{
	XMStoreFloat4(&m_fParent_WorldPos, fParent_World);
}

void CSlot::Set_Move(_float fX, _float fY)
{
	m_fX = fX;
	m_fY = fY;

	m_pRect = { long(m_fX - m_fSizeX * 0.5f),long(m_fY - m_fSizeY * 0.5f),long(m_fX + m_fSizeX * 0.5f),long(m_fY + m_fSizeY * 0.5f) };

}


void CSlot::OnClick()
{
	int a = 10;
}

HRESULT CSlot::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Inventory_Slot"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Shader_Hover"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlot::Bind_ShaderResources()
{
	/*m_pShaderCom->Bind_Matrix("g_WorldMatrix", );*/
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Int("g_Hover", m_bHover)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture1", a)))
		return E_FAIL;

	return S_OK;
}

CSlot* CSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSlot* pInstance = new CSlot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSlot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSlot::Clone(void* pArg)
{
	CSlot* pInstance = new CSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSlot ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlot::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
