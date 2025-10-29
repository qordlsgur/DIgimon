#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	// 이 구조체는 이제 회전 값이랑 스피드를 받아오는데 여기서 받는 
	// 회전 값은 이제 건물 같은 것 들이 시작하자 마자 회전이 되니까 그걸
	// 처리 해 주기 위해서 받아온다.
	typedef struct tagTransformDesc
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;

	}TRANSFORM_DESC;
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& Prototype);
	virtual ~CTransform() = default;

public:
	// 이 함수는 Matrix에 있는 x(Right),y(Up),z(Look)을 받아 올 때 어떤걸 받아서 불러올 지
	// 정해주는 함수이다. 만약 eState에 Right를 집어 넣으면 x값 즉 Right의 값 Matrix의 0번을
	// _float4 함수에 저장을 한다.
	_vector Get_State(STATE eState) const {
		return XMLoadFloat4(reinterpret_cast<const _float4*>(
			&m_WorldMatrix.m[ENUM_CLASS(eState)]));
	}

	_float3 Get_Scale() const; // 스케일 받기

	// 월드 매트릭스를 던져준다.
	const _float4x4* Get_WorldMatrixPtr() const {
		return &m_WorldMatrix;
	}

	// 이 함수는 이제 계산을 한 후에 x(Right),y(Up),z(Look), 중에 어디에 뭐를 집어 넣을 지 
	// 값을 세팅 해주는 함수 이다. 만약 Right에 vState값을 넣어 주고 싶다고 하면 이제 eState에
	// Right를 넣고 뒤에 계산용 벡터를 집어 넣으면 그 계산용 벡터를 저장용에 집어 넣어 준다.
	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4(reinterpret_cast<_float4*>(
			&m_WorldMatrix.m[ENUM_CLASS(eState)]), vState);
	}

	// 이 함수는 스케일 값을 조절 해주는 함수이다.
	// 스케일 값이 바뀔때 이거를 사용한다.
	void Set_Scale(_float fX, _float fY, _float fZ);

	void Set_WorldMatrix(_float4x4 Matrix) { m_WorldMatrix = Matrix; }

	void Update_WoldMatrix();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	// Shader에 Wolrd maxrix를 넘기기 위해서 선언을 해줌
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

public:
	// 말 그대로 앞,뒤,좌,우 이동을 하는 함수
	void Go_Straight(_float fTimeDelta, _int a = 0);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);

	// 임의의 축을 기준으로 회전을 해준다.
	void Turn(_fvector vAxis, _float fTimeDelta);

	// 위의 함수는 매 프레임마다 회전을 한다고 하면 이 함수는 건물이나 나무 처럼 처음 생성이
	// 될 때 한번만 회전 하는 친구들이 사용하는 함수이다. 임의의 축으로 값 만큼 한번 회전 한다.
	void Rotation(_fvector vAxis, _float fRadian);

	// 이 함수는 위에 있는 임의의 축을 기준으로 회전을 하는 함수가 있는데 이제 그 축을 바로 
	// 생각을 해서 적용하기에 어려움이 있어서 그게 아니라 회전하는 값 자체를 임의의 축이 아니라
	// x, y, z를 이용해서 회전을 해주는 함수 이다.
	void Rotation(_float fRadianX, _float fRadianY, _float fRadianZ);

	// 이 함수는 물체를 Look 처다보게 해 주는 함수이다.
	void LookAt(_fvector vAt);

	void SizeUp(_float fX, _float fY, _float fZ);

	void TargetLook(_fvector Rarget);

public:
	void Jump(_float fHight);
	void Set_Y(_float y);
	void Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	_bool FollowPlayer(_vector fTarget, _float fFollowDistance, _float fTimeDelta);
	void Look(_vector Angle, _float fTimeDeata);
	void LookAtPlayer(_vector fTarger, _float fTimeDelta);
	void LookAtCamera(_vector fTarger,_float Speed, _float fTimeDelta);
	void TurnY(_float fAngle, _float fTimeDelta);
	void TurnY(_float fAngle, _float Speed, _float fTimeDelta);
	void Target_Pos_Move(_fvector Target_Pos, _float fTimeDelta);
	void Target_Pos_Move_Speed(_fvector Target_Pos, _float Speed, _float fTimeDelta);
	_bool Target_Pos_Move_Bool(_fvector Target_Pos, _float fTimeDelta);
	_bool Target_Pos_Move_Bool_Speed(_fvector Target_Pos,_float Speed, _float fTimeDelta);

private:
	// 오브젝트의 이동 속도
	_float				m_fSpeedPerSec = {};

	// 오브젝트의 회전 값 이제 한번 회전할 때 그 회전 값을 저장해주는 변수
	_float				m_fRotationPerSec = {};

	// 월드 매트릭스
	_float4x4			m_WorldMatrix{};

	_bool				m_bisMove = false;
	_bool				isFollow = false;
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END