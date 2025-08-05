#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	typedef		bool						_bool;

	typedef		signed char					_byte;
	typedef		unsigned char				_ubyte;
	
	typedef		signed short				_short;
	typedef		unsigned short				_ushort;

	typedef		signed int					_int;
	typedef		unsigned int				_uint;

	typedef		signed long					_long;
	typedef		unsigned long				_ulong;

	typedef		char						_char;
	typedef		wchar_t						_tchar;

	typedef		float						_float;
	typedef		double						_double;
	typedef		wstring						_wstring;

	typedef		XMFLOAT2					_float2;  // 저장용
	typedef		XMFLOAT3					_float3;  // 저장용
	typedef		XMFLOAT4					_float4;  // 저장용

	typedef		XMVECTOR					_vector;  // 계산용

	typedef		FXMVECTOR					_fvector;  // 매개변수로 받을 때 1~3번 까지는 f를 사용
	typedef		GXMVECTOR					_gvector;  // 매개변수로 받을 때 4번은 g를 사용
	typedef		HXMVECTOR					_hvector;  // 매개변수로 받을 때 5~6번은 h를 사용
	typedef		CXMVECTOR					_cvector;  // 매개변수로 받을 때 7번 이상은 c를 사용

	typedef		XMFLOAT4X4					_float4x4; // 저장용

	typedef		XMMATRIX					_matrix;   // 계산용

	typedef		FXMMATRIX					_fmatrix;  // 매개변수로 받을 때 1번은 f
	typedef		CXMMATRIX					_cmatrix;  // 매개변수로 받을 때 2번 이상은 다 c

}

#endif // Engine_Typedef_h__
