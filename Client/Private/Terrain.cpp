#include "Terrain.h"

#include "GameInstance.h"
#include "Tennis.h"
#include "BldA.h"
#include "BldB.h"
#include "BldC.h"
#include "BldD.h"
#include "BldE.h"
#include "BldH.h"
#include "BldJ.h"
#include "BldK.h"
#include "Navigation.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//Load_Object(TEXT("../Bin/Resources/Textures/Objects.bin"));
	 

	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CTerrain::Update(_float fTimeDelta)
{
	m_pNavigationCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrixPtr()));
}

void CTerrain::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Resources()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pNavigationCom->Render();
#endif

	return S_OK;
}

void CTerrain::Load_Object(const _tchar* szFileName)
{
	ifstream in(szFileName, ios::binary);
	if (!in)
		return;

	size_t size = 0;
	in.read(reinterpret_cast<_char*>(&size), sizeof(size));

	m_vLoadDate.resize(size);

	in.read(reinterpret_cast<char*>(m_vLoadDate.data()), sizeof(OBJECT_DATA) * size);


	for (auto& it : m_vLoadDate)
	{

		switch (it.Object_Type)
		{
		case ENUM_CLASS(OBJECT::TENNIS):
			LoadObject<CTennis>(L"Prototype_GameObject_Tennis", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING1):
			LoadObject<CBldA>(L"Prototype_GameObject_BldA", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING2):
			LoadObject<CBldB>(L"Prototype_GameObject_BldB", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING3):
			LoadObject<CBldC>(L"Prototype_GameObject_BldC", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING4):
			LoadObject<CBldD>(L"Prototype_GameObject_BldD", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING5):
			LoadObject<CBldE>(L"Prototype_GameObject_BldE", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING6):
			LoadObject<CBldH>(L"Prototype_GameObject_BldH", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING7):
			LoadObject<CBldJ>(L"Prototype_GameObject_BldJ", it.World_Matrix);
			break;

		case ENUM_CLASS(OBJECT::BILDING8):
			LoadObject<CBldK>(L"Prototype_GameObject_BldK", it.World_Matrix);
			break;
		}
	}
}

template<typename T>
void CTerrain::LoadObject(const wstring& strPrototype, _float4x4 worldMatrix)
{
	if (!m_bObjectSet)
		m_bObjectSet = true;
	int iObject_Count{};

	iObject_Count = static_cast<int>(m_vObjects.size());
	wstring strName = L"Object_Layer";

	m_pObject = dynamic_cast<T*>(m_pGameInstance->Add_GameObject_ToLayer_ToCreate(ENUM_CLASS(LEVEL::GAMEPLAY), strPrototype,
		ENUM_CLASS(LEVEL::GAMEPLAY), strName));

	m_pObject->Set_Matrix(worldMatrix);

	m_vObjects.push_back(m_pObject);
}

HRESULT CTerrain::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Terrain"),
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


	/* Com_Navigation */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
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

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();


	m_vObjects.clear();
	m_vLoadDate.clear();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
