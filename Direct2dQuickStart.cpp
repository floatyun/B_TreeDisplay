// Direct2dQuickStart.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Direct2dQuickStart.h"
#include <Windows.h>
#include "Work.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
DemoApp app;
void CALLBACK TimeProc(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime
)
{
	if (app.dwTimerId == idTimer) {
		//app.json = std::to_string(time(NULL));
		app.Clear();
		if (!work(app.line)) {
			app.Draw_Text_At_Left_Bottom(app.str);
			app.RePaint();
			Sleep(1000);
			PostMessage(app.m_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			app.Draw_Text_At_Left_Bottom(app.str);
			app.RePaint();
		}
	}
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

DemoApp::DemoApp() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pBgBrush(NULL),
	m_pFgBrush(NULL),
	m_pBranchBrush(NULL),
	m_pWriteFactory(NULL),
	m_pTextFormat(NULL),
	dwTimerId(0),
	gridSizeX(10),
	gridSizeY(80),
	marginSize(5),
	line(-1),
	t_hr(S_OK)
{
}

DemoApp::~DemoApp()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBgBrush);
	SafeRelease(&m_pFgBrush);
	SafeRelease(&m_pBranchBrush);
	SafeRelease(&m_pWriteFactory);
	SafeRelease(&m_pTextFormat);
	//KillTimer(NULL, dwTimerId);
}

void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DemoApp::Initialize()
{
	HRESULT hr;

	// Initialize device-indpendent resources, such
	// as the Direct2D factory.
	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr))
	{
		// Register the window class.
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DDemoApp";

		RegisterClassEx(&wcex);


		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size.
		FLOAT dpiX, dpiY;

		// The factory returns the current system DPI. This is also the value it will use
		// to create its own windows.
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);


		// Create the window.
		m_hwnd = CreateWindow(
			L"D2DDemoApp",
			L"Direct2D B_Tree Paint",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(1200.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(900.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	if (!work_Initialize()) {
		PostQuitMessage(0);
	}

	return hr;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	// Create a Direct2D factory.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);

	if (SUCCEEDED(hr)) {
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pWriteFactory)
		);
	}

	if (SUCCEEDED(hr)) {
		hr = m_pWriteFactory->CreateTextFormat(
			L"Consolas",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			18.0f,
			L"en-us",
			&m_pTextFormat
		);
	}

	return hr;
}

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		// Create a Direct2D render target.
		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);


		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Pink),
				&m_pBgBrush
			);
		}
		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black), // Color
				&m_pFgBrush
			);
		}

		if (SUCCEEDED(hr))
		{
			// Create a blue brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Red), // Color
				&m_pBranchBrush
			);
		}

		
	}

	t_hr = hr;
	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pBgBrush);
	SafeRelease(&m_pFgBrush);
	SafeRelease(&m_pBranchBrush);
	SafeRelease(&m_pWriteFactory);
	SafeRelease(&m_pTextFormat);
}

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp *pDemoApp = (DemoApp *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
		);
		result = 1;
	}
	else if (message == WM_RBUTTONDOWN) {
		app.Clear();
		if (!work(app.line)) {
			//app.Draw_Text_At_Left_Bottom(app.line);
			//app.RePaint();
			MessageBoxW(NULL, _T("已经处理完文件中的所有命令"), _T("程序结束"), MB_OK);
			PostMessage(app.m_hwnd, WM_DESTROY, 0, 0);
		}
		else {
			std::string t = app.str +"\n"+ app.nxt_str;
			app.Draw_Text_At_Left_Bottom(t);
			app.Draw_Text_At_Right_Bottom(app.res_str);
			app.RePaint();
		}
	}
	else
	{
		DemoApp *pDemoApp = reinterpret_cast<DemoApp *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pDemoApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pDemoApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

HRESULT DemoApp::Draw_Node_Framework(int h, float l, float r, int count, bool isLastChild/*, float p_mid_x, int ch_id*/)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		D2D1_RECT_F node_rect = D2D1::RectF(
			l,
			2 * h*gridSizeY,
			r - gridSizeX,
			(2 * h + 1)*gridSizeY
		);
		// 画node的大矩形
		m_pRenderTarget->FillRectangle(&node_rect, m_pBgBrush);
		m_pRenderTarget->DrawRectangle(&node_rect, m_pFgBrush);

		float each_width = (r - l) / count;
		float x;
		// 画内部关键字之间的分隔线
		for (int i = 0; i < count; ++i)
		{
			x = l + i * each_width;
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 2 * h*gridSizeY),
				D2D1::Point2F(static_cast<FLOAT>(x), (2 * h + 1)*gridSizeY),
				m_pFgBrush,
				0.5f
			);
		}

		// 画父节点与本节点的连线
		if (isLastChild) {
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(l - (r - l) *0.382f), (2 * h - 1)*gridSizeY),
				D2D1::Point2F(static_cast<FLOAT>(l), (2 * h)*gridSizeY),
				m_pBranchBrush,
				1.0f
			);
		}
		else {
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(l + (r - l) *0.382f), (2 * h - 1)*gridSizeY),
				D2D1::Point2F(static_cast<FLOAT>(l), (2 * h)*gridSizeY),
				m_pBranchBrush,
				1.0f
			);
		}
		
		// 告知程序画完了
		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

HRESULT DemoApp::Draw_Text(std::string & str, int h, float l, float r)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);
		D2D1_RECT_F key_area_rect = D2D1::RectF(
			l+marginSize,
			2 * h*gridSizeY,
			r - gridSizeX,
			(2 * h + 1)*gridSizeY
		);
		// Draw text
		CString tmpStr(str.c_str());
		m_pRenderTarget->DrawText(
			tmpStr,
			wcslen(tmpStr),
			m_pTextFormat,
			key_area_rect,
			m_pFgBrush
		);
		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

HRESULT DemoApp::Draw_Text_At_Left_Bottom(std::string &s)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);
		D2D1_RECT_F layoutRect = D2D1::RectF(
			marginSize,
			rtSize.height - 60.0f,
			width/2-marginSize,
			rtSize.height
		);
		// Draw text
		CString tmpStr(s.c_str());
		m_pRenderTarget->DrawText(
			tmpStr,
			wcslen(tmpStr),
			m_pTextFormat,
			layoutRect,
			m_pFgBrush
		);
		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

HRESULT DemoApp::Draw_Text_At_Right_Bottom(std::string & s)
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);
		D2D1_RECT_F layoutRect = D2D1::RectF(
			width/2+marginSize,
			rtSize.height - 60.0f,
			width-marginSize,
			rtSize.height
		);
		// Draw text
		CString tmpStr(s.c_str());
		m_pRenderTarget->DrawText(
			tmpStr,
			wcslen(tmpStr),
			m_pTextFormat,
			layoutRect,
			m_pFgBrush
		);
		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

HRESULT DemoApp::Clear()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

HRESULT DemoApp::OnRender()
{
	HRESULT hr = S_OK;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
		
		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();
		// Draw a grid background.
		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);

		for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_pBgBrush,
				0.5f
			);
		}

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_pBgBrush,
				0.5f
			);
		}
		// Draw two rectangles.
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			rtSize.width / 2 - 50.0f,
			rtSize.height / 2 - 50.0f,
			rtSize.width / 2 + 50.0f,
			rtSize.height / 2 + 50.0f
		);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
			rtSize.width / 2 - 100.0f,
			rtSize.height / 2 - 100.0f,
			rtSize.width / 2 + 100.0f,
			rtSize.height / 2 + 100.0f
		);
		// Draw a filled rectangle.
		m_pRenderTarget->FillRectangle(&rectangle1, m_pBgBrush);
		// Draw the outline of a rectangle.
		m_pRenderTarget->DrawRectangle(&rectangle2, m_pFgBrush);
		// Draw text
		D2D1_RECT_F layoutRect = D2D1::RectF(1000.f, 1000.f, 200.f, 200.f);
		CString tmpStr(str.c_str());
		m_pRenderTarget->DrawText(
			tmpStr,
			wcslen(tmpStr),
			m_pTextFormat,
			rectangle1,
			m_pFgBrush
		);
		hr = m_pRenderTarget->EndDraw();
	}
	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}
	return hr;
}

void DemoApp::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		// Note: This method can fail, but it's okay to ignore the
		// error here, because the error will be returned again
		// the next time EndDraw is called.
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}