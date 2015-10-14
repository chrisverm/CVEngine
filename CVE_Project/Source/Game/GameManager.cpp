#include "GameManager.h"
#include "FrameManager.h"

void GameManager::initialize( void )
{

}

void GameManager::release( void )
{

}

void GameManager::update( void )
{
	if ( !FRAME_MGR.pop( CVE::Graphics::GAME, &m_currentFrame ) ) return;

	m_currentFrame->Data += 5;

	FRAME_MGR.push( CVE::Graphics::GAME, &m_currentFrame );
}