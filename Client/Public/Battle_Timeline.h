#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CBattle_Timeline final : public CUIObject
{
private:
	CBattle_Timeline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattle_Timeline(const CBattle_Timeline& Prototype);
	virtual ~CBattle_Timeline() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Digimon(_bool Enemy, _int DigimonId) { m_iEnemy = Enemy; m_iDigimon_Image = DigimonId; }

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTimeLine_TextureCom = { nullptr };
	CTexture* m_pTimelineBGTextureCom = { nullptr };
	CTexture* m_pBattleTimeLineLineTextureCom = { nullptr };
	CTexture* m_pDigimonTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

private:
	_int m_iDigimon_Image{};
	_bool m_iEnemy = { false };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBattle_Timeline* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END