#include "GameManager.h"
#include "FrameManager.h"

using namespace CVE;

void GameManager::initialize( void )
{
	CVE_ASSERT( Graphics::loadMeshFromCVO( 
		"crate.cvo", 
		Graphics::VERTEX_POSITION | Graphics::VERTEX_NORMAL | Graphics::VERTEX_TEXCOORD,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
		&m_mesh ) 
		);
}

void GameManager::release( void )
{
	m_mesh.Release();
}

void GameManager::update( void )
{
	if ( !FRAME_MGR.pop( CVE::Graphics::GAME, &m_currentFrame ) ) return;

	m_currentFrame->Data += 5;

	FRAME_MGR.push( CVE::Graphics::GAME, &m_currentFrame );
}