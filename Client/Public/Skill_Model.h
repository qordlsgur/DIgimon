#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)

class CSkill_Model final : public CGameObject
{
private:
	CSkill_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Model(const CSkill_Model& Prototype);
	virtual ~CSkill_Model() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CModel* m_pModel1Com = { nullptr };
	CModel* m_pModel2Com = { nullptr };
	CModel* m_pModel3Com = { nullptr };
	CModel* m_pModel4Com = { nullptr };
	CTexture* m_pTexture1Com = { nullptr };
	CTexture* m_pTexture2Com = { nullptr };
	CTexture* m_pTexture3Com = { nullptr };
	CTexture* m_pTexture4Com = { nullptr };
	CTexture* m_pTexture5Com = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	_float m_fTime{};

	_bool m_bOn = { false };
	_int a{};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CSkill_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END