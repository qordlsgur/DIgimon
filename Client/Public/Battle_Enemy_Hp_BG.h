#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
class CContainerObject;
NS_END

NS_BEGIN(Client)

class CBattle_Enemy_Hp_BG final : public CUIObject
{
private:
	CBattle_Enemy_Hp_BG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattle_Enemy_Hp_BG(const CBattle_Enemy_Hp_BG& Prototype);
	virtual ~CBattle_Enemy_Hp_BG() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Move(_float fX, _float fY);
	void Set_Enemy(CContainerObject* Enemy);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_tchar			m_szMaxHp[MAX_PATH] = {};
	_tchar			m_szCurrentHp[MAX_PATH] = {};


	class CBattle_Enemy_HpBar* m_pHpBar = { nullptr };
	CContainerObject* m_pEnemy = { nullptr };

	_float m_fMaxHp{};
	_float m_fCurrentHp{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Create_HpBar(const _wstring& strLayerTag);

public:
	static CBattle_Enemy_Hp_BG* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END