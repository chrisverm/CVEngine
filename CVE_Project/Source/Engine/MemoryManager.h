#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "Singleton.h"

#define MEMORY_MGR CVE::System::MemoryManager::Instance()

namespace CVE
{
	namespace System
	{
		//extern const int MEM_POOL_SIZE = 65536;

		class MemoryManager
		{
			SINGLETON_INSTANCE( MemoryManager );

			static const int MEM_POOL_SIZE = 65536; // 64kb

		public:
			void	initialize( void );
			void	release( void );

			void*	allocate( size_t size );
			void	free( void* ptr );

		private:
			void*	m_memory;

		};
	}
}

#endif