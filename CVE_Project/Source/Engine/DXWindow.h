#ifndef DX_WINDOW_H
#define DX_WINDOW_H

#include <d3d11.h>
#include "Window.h"

//#include "RenderManager.h" // temporary, find work around

namespace CVE
{
	namespace Graphics
	{
		class DXWindow : public System::Window
		{
			friend class RenderManager; // temporary, find work around
		public:
			DXWindow( const System::WindowParams& params );
			~DXWindow( void );

			void resize( void );

		private:
			bool					m_enable4xMsaa;
			UINT					m_msaa4xQuality;

			ID3D11Device*			m_device;
			ID3D11DeviceContext*	m_deviceContext;
			IDXGISwapChain*			m_swapChain;
			ID3D11Texture2D*		m_depthStencilBuffer;
			ID3D11DepthStencilView* m_depthStencilView;
			ID3D11RenderTargetView* m_renderTargetView;
		};
	}
}

#endif