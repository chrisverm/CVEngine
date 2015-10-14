#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Singleton.h"

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


			void setWindow( const DXWindow* const window );

		private:
			const DXWindow* m_window;
			
			FrameParams*	m_currentFrame;

		};
	}
}

#endif