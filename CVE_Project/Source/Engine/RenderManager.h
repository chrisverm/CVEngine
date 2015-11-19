#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <d3d11.h>
#include "Singleton.h"
#include "DXWindow.h"
#include "FrameParams.h"

#define RENDER_MGR CVE::Graphics::RenderManager::Instance()

namespace CVE
{
	namespace Graphics
	{
		class RenderManager
		{
			SINGLETON_INSTANCE( RenderManager );
		public:
			void initialize( void );
			void release( void );
			void render( void );

			void setWindow( DXWindow* const window );
			void createBuffer( const D3D11_BUFFER_DESC* const desc, const D3D11_SUBRESOURCE_DATA* const initialData, ID3D11Buffer** output );

		private:
			const DXWindow* m_window;

			ID3D11Device*			m_device;
			ID3D11DeviceContext*	m_deviceContext;
			
			FrameParams*	m_currentFrame;

		};
	}
}

#endif