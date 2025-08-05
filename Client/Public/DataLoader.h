#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Client)

class CDataLoader final : public CBase
{
	typedef struct Digimon_Data
	{
		_tchar Imaga_name[64];
		_tchar Digimon_name[24];
		_int   Digimon_Type;
		_int   Digimon_Stage;
	}DIGIMONDATADESC;
private:
	CDataLoader();
	virtual ~CDataLoader();

public:
	void Initialize_Prototype(const _tchar* pShaderFilePath);

private:
	_bool LoadData();

	vector<Digimon_Data> Digimon_Data;

private:
	static CDataLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath);
	virtual void Free() override;
};

NS_END