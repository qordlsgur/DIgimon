#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
class CTexture;
class CShader;
class CNavigation;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CBattle_Terrain final : public CGameObject
{
public:
	typedef struct Object_Data
	{
		_uint Object_Type{};
		_float4x4 World_Matrix{};
	}OBJECT_DATA;
private:
	CBattle_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattle_Terrain(const CBattle_Terrain& Prototype);
	virtual ~CBattle_Terrain() = default;

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

	class CBattle_Manager* m_pBattle_Manager = { nullptr };
	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };

	_bool m_bBattle = { false };

	_int m_iMaxDigimon{};

	_vector m_vPlayerDigimonPos[5] = {};
	_vector m_vMonsterDigimonPos[5] = {};
	_vector m_vPlayerPos{};

	_vector m_vPlayerDigimonAttackPos[5] = {};
	_vector m_vMonsterDigimonAttackPos[5] = {};

	_bool					m_bObjectSet = false;
	vector<CGameObject*>	m_vObjects;
	vector<OBJECT_DATA>		m_vLoadDate;
	CGameObject* m_pObject = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBattle_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END


