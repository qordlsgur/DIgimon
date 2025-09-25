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

	void Set_Digivice_Slot(_int SlotID, _bool Has, _int Digimon_ID);

	//void Copy_Digimon();

public:
	void PartyUHD(class CPartyUHD* pPartyUHD);
	void Digivice(class CDigivice* pDigivice);

	void Player(CGameObject* pPlayer);
	_vector PlayerPos();

	HRESULT Digimon_Add(_int Digimon_ID,DIGIMON_INFO Info);

	DIGIMON_INFO* Search_Digimon(_int ID);

	const _wstring& Get_Model_ID(_int ID);
	const _wstring& Get_Prototype_ID(_int ID);

private:
	class CPartyUHD* m_pPartyUHD = { nullptr };
	class CDigivice* m_pDigivice = { nullptr };

	vector<_bool> m_vDigivice_Battle_Slot;
	_uint m_iDigivice_Number{};


	CGameObject* m_pPlayer = { nullptr };

	map<_int, DIGIMON_INFO> m_Digimon_Info;
	map<_int, const _wstring> m_Digimon_Model;
	map<_int, const _wstring> m_Digimon_Prototype;
private:
	void Model_Name();
	void Digimon_Prototypr();
	DIGIMON_INFO* Find_Digimon(_int Digimon_ID);
	const _wstring* Find_Model(_int Digimon_ID);
	const _wstring* Find_Prototype(_int Digimon_ID);

public:
	virtual void Free() override;
};

NS_END