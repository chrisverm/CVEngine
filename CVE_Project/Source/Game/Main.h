#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <atomic>
#include <thread>
#include "DXWindow.h"

static CVE::Graphics::DXWindow* window;

// multi-threading variables
static std::atomic_bool*	running;
static std::thread			tGameLogic;
static std::thread			tRenderLogic;
static std::thread			tGPULogic;

int WINAPI WinMain( HINSTANCE appInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd );
void Release( void );

void RunGameLogicThread( void );
void RunRenderLogicThread( void );
void RunGPUThread( void );

#endif