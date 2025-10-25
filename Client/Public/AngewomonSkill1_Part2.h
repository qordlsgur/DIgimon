#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)


class CAngewomonSkill1_Part2 final : public CPartObject
{
public:
	typedef struct tagBody_Player_Desc : public CPartObject::PARTOBJECT_DESC
	{

	}BODY_PLAYER_DESC;
private:
	CAngewomonSkill1_Part2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAngewomonSkill1_Part2(const CAngewomonSkill1_Part2& Prototype);
	virtual ~CAngewomonSkill1_Part2() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	class CTransform* m_pParentTransformCom = { nullptr };

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pMaskTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_float m_fTime{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CAngewomonSkill1_Part2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END
