#include "RenderManager.h"
#include "FrameManager.h"

namespace CVE
{
	namespace Graphics
	{
		void RenderManager::initialize( void )
		{

		}

		void RenderManager::release( void )
		{

		}

		void RenderManager::render( void )
		{
			if ( !FRAME_MGR.pop( RENDER, &m_currentFrame ) ) return;

			m_currentFrame->Data += 10;

			FRAME_MGR.push( RENDER, &m_currentFrame );
		}
	}
}