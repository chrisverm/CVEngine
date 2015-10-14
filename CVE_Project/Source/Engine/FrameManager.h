#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <atomic>
#include "Singleton.h"

#define FRAME_MGR CVE::Graphics::FrameManager::Instance()

namespace CVE
{
	namespace Graphics
	{
		enum FrameStage : uint8_t
		{
			GAME = 0,
			RENDER = 1,
			GPU = 2
		};

		struct Frame
		{
			double DeltaTime;
			double StartTime, EndTime;

			FrameStage CurrentStage;
			int Data;
		};

		class FrameManager
		{
			SINGLETON_INSTANCE( FrameManager );
		public:
			static const uint8_t MAX_FRAMES = 10;
			static const uint8_t STAGE_COUNT = 3;

			void initialize( void );
			bool pop( FrameStage stage, Frame** out );
			bool push( FrameStage stage, Frame** in );

		private:
			Frame				m_Frames[ MAX_FRAMES ];
			std::atomic_uint8_t m_StageIndices[ STAGE_COUNT ];

		};
	}
}

#endif