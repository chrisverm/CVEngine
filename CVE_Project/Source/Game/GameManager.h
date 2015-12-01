#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Singleton.h"
#include "FrameParams.h"
#include "RenderResources.h"

#define GAME_MGR GameManager::Instance()

class GameManager
{
	SINGLETON_INSTANCE( GameManager );
public:
	void initialize( void );
	void release( void );
	void update( void );

private:
	CVE::Graphics::FrameParams*	m_currentFrame;
	CVE::Graphics::MeshResource m_mesh;
};

#endif