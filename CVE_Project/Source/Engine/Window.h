#ifndef WINDOW_H
#define WINDOW_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "CVEDataTypes.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

namespace CVE
{
	namespace System
	{
		struct WindowParams
		{
			HINSTANCE	HInstance;
			u16			Width;
			u16			Height;
			LPCWSTR		Name;
			WNDPROC		WndProcedure;
		};

		class Window
		{
		public:
			const u16 width( void ) const;
			const u16 height( void ) const;
			const f32 aspectRatio( void ) const;
			const HWND* const handle( void ) const;

		protected:
			HWND	m_handle;
			u16		m_width;
			u16		m_height;
			f32		m_aspectRatio;

			Window( const WindowParams& params );
			virtual void resize( u16 width, u16 height );
		};
	}
}

#endif