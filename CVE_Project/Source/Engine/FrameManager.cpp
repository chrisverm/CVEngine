#include "FrameManager.h"

namespace CVE
{
	namespace Graphics
	{
		void FrameManager::initialize( void )
		{
			int i = 0;
			for ( ; i < STAGE_COUNT; i++ )
			{
				m_StageIndices[ i ] = 0;
			}
			for ( i = 0; i < MAX_FRAMES; i++ )
			{
				m_Frames[ i ].CurrentStage = GAME;
				m_Frames[ i ].Data = i;
			}
		}

		bool FrameManager::pop( FrameStage stage, FrameParams** out )
		{
			const uint8_t currentStageIndex = m_StageIndices[ stage ].load( std::memory_order_relaxed );
			if ( m_Frames[ currentStageIndex ].CurrentStage == stage )
			{
				( *out ) = &m_Frames[ currentStageIndex ];
				return true;
			}

			return false;
		}

		bool FrameManager::push( FrameStage stage, FrameParams** in )
		{
			const uint8_t currentStageIndex = m_StageIndices[ stage ].load( std::memory_order_relaxed );
			if ( in == nullptr || ( *in )->CurrentStage != stage || ( *in ) != &m_Frames[ currentStageIndex ] )
			{
				return false;
			}

			switch ( ( *in )->CurrentStage ) // is this thread safe? does it need to be?
			{
			case GAME:
				( *in )->CurrentStage = RENDER;
				break;
			case RENDER:
				( *in )->CurrentStage = GPU;
				break;
			case GPU:
				( *in )->CurrentStage = GAME;
				break;
			}

			( *in ) = nullptr;
			uint8_t nextStageIndex = currentStageIndex + 1;
			if ( nextStageIndex == MAX_FRAMES )
			{
				nextStageIndex = 0;
			}
			m_StageIndices[ stage ].store( nextStageIndex, std::memory_order_release );

			return true;
		}
	}
}