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
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBattle_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END