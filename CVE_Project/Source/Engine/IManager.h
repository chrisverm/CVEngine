#ifndef I_MANAGER_H
#define I_MANAGER_H

#include "Singleton.h"

namespace CVE
{
	namespace Utilities
	{
		class IManager
		{
			virtual void initialize( void ) = 0;
			virtual void release( void ) = 0;
		};
	}
}

#endif