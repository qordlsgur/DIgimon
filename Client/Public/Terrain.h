#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
class CTexture;
class CShader;
class CNavigation;
NS_END

NS_BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	typedef struct Object_Data
	{
		_uint Object_Type{};
		_float4x4 World_Matrix{};
	}OBJECT_DATA;
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Load_Object(const _tchar* szFileName);
	template<typename T>
	void LoadObject(const wstring& strPrototype, _float4x4 worldMatrix);

private:
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

	_bool					m_bObjectSet = false;
	vector<CGameObject*>	m_vObjects;
	vector<OBJECT_DATA>		m_vLoadDate;
	CGameObject* m_pObject = { nullptr };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END