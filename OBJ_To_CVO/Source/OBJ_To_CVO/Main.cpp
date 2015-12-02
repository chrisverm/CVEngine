#include <iostream>
#include <fstream>
#include <sstream>

struct CVEFloat2 { float x, y; };
struct CVEFloat3 { float x, y, z;  };

bool loadOBJData( std::string filename );
bool createCVOFile( std::string filename );
bool compare2( const std::string* s, const char* c );
bool compare3( const std::string* s, const char* c );

int main( void )
{
	std::cout << "Welcome to the tool for converting OBJ files to the proprietary CVO file format." << std::endl;
	std::cout << "Your OBJ file to convert must be located in the same folder as this exe." << std::endl;
	std::cout << "Please enter an OBJ file name to convert to a CVO file: ";

	std::string filename;
	std::getline( std::cin, filename );
	bool b = loadOBJData( filename );
	if ( b ) createCVOFile( filename );

	system( "pause" );
	return 0;
}

/*
Compare 2 or 3 characters in a string and a character array
*/
inline bool compare2( const std::string* s, const char* c )
{
	return *(short*)c == *(short*)s->c_str();
}
inline bool compare3( const std::string* s, const char* c )
{
	const struct foobar { int val : 24; int junk : 8; };
	return ( *(foobar*)c ).val == ( *(foobar*)s->c_str() ).val;
}

bool loadOBJData( std::string filename )
{
	filename += ".obj";
	std::ifstream file( filename );
	if ( !file )
	{
		std::cerr << "*****ERROR*****" << std::endl;
		std::cerr << "Could not find '" << filename << '\'' << std::endl;
		std::cerr << "***************" << std::endl;

		return false;
	}

	return true;
}

bool createCVOFile( std::string filename )
{
	filename += ".cvo";
	std::ofstream file( filename );
	if ( !file )
	{
		std::cerr << "*****ERROR*****" << std::endl;
		std::cerr << "Could not create '" << filename << '\'' << std::endl;
		std::cerr << "***************" << std::endl;

		return false;
	}

	return true;
}