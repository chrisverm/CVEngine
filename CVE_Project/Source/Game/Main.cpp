#include "Main.h"
#include "MemoryManager.h"
#include "FrameManager.h"
#include "GameManager.h"
#include "RenderManager.h"

#include "RenderResources.h"
#include <sstream>

using namespace CVE;

LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT( 0, MNC_CLOSE );
	case WM_GETMINMAXINFO:
		( (MINMAXINFO*)lParam )->ptMinTrackSize.x = 200;
		( (MINMAXINFO*)lParam )->ptMinTrackSize.y = 200;
		return 0;
	default:
		return DefWindowProc( hwnd, msg, wParam, lParam );
	}
}
static WNDPROC MainWndProc = [] ( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) { return MsgProc( hwnd, msg, wParam, lParam ); };

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	//srand( time( NULL ) );

	// manager/singleton initialization
	SINGLETON_INIT( MEMORY_MGR );
	SINGLETON_INIT( FRAME_MGR );

	Graphics::loadPNUMeshFromOBJ( "crate_obj.obj" );

	System::WindowParams windowParams;
	windowParams.HInstance = appInstance;
	windowParams.Width = 800;
	windowParams.Height = 600;
	windowParams.Name = L"ASTEROIDS";
	windowParams.WndProcedure = MainWndProc;
	window = new Graphics::DXWindow( windowParams );
	RENDER_MGR.setWindow( window );

	timer = new System::Timer();

	// multi-threading variable initialization
	running = new std::atomic_bool();
	running->store( true );
	tGameLogic = std::thread( RunGameLogicThread );
	tRenderLogic = std::thread( RunRenderLogicThread );
	tGPULogic = std::thread( RunGPUThread );

	timer->reset();

	MSG msg = { 0 };
	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			timer->tick();

			CalculateFrameStats();
		}
	}

	Release();

	_CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}

void Release( void )
{
	running->store( false );
	tGameLogic.join();
	tRenderLogic.join();
	tGPULogic.join();

	delete running;

	delete timer;
	delete window;
}

void CalculateFrameStats( void )
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ( ( timer->totalTime() - timeElapsed ) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision( 6 );
		outs << "Window" << L"  |  "
			<< L"Width: " << window->width() << L"    "
			<< L"Height: " << window->height() << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";

		SetWindowText( *window->handle(), outs.str().c_str() );

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void RunGameLogicThread( void )
{
	while ( running->load() )
	{
		GAME_MGR.update();
	}
}

void RunRenderLogicThread( void )
{
	while ( running->load() )
	{
		RENDER_MGR.render();
	}
}

void RunGPUThread( void )
{
	while ( running->load() )
	{

	}
}