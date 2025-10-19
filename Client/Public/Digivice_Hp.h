#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CDigivice_Hp final : public CUIObject
{
private:
	CDigivice_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDigivice_Hp(const CDigivice_Hp& Prototype);
	virtual ~CDigivice_Hp() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_MaxHp(_int MaxHp);
	void Set_CurrentHp(_int CurrentHp);
	void Set_Damage(_int Damage);
	void Set_Move(_float fX, _float fY);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pBgTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_int m_fMaxHp{};
	_int m_fCurrentHp{};
	_float m_fHpRatio{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDigivice_Hp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END