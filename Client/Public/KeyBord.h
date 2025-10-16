#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CKeyBord final : public CUIObject
{
private:
	CKeyBord(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKeyBord(const CKeyBord& Prototype);
	virtual ~CKeyBord() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_KeyBord(_int Key);
	void Set_Move(_float fX, _float fY);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pSelectTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_int m_iKey{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CKeyBord* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END