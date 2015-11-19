#include "DXWindow.h"
#include "FrameParams.h"

#include "RenderManager.h"
#include "FrameManager.h"
//#include <d3d11.h>
#include "CVEErrorCodes.h"
#include "CVEDataTypes.h"
#include <d3dcompiler.h>

namespace CVE
{
	namespace Graphics
	{
		void RenderManager::initialize( void )
		{
			UINT createDeviceFlags = 0;
#if defined(DEBUG) | defined(_DEBUG)
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_9_1;
			HRESULT hr = 0;
			HR( hr = D3D11CreateDevice(
					0,
					D3D_DRIVER_TYPE_HARDWARE,
					0,
					createDeviceFlags,
					0,
					0,
					D3D11_SDK_VERSION,
					&m_device,
					&featureLevel,
					&m_deviceContext )
			);
		}

		void RenderManager::release( void )
		{
			ReleaseMacro( m_device );
			ReleaseMacro( m_deviceContext );
		}

		void RenderManager::render( void )
		{
			if ( !FRAME_MGR.pop( RENDER, &m_currentFrame ) ) return;

			const f32 color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };

			m_deviceContext->ClearRenderTargetView(
				m_window->m_renderTargetView,
				color );
			m_deviceContext->ClearDepthStencilView(
				m_window->m_depthStencilView,
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
				1.0f,
				0 );

			m_currentFrame->Data += 10;

			HR( m_window->m_swapChain->Present( 0, 0 ) );

			FRAME_MGR.push( RENDER, &m_currentFrame );
		}

		void RenderManager::setWindow( DXWindow* const window )
		{
			CVE_ASSERT( window != nullptr );

			m_window = window;
			window->initialize( m_device );
		}
	}
}