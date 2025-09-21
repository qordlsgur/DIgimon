#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)

class CDigimon_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CDigimon_Manager);

private:
	CDigimon_Manager();
	virtual ~CDigimon_Manager() = default;

public:
	HRESULT Initialize();

public:
	void Swap_Digimon(_int Sour, _int Dest);
	void Digimon_UHD();

	void Acquire_Digimon(_int ID);
	void Release_Digimon(_int ID);

	void Set_Digivice_Slot(_int ID, _bool Has);

	//void Copy_Digimon();

public:
	void PartyUHD(class CPartyUHD* pPartyUHD);
	void Digivice(class CDigivice* pDigivice);
	void Digimon_Storage(class CDigimon_Storage* pDigimon_Storage);

	void Player(CGameObject* pPlayer);
	_vector PlayerPos();

	HRESULT Digimon_Add(_int Digimon_ID,DIGIMON_INFO Info);

private:
	class CPartyUHD* m_pPartyUHD = { nullptr };
	class CDigivice* m_pDigivice = { nullptr };
	class CDigimon_Storage* m_pDigimon_Storage = { nullptr };

	vector<_bool> m_vDigivice_Battle_Slot;
	_uint m_iDigivice_Number{};

	vector<class CDigimon_Storage*> m_vDigimon_Storage_Slot;
	_uint m_iDigimon_Storage_Slot_Number{};

	CGameObject* m_pPlayer = { nullptr };

	map<_int, DIGIMON_INFO> m_Digimon_Info;

private:
	DIGIMON_INFO* Find_Digimon(_int Digimon_ID);

public:
	virtual void Free() override;
};

NS_END