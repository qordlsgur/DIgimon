#include "Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CTransform::CTransform(const CTransform& Prototype)
	: CComponent{ Prototype }
{
}


_float3 CTransform::Get_Scale() const
{
	// 이 함수는 이제 바뀌기 전 초기 Scale을 받아 오기 위해서 선언을 한 함수이다.
	// XMVectorGetX가 3개 있다.
	// 그 이유는 이제 XMVector3Length라는 함수가 이제 저장용이 아닌 계산용으로 만들어 진 함수
	// 인데 값이 _float4로 저장이 되는데 만약 길이가 3이라 하면 3,3,3,3이렇게 된다.
	// 생각을 해 보자면 이제 저거를 계산용으로 한다고 하면 1,0,0,0으로 계산을 하면 결국
	// 3,0,0,0이라서 저렇게 만든 것 같다.
	// 그러면 이제 X를 받든 Y를 받든 Z를 받든 W를 받는 결국 값은 다 똑같아서 X로 받는다.
	return _float3(
		XMVectorGetX(XMVector3Length(Get_State(STATE::RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE::LOOK)))
	);
}

void CTransform::Set_Scale(_float fX, _float fY, _float fZ)
{
	Set_State(STATE::RIGHT, XMVector3Normalize(Get_State(STATE::RIGHT)) * fX);
	Set_State(STATE::UP, XMVector3Normalize(Get_State(STATE::UP)) * fY);
	Set_State(STATE::LOOK, XMVector3Normalize(Get_State(STATE::LOOK)) * fZ);
}

void CTransform::Update_WoldMatrix()
{
	_matrix Sacle = XMMatrixScaling(Get_State(STATE::RIGHT).m128_f32[0], Get_State(STATE::UP).m128_f32[1], Get_State(STATE::LOOK).m128_f32[2]);
	_matrix Rot = XMMatrixRotationY(XMConvertToRadians(-180.f));
	_matrix Translation = XMMatrixTranslation(Get_State(STATE::POSITION).m128_f32[0], Get_State(STATE::POSITION).m128_f32[1], Get_State(STATE::POSITION).m128_f32[2]);
	_matrix World = Sacle * Rot * Translation;

	_float4x4 matWorld;

	XMStoreFloat4x4(&matWorld, World);

	Set_WorldMatrix(matWorld);
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;

	return S_OK;
}

// 셰이더에 Worldmatrix를 넘기기 위해 만들어 둠
HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);

}

void CTransform::Go_Straight(_float fTimeDelta, _int a)
{
	// 이 함수는 앞으로 가기 위한 함수이다.
	// 
	// 저장용 _float4를 계산용 _vector으로 바꾸기 위해서 Get_State에 있는 함수를 이용해서 
	// 값을 계산용에 대입 해준다.
	_vector		vPosition = Get_State(STATE::POSITION);

	// 이 친구도 마찬가지
	_vector		vLook = Get_State(STATE::LOOK);

	// 이제 위치를 옮겨 주는데 여기서 XMVector3Normalize를 사용해서 길이를 1로 만들어 준다.
	// 오해를 하면 안되는데 이제 이 함수는 저 값을 이용해서 1로 만들기만 하지 1로 만든 다음에
	// 저장을 해주는건 아니다 저장을 할거면 대입까지 해줘야함
	// 이제 바라보는 방향으로 속도랑 시간을 입력해서 시간마다 속도로 바라보는 방향으로 값을 
	// 더해준다.
	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	// 이제 Set_State를 이용해서 vPosition(_vector)를 Position(Matrix의 3번째 W값)에
	// 즉, 계산용을 저장용으로 바꿔서 저장을 하기 위해서 밑에 함수안에 XMStroeFloat4를 
	// 이용해서 값을 바꿔준다.
	Set_State(STATE::POSITION, vPosition);
}


void CTransform::Go_Backward(_float fTimeDelta)
{
	// 이 함수는 뒤로 가기 위한 함수이다. 이제 뒤로 가야해서 위의 Straight에 있는 +를 
	// -로 바꿔서 값을 저장 해준다.
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}



void CTransform::Go_Left(_float fTimeDelta)
{
	// 이 함수는 왼쪽으로 이동하기 위해서 Look이 아닌 Right를 이용해서 값을 계산 한다.
	// 그리고 Right 라서 왼쪽으로 가기 위해서 -로 해준다.
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}


void CTransform::Go_Right(_float fTimeDelta)
{
	// 이 함수는 오른쪽으로 이동 하기 위해서 Look이 아닌 Right를 이용해서 값을 계산 한다.
	// 위의 함수는 왼쪽으로 가기 위해서 -를 했지만 이 함수는 오른쪽으로 가는거라 그냥 +로 한다.
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vRight = Get_State(STATE::RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::POSITION, vPosition);
}



void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	// 이 함수는 이제 물체를 프레임마다 회전을 시키기 위해서 사용하는 함수로 임의의 축을
	// 기준으로 메 프레임마다 회전을 해주는 함수이다. 

	// 회전을 하기 위해서 일단 Right,Up,Look을 받아온다.
	_vector		vRight = Get_State(STATE::RIGHT);
	_vector		vUp = Get_State(STATE::UP);
	_vector		vLook = Get_State(STATE::LOOK);

	// 이 함수는 이제 임의의 축으로 회전값 만큼 프레임마다 돌리는 거다.
	// XMMatrixRotationAxis은 원점(0,0,0)?을 기준으로 임의의 축 만큼 돌린다.
	_matrix		RotationMatrix = XMMatrixRotationAxis(
		vAxis, m_fRotationPerSec * fTimeDelta);

	// 이제 위에서 돌린 값들을 각각의 벡터에 집어 넣는다.
	// 그러면 알아서 메트릭스의 값들을 _vector에 집어 넣어 준다.
	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	// 그리고 이제 집어 넣어서 회전시킨 값들을 대입 해준다.
	// Set_State에서 계산용 _vcetor를 저장용인 _float4에 집어 넣어준다.
	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	// 이 함수는 임의의 축으로 한번 회전 한다.
	// 이제 건물이나, 나무 이런 변하지 않는 것들은 이거로 처리한다.

	// 스케일을 바꾸지 않기 위해서 미리 받아온다.
	_float3		vScale = Get_Scale();

	// 크기를 이제 계산용 벡터에 넣어준다.
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	// 그 후에 임의의 축을 받아온 fRadian값 만큼 돌려준다.
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	// 그 후에 움직이지 않으니 w값을 0으로 해주고 값을들 넣어준다.
	// 이거는 그냥 리턴만 해주는 거라서 대입을 해줘야 값이 저장이 됨.
	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	// 그 후에 이제 회전만 한 값을 넣어준다.
	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

// 이제 임의의 축을 구하기 힘들면 그냥 각도를 다 지정 해줘서 값을 넣어주는 방법도 있다.
void CTransform::Rotation(_float fRadianX, _float fRadianY, _float fRadianZ)
{
	_float3		vScale = Get_Scale();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	// 일단 값을 순차적으로 넣어서 계산을 해준다.
	// 그렇게 계산을 하다 보면 중간의 값이 90도가 되는 순간 짐벌락 현상이 일어난다.
	// 짐벌락은 오일러 회전의 중간 축이 90도 일 때 발생함
	// 그래서 이거를 해결하기 위해서 89도로 제한 하거나 쿼터니언 회전으로 해결한다.
	// 짐벌락 : 복합적인 회전을 적용하다보면 특정 축 회전이 
	// 이전 축 회전을 상쇄시켜버리는 경우가 발생하여
	// 특정 축에 대한 회전이 무효화되는 상황이 발생한다.

	// 순차적으로 계산해서 짐벌락 현상이 발생할 수 있음
	// _matrix	RotationMatrix =  XMMatrixRotationX(fRadianX) * 
	// XMMatrixRotationY(fRadianY) * XMMatrixRotationZ(fRadianZ);

	// 복소수가 회전을 의미한다. (a + bi) 회전을 의미한다. 나중에 설명해줌
	// w + xi + yj + zk  (4차원 복소수가 3차원을 의미함 이거는 알아서 찾아봐라)

	// 이 함수는 쿼터니언을 만들어 주는 함수로
	// Pitch: X축
	// Yaw: Y축
	// Roll: Z축
	_vector		vQuternion = XMQuaternionRotationRollPitchYaw(fRadianX, fRadianY, fRadianZ);

	// 위에서 쿼터니언을 받아온다.
	// 위에는 _vector라서 Maxtix로 바꿔주는 함수를 이용해서 벡터를 매트릭스로 바꿔준다.
	// 회전 행렬 공식을 이용해서 쿼터니언을 3x3 행렬로 변환해 준 다음에 남은 값은 단위
	// 행렬로 채워준다.
	_matrix		RotationMatrix = XMMatrixRotationQuaternion(vQuternion);

	// 그 값을 이동을 안하니까 Normal로 받아온다.
	// 다시 말하지만 밑에 함수는 return만 해주므로 값을 대입해서 저장을 해줘야 한다.
	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	// 그 값을 각 위치에 넣어준다.
	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}


// 이 함수는 지금 이 오브젝트가 다른 물체를 바라보게 해 주는 함수이다.
void CTransform::LookAt(_fvector vAt)
{
	// 바라보는거도 결국 회전 이라서 스케일 값이 변하면 안되서 스케일 값을 저장 해둔다.
	_float3		vScale = Get_Scale();

	// 일단 Look(바라보는 방향)을 바꿔줘야 하는데 내가 at을 봐야 하는거니까 
	// 목표 위치 - 현재 위치를 해준다.
	_vector		vLook = vAt - Get_State(STATE::POSITION);

	// 그리고 그렇게 나온 Look값을 Up의 값과(임시) 외적 해주는데 계산하는 순서가 중요함
	// 그 이유는 두 벡터의 수직인 벡터를 만들어야 하는데 오른손의 법칙으로
	// 외적의 방향은 엄지(Up)은 A Look은 검지(Look)이다.
	// 그래서 Look은 바라보는 전방 방향 벡터인데 Up은 보통 위쪽 방향 벡터 이다.
	// 계산 방법은 무조건 Up이 먼저 Look이 뒤에 온다 반대로 하면 법선이 반대로 생김
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);

	// 이제 Right를 구했으면 Up을 임시가 아닌 값을 구해야 해서 Look이랑 계산한 Right를 
	// 이용해서 Up을 구해준다. 이거도 마찬가지로 순서를 잘 해줘야 결과값이 나온다.
	_vector		vUp = XMVector3Cross(vLook, vRight);

	// 계산해서 나온 값을 대입 해준다.
	// 스케일이 바뀌지 않게 스케일도 곱해줌
	Set_State(STATE::RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE::UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE::LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::SizeUp(_float fX, _float fY, _float fZ)
{
	_float3 vScale = Get_Scale();

	// 누적해서 스케일 계산
	vScale.x += fX;
	vScale.y += fY;
	vScale.z += fZ;

	// 각 축 벡터 방향은 그대로, 길이만 스케일 적용
	Set_State(STATE::RIGHT, XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x);
	Set_State(STATE::UP, XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y);
	Set_State(STATE::LOOK, XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z);
}

void CTransform::TargetLook(_fvector Rarget)
{
	_vector Pos = Get_State(STATE::POSITION);

	_vector dir = Rarget - Pos;

	dir.m128_f32[1] = 0.f;

	if (XMVector3LengthSq(dir).m128_f32[0] > 0.0001f)
	{
		dir = XMVector3Normalize(dir);
		_float yaw = atan2f(XMVectorGetX(dir), XMVectorGetZ(dir));

		Rotation(0.f, yaw, 0.f);
	}
}

void CTransform::Jump(_float fHight)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vUp = Get_State(STATE::UP);

	vPosition += XMVector3Normalize(vUp) * fHight;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Set_Y(_float y)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vUp = Get_State(STATE::UP);

	vPosition.m128_f32[1] = y;

	Set_State(STATE::POSITION, vPosition);
}

void CTransform::Go_Straight(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE::POSITION);
	_vector		vLook = Get_State(STATE::LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation ||
		true == pNavigation->isMove(vPosition))
		Set_State(STATE::POSITION, vPosition);
}

_bool CTransform::FollowPlayer(_vector fTarget, _float fFollowDistance, _float fTimeDelta)
{
	_vector Pos = Get_State(STATE::POSITION);

	_vector Target = XMVectorSet(fTarget.m128_f32[0], 0.f, fTarget.m128_f32[2], fTarget.m128_f32[3]);

	_vector Dir = XMVectorSubtract(Target, Pos);
	_vector LenVec = XMVector3Length(Dir);
	_float Distance = XMVectorGetX(LenVec);

	if (Distance >= fFollowDistance)
		isFollow = true;

	if (Distance <= 10)
	{
		isFollow = false;
		return false;
	}

	if (isFollow)
	{
		Pos += XMVector3Normalize(Dir) * (m_fSpeedPerSec * 0.7f) * fTimeDelta;
		Set_State(STATE::POSITION, Pos);
		return true;
	}
}

void CTransform::Look(_vector Angle, _float fTimeDelta)
{
	_vector vRight = Get_State(STATE::RIGHT);
	_vector vUp = Get_State(STATE::UP);
	_vector vLook = Get_State(STATE::LOOK);

	_vector Look = XMVector3Normalize(vLook);
	_vector NorAngle = XMVector3Normalize(Angle);

	_float vDot = XMVectorGetX(XMVector3Dot(Look, NorAngle));

	_float vAngle = acos(clamp(vDot, -1.f, 1.f));

	_vector cross = XMVector3Cross(Look, NorAngle);

	_float Sing = XMVectorGetY(cross);
	if (fabs(Sing) <= 0.0001f)
		Sing = 0.f;
	Sing = (Sing >= 0) ? 1.f : -1.f;

	_float Max = m_fSpeedPerSec * fTimeDelta;

	_float TargetAngle = min(vAngle, Max) * Sing;

	_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), TargetAngle);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::LookAtPlayer(_vector fTarger, _float fTimeDelta)
{
	_vector	vRight = Get_State(STATE::RIGHT);
	_vector	vUp = Get_State(STATE::UP);
	_vector	vLook = Get_State(STATE::LOOK);

	_vector pos = Get_State(STATE::POSITION);
	_vector Look = XMVector3Normalize(vLook);

	_vector Target = XMVectorSet(fTarger.m128_f32[0], 0.f, fTarger.m128_f32[2], fTarger.m128_f32[3]);

	_vector Dir = XMVector3Normalize(XMVectorSubtract(Target, pos));

	_float Dot = XMVectorGetX(XMVector3Dot(Dir, Look));

	_float vAngle = acos(clamp(Dot, -1.f, 1.f));

	_vector Cross = XMVector3Cross(Look, Dir);

	_float Sing = XMVectorGetY(Cross);

	if (fabs(Sing) <= 0.0001f)
		Sing = 0.f;

	Sing = (Sing >= 0) ? 1.f : -1.f;

	_matrix RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), vAngle * Sing * m_fSpeedPerSec * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::TurnY(_float fAngle, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE::RIGHT);
	_vector		vUp = Get_State(STATE::UP);
	_vector		vLook = Get_State(STATE::LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fAngle * m_fRotationPerSec * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE::RIGHT, vRight);
	Set_State(STATE::UP, vUp);
	Set_State(STATE::LOOK, vLook);
}

void CTransform::Target_Pos_Move(_fvector Target_Pos, _float fTimeDelta)
{
	_vector vPos = Get_State(STATE::POSITION);

	// 목표까지 방향 계산
	_vector Dir = XMVector3Normalize(Target_Pos - vPos);

	// 이번 프레임 이동량
	_vector Move = Dir * m_fSpeedPerSec * fTimeDelta;

	// 목표까지 남은 거리 계산
	float Distance = XMVectorGetX(XMVector3Length(Target_Pos - vPos));
	const float Threshold = 0.1f; // 도착 판정 범위

	if (Distance <= Threshold)
	{
		// 도착하면 위치를 목표로 맞추고 이동 종료
		Set_State(STATE::POSITION, Target_Pos);
		return;
	}

	// 이동량이 남은 거리보다 크면, 목표 위치로 바로 이동
	if (XMVectorGetX(XMVector3Length(Move)) > Distance)
		Move = Target_Pos - vPos;

	// 위치 갱신
	Set_State(STATE::POSITION, vPos + Move);
}

_bool CTransform::Target_Pos_Move_Bool(_fvector Target_Pos, _float fTimeDelta)
{
	_vector vPos = Get_State(STATE::POSITION);

	_vector toTarget = Target_Pos - vPos;
	float distance = XMVectorGetX(XMVector3Length(toTarget));

	const float Threshold = 0.1f;
	if (distance <= Threshold)
	{
		Set_State(STATE::POSITION, Target_Pos);
		return false; // 이동 완료
	}

	_vector dir = XMVector3Normalize(toTarget);
	_vector move = dir * m_fSpeedPerSec * fTimeDelta;

	// 이동량이 남은 거리보다 크면 목표 위치로 바로 이동
	if (XMVectorGetX(XMVector3Length(move)) > distance)
		move = toTarget;

	Set_State(STATE::POSITION, vPos + move);
	return true; // 이동 중
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(" Failed to Create : Transform ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(" Failed to Cloned : Transform ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
