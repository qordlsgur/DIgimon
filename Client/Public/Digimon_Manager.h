#pragma once

#include "Client_Defines.h"
#include "Base.h"

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
	void Swap_Digimon();
	void Digimon_UHD();

	void Acquire_Digimon();

	void Copy_Digimon();

	void PartyUHD(class CPartyUHD* pPartyUHD);
	void Digivice(class CDigivice* pDigivice);
	void Digimon_Storage(class CDigimon_Storage* pDigimon_Storage);

private:
	class CPartyUHD* m_pPartyUHD = { nullptr };
	class CDigivice* m_pDigivice = { nullptr };
	class CDigimon_Storage* m_pDigimon_Storage = { nullptr };

	vector<class CDigivice*> m_vDigivice_Battle_Slot;
	_uint m_iDigivice_Number{};

	vector<class CDigimon_Storage*> m_vDigimon_Storage_Slot;
	_uint m_iDigimon_Storage_Slot_Number{};



public:
	virtual void Free() override;
};

NS_END