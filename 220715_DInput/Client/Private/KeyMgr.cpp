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
	// ������ ���� ���� ����, �� ������ ������ ��츸 ��
	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = true;
		return true;
	}

	// ���� ��� key���� ���� üũ
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

��ü A
if (keyMge::get_Instance()->KeyDown('K')) 

��ü B
if (keyMge::get_Instance()->KeyDown('K'))

��ü A�� B�� �Ѵ� Ű �ٿ� Ű�� K�� �����ִ�
���� ��ü A���� KeyDown�� �����ϰ� �Ǹ� ��ü B�� keyDown�� �����ȴ�.

m_bKeyState[_iKey]�� bool���� true�� �ٲٱ� ����
*/

bool CKeyMgr::Key_Up(int _iKey)
{
	// ������ ������ �ְ�, �� ������ ������ �ʾ��� ��� ��
	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = false;
		return true;
	}

	// ���� ��� key���� ���� üũ
	for (int i = 0; i < 0xff; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
		{
			m_bKeyState[i] = false;
		}
	}

	return false;
}