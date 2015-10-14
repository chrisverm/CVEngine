#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <atomic>
#include "Singleton.h"
#include "FrameParams.h"

#define FRAME_MGR CVE::Graphics::FrameManager::Instance()

namespace CVE
{
	namespace Graphics
	{
		class FrameManager
		{
			SINGLETON_INSTANCE( FrameManager );
		public:
			static const uint8_t MAX_FRAMES = 10;
			static const uint8_t STAGE_COUNT = 3;

			void initialize( void );
			bool pop( FrameStage stage, FrameParams** out );
			bool push( FrameStage stage, FrameParams** in );

		private:
			FrameParams			m_Frames[ MAX_FRAMES ];
			std::atomic_uint8_t m_StageIndices[ STAGE_COUNT ];

		};
	}
}

#endif