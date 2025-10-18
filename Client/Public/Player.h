#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CNavigation;
class CPartObject;
class CCollider;
class CGameObject;
NS_END

NS_BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Intersect_Enemy(_int pEnemy);
	void First_Digimon(CContainerObject* First);
	CContainerObject* First_Digimon() { return m_pFirst_Digimon; }

private:
	void Jump(_float fTimeDelta);
	void LockCamera();


private:
	CPartObject* m_pPart_Body = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	_int m_iEnemy;

	CContainerObject* m_pFirst_Digimon = { nullptr };

	class CStateMachine* m_pFsm = { nullptr };
	class CCamera_Manager* m_pCamera_Manager = { nullptr };
	class CDigimon_Manager* m_pDigimon_Manager = { nullptr };
	class CBattle_Manager* m_pBattle_Manager = { nullptr };
	class CInteraction_Manager* m_pInteraction_Manager = { nullptr };

	_bool m_bMove = false;

	PLAYER_STATE m_eCurrentState;
	PLAYER_STATE m_ePreviousState;

	_bool	m_bisJump = false;
	_float	m_fJumpStart{};
	_float	m_fHight{};
	_float	m_fGravity{};
	_float	m_fJumpPower{};
	_float	m_fJumpTime{};
	_float	m_fMax_Jump{};

	_bool	m_FindCell = { false };
	_bool	m_bOnInteract = { false };

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
