#ifndef ERROR_CODES_H
#define ERROR_CODES_H

#if defined( DEBUG ) | defined( _DEBUG )																
#include <iostream>															
#define CVE_ASSERT( condition )												\
{																			\
	if ( !( condition ) )													\
	{																		\
		std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__;	\
		std::cerr << " inside " << __FUNCTION__ << std::endl;				\
		std::cerr << "Condition: " << #condition;							\
		abort();															\
	}																		\
}
#else
#define CVE_ASSERT( condition ) condition
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			/*DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);*/	\
			PostQuitMessage(0);									\
		}														\
	}														
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

enum CVEErrorCodes
{
	CVE_OK,
	CVE_FAIL
};

#endif