#ifndef SINGLETON_H
#define SINGLETON_H

namespace CVE
{
	namespace Utilities
	{

#define SINGLETON_INSTANCE( s )			\
public:									\
	static s& Instance( void )			\
	{									\
		static s instance;				\
		return instance;				\
	}									\
private:								\
	s( void ) { }						\
	s( const s& ) { }					\
	void operator=( const s& ) { }
	

#define SINGLETON_INIT( s )	s.initialize();
#define SINGLETON_RELEASE( s ) s.release();

	}
}

#endif