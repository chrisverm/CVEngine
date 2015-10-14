#include "Window.h"
#include "CVEErrorCodes.h"

namespace CVE
{
	namespace System
	{
		Window::Window( const WindowParams& params )
		{
			WNDCLASS window;
			// CS_HREDRAW: redraws entire window if window client width is changed
			// CS_VREDRAW: redraws entire window if window client height is changed
			window.style = CS_HREDRAW | CS_VREDRAW;
			window.lpfnWndProc = params.WndProcedure;
			window.cbClsExtra = 0; // number of extra bytes to allocate to the window-class structure
			window.cbWndExtra = 0; // number of extra bytes to allocate to the window instance
			window.hInstance = params.HInstance;
			window.hIcon = LoadIcon( 0, IDI_APPLICATION );
			window.hCursor = LoadCursor( 0, IDC_ARROW );
			window.hbrBackground = (HBRUSH)GetStockObject( NULL_BRUSH );
			window.lpszMenuName = 0;
			window.lpszClassName = params.Name;
			CVE_ASSERT( RegisterClass( &window ) );

			resize( params.Width, params.Height );
			RECT rect = { 0, 0, m_width, m_height };
			AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );

			m_handle = CreateWindowEx( 0, params.Name, L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
				CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 0, 0, params.HInstance, 0 );
			CVE_ASSERT( m_handle );

			ShowWindow( m_handle, SW_SHOW );
			UpdateWindow( m_handle );
		}

		void Window::resize( u16 width, u16 height )
		{
			m_width = width;
			m_height = height;
			m_aspectRatio = (f32)m_width / (f32)m_height;
		}

		const u16 Window::width( void ) const
		{
			return m_width;
		}

		const u16 Window::height( void ) const
		{
			return m_height;
		}

		const f32 Window::aspectRatio( void ) const
		{
			return m_aspectRatio;
		}

		const HWND* const Window::handle( void ) const
		{
			return &m_handle;
		}
	}
}