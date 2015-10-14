#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Singleton.h"
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

		private:
			FrameParams*	m_currentFrame;
		};
	}
}

#endif