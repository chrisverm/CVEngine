#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Singleton.h"
#include <d3d11.h>

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

		private:
			const DXWindow* m_window;

			ID3D11Device*			m_device;
			ID3D11DeviceContext*	m_deviceContext;
			
			FrameParams*	m_currentFrame;

		};
	}
}

#endif