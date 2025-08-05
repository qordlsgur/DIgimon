#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CMouse final : public CBase
{
private:
	CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMouse() = default;

public:
	

private:


public:
	static CMouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;
};

NS_END