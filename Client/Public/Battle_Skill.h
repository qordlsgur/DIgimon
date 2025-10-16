#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)

class CBattle_Skill final : public CUIObject
{
private:
	CBattle_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattle_Skill(const CBattle_Skill& Prototype);
	virtual ~CBattle_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Move(_float fX, _float fY);

	void Set_HasDigimon(_bool Digimon) { m_bHasDigimon = Digimon; }
	_bool Get_HasDigimon() { return m_bHasDigimon; }

	void Set_Info(DIGIMON_INFO* pInfo);

	virtual void OnClick() override;
	virtual void OnHover() override;

	void Set_Digimon_Skill_Info_Pos(_float fX, _float fY);
	void Set_Hover();

	HRESULT Set_Digimon_SkillSet(_int ID, _int Digimon_Skill);

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pSlotTextureCom = { nullptr };
	CTexture* m_pDigimonSKillTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };

	class CDigimon_Manager* m_pManager = { nullptr };
	class CDigivice_Skill_Info* m_pSkill_Info = { nullptr };

	DIGIMON_INFO* m_Info;

	_int			m_iDigimon_ID{};

	_bool			m_bHasDigimon = false;

	_int			m_iDigimon_Skill{};

	_wstring		m_strDigimon_SkillName;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	HRESULT Create_Info();

public:
	static CBattle_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END