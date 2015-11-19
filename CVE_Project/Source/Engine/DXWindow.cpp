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
			m_enable4xMSAA = false;
			m_msaa4xQuality = 0;

			m_swapChainDesc.BufferDesc.Width = m_width;
			m_swapChainDesc.BufferDesc.Height = m_height;
			m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			m_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			m_swapChainDesc.BufferCount = 1;
			m_swapChainDesc.OutputWindow = m_handle;
			m_swapChainDesc.Windowed = true;
			m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			m_swapChainDesc.Flags = 0;
			if ( m_enable4xMSAA )
			{
				m_swapChainDesc.SampleDesc.Count = 4;
				m_swapChainDesc.SampleDesc.Quality = m_msaa4xQuality - 1;
			}
			else
			{
				m_swapChainDesc.SampleDesc.Count = 1;
				m_swapChainDesc.SampleDesc.Quality = 0;
			}
		}

		DXWindow::~DXWindow( void )
		{
			ReleaseMacro( m_swapChain );
			ReleaseMacro( m_renderTargetView );
			ReleaseMacro( m_depthStencilView );
			ReleaseMacro( m_depthStencilBuffer );
		}

		void DXWindow::initialize( ID3D11Device* const device )
		{
			IDXGIDevice* dxgiDevice = nullptr;
			device->QueryInterface( __uuidof( IDXGIDevice ), (void**)&dxgiDevice );

			IDXGIAdapter* dxgiAdapter = nullptr;
			dxgiDevice->GetParent( __uuidof( IDXGIAdapter ), (void**)&dxgiAdapter );
			ReleaseMacro( dxgiDevice );

			IDXGIFactory* dxgiFactory = nullptr;
			dxgiAdapter->GetParent( __uuidof( IDXGIFactory ), (void**)&dxgiFactory );
			ReleaseMacro( dxgiAdapter );

			dxgiFactory->CreateSwapChain( device, &m_swapChainDesc, &m_swapChain );
			ReleaseMacro( dxgiFactory );

			resize( m_width, m_height );
		}

		void DXWindow::resize( u16 width, u16 height )
		{
			Window::resize( width, height );

			ReleaseMacro( m_renderTargetView );
			ReleaseMacro( m_depthStencilBuffer );
			ReleaseMacro( m_depthStencilView );

			// temp device and context
			ID3D11Device* t_device;
			ID3D11DeviceContext* t_deviceContext;
			m_swapChain->GetDevice( _uuidof( ID3D11Device ), (void**)&t_device );
			t_device->GetImmediateContext( &t_deviceContext );

			m_swapChainDesc.BufferDesc.Width = m_width;
			m_swapChainDesc.BufferDesc.Height = m_height;
			m_swapChain->ResizeBuffers(
				1,
				m_width,
				m_height,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				0
				);
			ID3D11Texture2D* backBuffer;
			HR( m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &backBuffer ) ) );
			HR( t_device->CreateRenderTargetView( backBuffer, 0, &m_renderTargetView ) );
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
			if ( m_enable4xMSAA )
			{
				depthStencilDesc.SampleDesc.Count = 4;
				depthStencilDesc.SampleDesc.Quality = m_msaa4xQuality - 1;
			}
			else
			{
				depthStencilDesc.SampleDesc.Count = 1;
				depthStencilDesc.SampleDesc.Quality = 0;
			}

			HR( t_device->CreateTexture2D( &depthStencilDesc, 0, &m_depthStencilBuffer ) );
			HR( t_device->CreateDepthStencilView( m_depthStencilBuffer, 0, &m_depthStencilView ) );
			t_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView );

			ReleaseMacro( t_device );
			ReleaseMacro( t_deviceContext );
		}
	}
}