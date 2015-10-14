#include "MemoryManager.h"

namespace CVE
{
	namespace System
	{
		void MemoryManager::initialize( void )
		{

		}

		void MemoryManager::release( void )
		{
			
		}

		void* MemoryManager::allocate( size_t size )
		{
			return new char[ size ];
		}

		void MemoryManager::free( void* ptr )
		{
			delete ptr;
		}
	}
}