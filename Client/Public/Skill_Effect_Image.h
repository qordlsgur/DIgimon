#pragma once

#include "Client_Defines.h"
#include "Effect.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CSkill_Effect_Image final : public CEffect
{
private:
	CSkill_Effect_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Effect_Image(const CSkill_Effect_Image& Prototype);
	virtual ~CSkill_Effect_Image() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pDissolveTextureCom = { nullptr };
	CTexture* m_pMaskTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_float m_fTime{};
	_int a{};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CSkill_Effect_Image* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END
