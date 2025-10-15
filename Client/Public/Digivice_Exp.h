#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CDigivice_Exp final : public CUIObject
{
private:
	CDigivice_Exp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDigivice_Exp(const CDigivice_Exp& Prototype);
	virtual ~CDigivice_Exp() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_MaxExp(_float MaxExp);
	void Set_Move(_float fX, _float fY);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pBgTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_float m_iMaxExp{};
	_float m_iCurrentExp{};
	_float m_fExpRatio{};


private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDigivice_Exp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END