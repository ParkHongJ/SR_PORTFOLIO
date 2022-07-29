#pragma once

namespace Client
{
	static const unsigned int g_iWinSizeX = 1280;
	static const unsigned int g_iWinSizeY = 720;
	
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_HONG, LEVEL_SENI, LEVEL_SJH, LEVEL_GYUH, LEVEL_END };
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern int g_iLoadingTexture;
using namespace Client;
