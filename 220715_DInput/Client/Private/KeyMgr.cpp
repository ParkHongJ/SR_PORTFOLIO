#include "stdafx.h"
#include "KeyMgr.h"


CKeyMgr* CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}


CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _iKey)
{
	if ((GetAsyncKeyState(_iKey) & 0x8000)) {
		m_bKeyState[_iKey] = true;
		return true;
	}

	return false;
}

bool CKeyMgr::Key_Down(int _iKey)
{
	// 이전에 눌린 적이 없고, 현 시점에 눌렸을 경우만 참
	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = true;
		return true;
	}

	// 현재 모든 key눌림 상태 체크
	for (int i = 0; i < 0xff; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
		{
			m_bKeyState[i] = false;
		}
	}

	return false;
}

/* 
[issue]

객체 A
if (keyMge::get_Instance()->KeyDown('K')) 

객체 B
if (keyMge::get_Instance()->KeyDown('K'))

객체 A와 B가 둘다 키 다운 키를 K로 쓰고있다
만일 객체 A에서 KeyDown이 성공하고 되면 객체 B의 keyDown은 생략된다.

m_bKeyState[_iKey]의 bool값을 true로 바꾸기 때문
*/

bool CKeyMgr::Key_Up(int _iKey)
{
	// 이전에 눌림이 있고, 현 시점에 눌리지 않았을 경우 참
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = false;
		return true;
	}

	// 현재 모든 key눌림 상태 체크
	for (int i = 0; i < 0xff; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
		{
			m_bKeyState[i] = false;
		}
	}

	return false;
}
