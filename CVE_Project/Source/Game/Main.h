#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <atomic>
#include <thread>
#include "DXWindow.h"
#include "Timer.h"

static CVE::Graphics::DXWindow* window;
static CVE::System::Timer* timer;

// multi-threading variables
static std::atomic_bool*	running;
static std::thread			tGameLogic;
static std::thread			tRenderLogic;
static std::thread			tGPULogic;

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd );
void Release( void );

void CalculateFrameStats( void );

void RunGameLogicThread( void );
void RunRenderLogicThread( void );
void RunGPUThread( void );

#endif