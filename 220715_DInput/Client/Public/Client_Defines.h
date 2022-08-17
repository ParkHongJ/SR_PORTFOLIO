#pragma once
#include <random>
#include <cmath>

namespace Client
{
	static const unsigned int g_iWinSizeX = 1280;
	static const unsigned int g_iWinSizeY = 720;
	
	enum LEVEL { LEVEL_STATIC, LEVEL_STAGE1, LEVEL_STAGE2, LEVEL_STAGE3, LEVEL_STAGE4, LEVEL_STAGE5, LEVEL_STAGE6, LEVEL_STAGE7, LEVEL_STAGE8, LEVEL_STAGE9, LEVEL_STAGE10, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_HONG, LEVEL_SENI, LEVEL_SJH, LEVEL_GYUH, LEVEL_GYUTEST, LEVEL_END };

}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern int g_iLoadingTexture;
using namespace Client;
