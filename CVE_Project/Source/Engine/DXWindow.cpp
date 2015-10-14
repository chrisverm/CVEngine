#include "DXWindow.h"
#include "CVEErrorCodes.h"

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

namespace CVE
{
	namespace Graphics
	{
		DXWindow::DXWindow( const System::WindowParams& params )
			: System::Window( params )
		{
			m_Enable4xMsaa = false;
			m_Msaa4xQuality = 0;

			DXGI_SWAP_CHAIN_DESC swapChainDesc;
			swapChainDesc.BufferDesc.Width = m_width;
			swapChainDesc.BufferDesc.Height = m_height;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.OutputWindow = m_handle;
			swapChainDesc.Windowed = true;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags = 0;
			if ( m_Enable4xMsaa )
			{
				swapChainDesc.SampleDesc.Count = 4;
				swapChainDesc.SampleDesc.Quality = m_Msaa4xQuality - 1;
			}
			else
			{
				swapChainDesc.SampleDesc.Count = 1;
				swapChainDesc.SampleDesc.Quality = 0;
			}

			UINT createDeviceFlags = 0;
#if defined(DEBUG) | defined(_DEBUG)
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_9_1;
			HRESULT hr = 0;
			hr = D3D11CreateDeviceAndSwapChain(
				0,
				driverType,
				0,
				createDeviceFlags,
				0,
				0,
				D3D11_SDK_VERSION,
				&swapChainDesc,
				&m_SwapChain,
				&m_Device,
				&featureLevel,
				&m_DeviceContext
				);
			if ( FAILED( hr ) )
			{
				MessageBox( 0, L"D3D11CreateDevice Failed", 0, 0 );
				return;
			}

			hr = m_Device->CheckMultisampleQualityLevels(
				DXGI_FORMAT_R8G8B8A8_UNORM,
				4,
				&m_Msaa4xQuality
				);
			CVE_ASSERT( m_Msaa4xQuality > 0 );

			resize();
		}

		DXWindow::~DXWindow( void )
		{
			ReleaseMacro( m_Device );
			ReleaseMacro( m_DeviceContext );
			ReleaseMacro( m_SwapChain );
			ReleaseMacro( m_RenderTargetView );
			ReleaseMacro( m_DepthStencilView );
			ReleaseMacro( m_DepthStencilBuffer );
		}

		void DXWindow::resize( void )
		{
			ReleaseMacro( m_RenderTargetView );
			ReleaseMacro( m_DepthStencilView );
			ReleaseMacro( m_DepthStencilBuffer );

			m_SwapChain->ResizeBuffers(
				1,
				m_width,
				m_height,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				0
				);
			ID3D11Texture2D* backBuffer;
			HR( m_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &backBuffer ) ) );
			HR( m_Device->CreateRenderTargetView( backBuffer, 0, &m_RenderTargetView ) );
			ReleaseMacro( backBuffer );

			D3D11_TEXTURE2D_DESC depthStencilDesc;
			depthStencilDesc.Width = m_width;
			depthStencilDesc.Height = m_height;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.CPUAccessFlags = 0;
			depthStencilDesc.MiscFlags = 0;
			if ( m_Enable4xMsaa )
			{
				depthStencilDesc.SampleDesc.Count = 4;
				depthStencilDesc.SampleDesc.Quality = m_Msaa4xQuality - 1;
			}
			else
			{
				depthStencilDesc.SampleDesc.Count = 1;
				depthStencilDesc.SampleDesc.Quality = 0;
			}

			HR( m_Device->CreateTexture2D( &depthStencilDesc, 0, &m_DepthStencilBuffer ) );
			HR( m_Device->CreateDepthStencilView( m_DepthStencilBuffer, 0, &m_DepthStencilView ) );

			m_DeviceContext->OMSetRenderTargets( 1, &m_RenderTargetView, m_DepthStencilView );

			Window::resize( m_width, m_height );
		}
	}
}