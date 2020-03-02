#pragma once

#include "resource.h"
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <string>
#include <iostream>
#include <atlstr.h>
#include <ctime>

#define IDB_ONE 3301

// add from msdn

template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}


#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class DemoApp
{
public:
	DemoApp();
	~DemoApp();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

private:
	// Initialize device-independent resources.
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources.
	HRESULT CreateDeviceResources();

	// Release device-dependent resource.
	void DiscardDeviceResources();

	// Draw content.
	HRESULT OnRender();

	// Resize the render target.
	void OnResize(
		UINT width,
		UINT height
	);

	// The windows procedure.
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	void RePaint() {
		ValidateRect(m_hwnd, NULL);
		//PostMessage(m_hwnd, WM_PAINT, 0, 0);
	}
	HRESULT Draw_Node_Framework(int h, float l, float r, int count,bool isLastChild = false/*,float p_mid_x,int ch_id*/);
	HRESULT Draw_Text(std::string &str,int h,float l,float r);
	HRESULT Draw_Text_At_Left_Bottom(std::string &str);
	HRESULT Draw_Text_At_Right_Bottom(std::string &str);
	HRESULT Clear();
public:
	HWND m_hwnd;
	//HWND m_button;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pBgBrush;
	ID2D1SolidColorBrush* m_pFgBrush;
	ID2D1SolidColorBrush* m_pBranchBrush;
	IDWriteFactory* m_pWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
public:
	DWORD dwTimerId;
	std::string str;
	std::string nxt_str;
	std::string res_str;
	float gridSizeX;
	float gridSizeY;
	float marginSize;
	int line;
	HRESULT t_hr;
};
extern DemoApp app;