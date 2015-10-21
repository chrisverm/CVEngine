#ifndef RENDER_RESOURCES_H
#define RENDER_RESOURCES_H

#include <d3d11.h>
#include "CVEDataTypes.h"
#include "CVEErrorCodes.h"
#include <fstream>
#include <sstream>

namespace CVE
{
	namespace Graphics
	{
		bool cveLoadMeshFromOBJ( char filePath[] )
		{
			std::string data = "", line = "";
			int cPositions = 0, cNormals = 0, cUVs = 0, cFaces = 0;
			try
			{
				std::ifstream file( filePath );
				file.exceptions( std::ifstream::failbit );

				while ( !file.eof() )
				{
					std::getline( file, line );
					data += line + '\n';
					if ( line == "" ) continue;

					// count types of vertex and polygonal data
					cPositions	+= !( line[ 0 ] ^ 'v' ) * !( line[ 1 ] ^ ' ' );
					cNormals	+= !( line[ 0 ] ^ 'v' ) * !( line[ 1 ] ^ 'n' );
					cUVs		+= !( line[ 0 ] ^ 'v' ) * !( line[ 1 ] ^ 't' );
					cFaces		+= !( line[ 0 ] ^ 'f' ) * !( line[ 1 ] ^ ' ' );
				}

				file.close();
			}
			catch ( std::ifstream::failure e )
			{
				std::cerr << "Could not find " << filePath;// << "\n\n" << e.what() << "\n\n" << e.code().message;

				return false;
			}

			// remove all comments from file before reading data
			i32 comment, numComments = (i32)std::count( data.begin(), data.end(), '#' );
			while ( numComments > 0 )
			{
				comment = (i32)data.find_first_of( '#' );

				while ( comment != 0 && data[ comment - 1 ] == ' ' )
					comment--;

				while ( data[ comment ] != '\n' )
					data.erase( comment, 1 );

				numComments--;
			}

			return false;
		}

		struct MeshResource
		{
			D3D_PRIMITIVE_TOPOLOGY	Topology;
			u32						VertexStride;
			u32						VertexIndexCount;
			ID3D11Buffer*			VertexBuffer;
			ID3D11Buffer*			IndexBuffer;

			void Release( void )
			{
				ReleaseMacro( VertexBuffer );
				ReleaseMacro( IndexBuffer );
			}
		};

		struct MaterialResource
		{
			ID3D11ShaderResourceView*	ShaderResourceView;
			ID3D11SamplerState*			SamplerState;

			void Release( void )
			{
				ReleaseMacro( ShaderResourceView );
				ReleaseMacro( SamplerState );
			}
		};

		struct InstancingResource
		{
			u32				Count;
			u32				Stride;
			ID3D11Buffer*	Buffer;

			void Release( void )
			{
				ReleaseMacro( Buffer );
			}
		};

		struct ShaderBuffers
		{
			MeshResource		Mesh;
			MaterialResource	Material;
			InstancingResource	Instances;
		};
	}
}

#endif