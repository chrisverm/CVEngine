#include "RenderResources.h"
#include "RenderManager.h"
#include <fstream>
#include <sstream>

#define XMFLOAT2_SIZE 8
#define XMFLOAT3_SIZE 12
#define XMFLOAT4_SIZE 16

namespace CVE
{
	namespace Graphics
	{
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

		bool loadMeshFromCVO( char filePath[], const i8 vertexFlags, D3D_PRIMITIVE_TOPOLOGY topology, MeshResource* const mesh )
		{
			const struct CVEFloat2 { f32 x, y; };
			const struct CVEFloat3 { f32 x, y, z; };
			const i32 VERTEX_SIZE = ( ( vertexFlags & VERTEX_POSITION ) >> 0 ) * sizeof( CVEFloat3 )
									+ ( ( vertexFlags & VERTEX_NORMAL ) >> 1 ) * sizeof( CVEFloat3 )
									+ ( ( vertexFlags & VERTEX_TEXCOORD ) >> 2 ) * sizeof( CVEFloat2 );
			const i8 TOPOLOGY_FACE_VERTEX_COUNT[] = { 0, 0, 0, 0, 3 }; // used for determining number of vertecies in a face according to the d3d11 topology

			std::string line = "";
			i16 cPositions = 0, cNormals = 0, cTexcoords = 0, cVertices = 0, cIndices = 0;
			CVEFloat3* tPositions = nullptr;
			CVEFloat3* tNormals = nullptr;
			CVEFloat2* tTexcoords = nullptr;
			byte* tVertices = nullptr;
			u32* tIndices = nullptr;

			std::ifstream file( filePath );
			if ( !file )
			{
				std::cerr << "Could not find a .cvo file with name/path: " << filePath;
				return false;
			}

			i16 i;
			size_t left = 0, right = 0;
			while ( !file.eof() )
			{
				std::getline( file, line );
				if ( line == "" || line[ 0 ] == '#' ) continue;
				line += '\0';

				if ( vertexFlags & VERTEX_POSITION && compare3( &line, "vp:" ) )
				{
					cPositions = (i16)atoi( &line[ 3 ] );
					tPositions = new CVEFloat3[ cPositions ];
					for ( i = 0; i < cPositions; i++ )
					{
						std::getline( file, line );
						CVE_ASSERT( compare3( &line, "vp " ) );
						left = right = 3;

						tPositions[ i ].x = (f32)atof( line.substr( left, ( right = line.find_first_of( " \0", left ) ) - left ).c_str() );
						left = right + 1;
						tPositions[ i ].y = (f32)atof( line.substr( left, ( right = line.find_first_of( " \0", left ) ) - left ).c_str() );
						left = right + 1;
						tPositions[ i ].z = (f32)atof( line.substr( left, line.find_first_of( " \0", left ) - left ).c_str() );
					}
				}
				else if ( vertexFlags & VERTEX_NORMAL && compare3( &line, "vn:" ) )
				{
					cNormals = (i16)atoi( &line[ 3 ] );
					tNormals = new CVEFloat3[ cNormals ];
					for ( i = 0; i < cNormals; i++ )
					{
						std::getline( file, line );
						CVE_ASSERT( compare3( &line, "vn " ) );
						left = right = 3;

						tNormals[ i ].x = (f32)atof( line.substr( left, ( right = line.find_first_of( " \0", left ) ) - left ).c_str() );
						left = right + 1;
						tNormals[ i ].y = (f32)atof( line.substr( left, ( right = line.find_first_of( " \0", left ) ) - left ).c_str() );
						left = right + 1;
						tNormals[ i ].z = (f32)atof( line.substr( left, line.find_first_of( " \0", left ) - left ).c_str() );
					}
				}
				else if ( vertexFlags & VERTEX_TEXCOORD && compare3( &line, "vt:" ) )
				{
					cTexcoords = (i16)atoi( &line[ 3 ] );
					tTexcoords = new CVEFloat2[ cTexcoords ];
					for ( i = 0; i < cTexcoords; i++ )
					{
						std::getline( file, line );
						CVE_ASSERT( compare3( &line, "vt " ) );
						left = right = 3;

						tTexcoords[ i ].x = (f32)atof( line.substr( left, ( right = line.find_first_of( " \0", left ) ) - left ).c_str() );
						left = right + 1;
						tTexcoords[ i ].y = (f32)atof( line.substr( left, line.find_first_of( " \0", left ) - left ).c_str() );
					}
				}
				else if ( compare2( &line, "v:" ) )
				{
					cVertices = (i16)atoi( &line[ 2 ] );
					tVertices = new byte[ cVertices * VERTEX_SIZE ];
					i32 offset = 0;
					for ( i = 0; i < cVertices; i++ )
					{
						std::getline( file, line );
						CVE_ASSERT( compare2( &line, "v " ) );
						left = right = 2;
						offset = 0;

						if ( vertexFlags & VERTEX_POSITION )
						{
							*(CVEFloat3*)( tVertices + i * VERTEX_SIZE + offset ) =
								tPositions[ atoi( line.substr( left, ( right = line.find_first_of( "/\0", left ) ) - left ).c_str() ) - 1 ];
							left = right + 1;
							offset += sizeof( CVEFloat3 );
						}
						if ( vertexFlags & VERTEX_NORMAL )
						{
							*(CVEFloat3*)( tVertices + i * VERTEX_SIZE + offset ) =
								tNormals[ atoi( line.substr( left, ( right = line.find_first_of( "/\0", left ) ) - left ).c_str() ) - 1 ];
							left = right + 1;
							offset += sizeof( CVEFloat3 );
						}
						if ( vertexFlags & VERTEX_TEXCOORD )
						{
							*(CVEFloat2*)( tVertices + i * VERTEX_SIZE + offset ) =
								tTexcoords[ atoi( line.substr( left, ( right = line.find_first_of( "/\0", left ) ) - left ).c_str() ) - 1 ];
						}
					}
				}
				else if ( compare2( &line, "f:" ) )
				{
					i8 fvCount = TOPOLOGY_FACE_VERTEX_COUNT[ topology ];

					cIndices = (i16)atoi( &line[ 2 ] );
					tIndices = new u32[ cIndices * fvCount ];
					int j = 0;
					for ( i = 0; i < cIndices; i++ )
					{
						std::getline( file, line );
						CVE_ASSERT( compare2( &line, "f " ) );
						left = right = 2;

						for ( j = 0; j < fvCount; j++ )
						{
							tIndices[ i * fvCount + j ] = atoi( line.substr( left, ( right = line.find_first_of( "/\0", left ) ) - left ).c_str() ) - 1;
							left = right + 1;
						}
					}
				}
			}

			if ( tPositions ) delete[] tPositions;
			if ( tNormals ) delete[] tNormals;
			if ( tTexcoords ) delete[] tTexcoords;

			mesh->Topology = topology;
			mesh->VertexStride = VERTEX_SIZE;
			mesh->VertexIndexCount = cIndices * TOPOLOGY_FACE_VERTEX_COUNT[ topology ];

			D3D11_BUFFER_DESC bufferDesc = { 0 };
			D3D11_SUBRESOURCE_DATA initialData { 0 };

			// create the vertex buffer
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.ByteWidth = cVertices * VERTEX_SIZE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			initialData.pSysMem = tVertices;
			RENDER_MGR.createBuffer( &bufferDesc, &initialData, &mesh->VertexBuffer );

			// create the index buffer
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.ByteWidth = mesh->VertexIndexCount * sizeof( u32 );
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			initialData.pSysMem = tIndices;
			RENDER_MGR.createBuffer( &bufferDesc, &initialData, &mesh->IndexBuffer );

			delete[] tVertices;
			delete[] tIndices;

			return true;
		}
		
		/*
		bool loadPNMeshFromCVOBJ( char filePath[], MeshResource* const mesh )
		{
			const struct CVEFloat3 { f32 x, y, z; };
			const struct CVEVertex { CVEFloat3 position, normal; };
			const i8 VERTEX_SIZE = XMFLOAT3_SIZE + XMFLOAT3_SIZE;

			std::string line = "";
			i16 cPositions = 0, cNormals = 0, cVertices = 0, cIndices = 0;
			CVEFloat3* tPositions = nullptr;
			CVEFloat3* tNormals = nullptr;
			CVEVertex* tVertices = nullptr;
			i16* tIndices = nullptr;

			try
			{
				std::ifstream file( filePath );
				file.exceptions( std::ifstream::failbit );
				i16 i = 0;
				size_t left = 0, right = 0;
				while ( !file.eof() )
				{
					std::getline( file, line );
					if ( line == "" || line[ 0 ] == '#' ) continue;

					if ( compare3( &line, "vp:" ) )
					{
						cPositions = (i16)atoi( line.erase( 0, 3 ).c_str() );
						tPositions = new CVEFloat3[ cPositions ];
						for ( i = 0; i < cPositions; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare3( &line, "vp " ) );
							left = right = 3;
							//line.erase( 0, 3 );
							
							tPositions[ i ].x = (f32)atof( line.substr( left, ( right = line.find_first_of( ' ' ) ) - left ).c_str() );
							left = right + 1;
							//line.erase( 0, temp + 1 );
							tPositions[ i ].y = (f32)atof( line.substr( left, ( right = line.find_first_of( ' ' ) ) - left ).c_str() );
							left = right + 1;
							//line.erase( 0, temp + 1 );
							tPositions[ i ].z = (f32)atof( line.substr( left, line.find_first_of( ' ' ) - left ).c_str() );
						}
					}
					else if ( compare3( &line, "vn:" ) )
					{
						cNormals = (i16)atoi( line.erase( 0, 3 ).c_str() );
						tNormals = new CVEFloat3[ cPositions ];
						for ( i = 0; i < cNormals; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare3( &line, "vn " ) );
							left = right = 3;
							//line.erase( 0, 3 );

							tNormals[ i ].x = (f32)atof( line.substr( left, ( right = line.find_first_of( ' ' ) ) - left ).c_str() );
							left = right + 1;
							//line.erase( 0, temp + 1 );
							tNormals[ i ].y = (f32)atof( line.substr( left, ( right = line.find_first_of( ' ' ) ) - left ).c_str() );
							left = right + 1;
							//line.erase( 0, temp + 1 );
							tNormals[ i ].z = (f32)atof( line.substr( left, line.find_first_of( ' ' ) - left ).c_str() );
						}
					}
					else if ( compare2( &line, "v:" ) )
					{
						cVertices = (i16)atoi( line.erase( 0, 2 ).c_str() );
						tVertices = new CVEVertex[ cVertices ];
						int pos, nor;
						for ( i = 0; i < cVertices; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare2( &line, "v " ) );
							left = right = 2;
							//line.erase( 0, 2 );

							pos = atoi( line.substr( left, line.find_first_of( '/' ) ).c_str() );
							nor = atoi( line.substr( line.find_last_of( '/' ) + 1, line.length() ).c_str() );
							tVertices[ i ] = { tPositions[ pos - 1 ], tNormals[ nor - 1 ] };

							int k = 0;
						}
					}
					else if ( compare2( &line, "f:" ) )
					{
						cIndices = (i16)atoi( line.erase( 0, 2 ).c_str() );
						tIndices = new i16[ cIndices * 3 ];
						for ( i = 0; i < cIndices; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare2( &line, "f " ) );
							left = right = 2;
							//line.erase( 0, 2 );

							tIndices[ i * 3 + 0 ] = atoi( line.substr( left, ( right = line.find_first_of( ' ' ) ) - left ).c_str() );
							left = right + 1;
							//line.erase( 0, temp + 1 );
							tIndices[ i * 3 + 1 ] = atoi( line.substr( left, ( right = line.find_first_of( ' ' ) ) - left ).c_str() );
							left = right + 1;
							//line.erase( 0, temp + 1 );
							tIndices[ i * 3 + 2 ] = atoi( line.substr( left, line.find_first_of( ' ' ) - left ).c_str() );
						}
					}
					else if ( !( compare3( &line, "vt:" ) || compare3( &line, "vt " ) ) )
					{
						throw std::ifstream::failbit;
					}
				}

				file.close();
			}
			catch ( std::ifstream::failure e )
			{
				std::cerr << "Could not find " << filePath;// << "\n\n" << e.what() << "\n\n" << e.code().message;

				return false;
			}

			int i = 0;

			delete[] tPositions;
			delete[] tNormals;

			mesh->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			mesh->VertexStride = VERTEX_SIZE;
			mesh->VertexIndexCount = cIndices;

			D3D11_BUFFER_DESC bufferDesc = { 0 };
			D3D11_SUBRESOURCE_DATA initialData { 0 };

			// create the vertex buffer
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.ByteWidth = cVertices * VERTEX_SIZE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			initialData.pSysMem = tVertices;
			RENDER_MGR.createBuffer( &bufferDesc, &initialData, &mesh->VertexBuffer );

			// create the index buffer
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.ByteWidth = cVertices * VERTEX_SIZE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			initialData.pSysMem = tVertices;
			RENDER_MGR.createBuffer( &bufferDesc, &initialData, &mesh->IndexBuffer );

			delete[] tVertices;
			delete[] tIndices;

			return true;
		}

		bool loadPNUMeshFromCVOBJ( char filePath[], MeshResource* const mesh )
		{
			const struct CVEFloat2 { f32 x, y; };
			const struct CVEFloat3 { f32 x, y, z; };
			const struct CVEVertex { CVEFloat3 position, normal; CVEFloat2 uv; };
			const i8 VERTEX_SIZE = XMFLOAT3_SIZE + XMFLOAT3_SIZE;

			std::string line = "";
			i16 cPositions = 0, cNormals = 0, cUVs = 0, cVertices = 0, cIndices = 0;
			CVEFloat3* tPositions = nullptr;
			CVEFloat3* tNormals = nullptr;
			CVEFloat2* tUVs = nullptr;
			CVEVertex* tVertices = nullptr;
			i16* tIndices = nullptr;

			try
			{
				std::ifstream file( filePath );
				file.exceptions( std::ifstream::failbit );
				i16 i = 0;
				size_t temp = 0;
				while ( !file.eof() )
				{
					std::getline( file, line );
					if ( line == "" || line[ 0 ] == '#' ) continue;

					if ( compare3( &line, "vp:" ) )
					{
						cPositions = (i16)atoi( line.erase( 0, 3 ).c_str() );
						tPositions = new CVEFloat3[ cPositions ];
						for ( i = 0; i < cPositions; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare3( &line, "vp " ) );
							line.erase( 0, 3 );

							tPositions[ i ].x = (f32)atof( line.substr( 0, temp = line.find_first_of( ' ' ) ).c_str() );
							line.erase( 0, temp + 1 );
							tPositions[ i ].y = (f32)atof( line.substr( 0, temp = line.find_first_of( ' ' ) ).c_str() );
							line.erase( 0, temp + 1 );
							tPositions[ i ].z = (f32)atof( line.substr( 0, line.find_first_of( ' ' ) ).c_str() );
						}
					}
					else if ( compare3( &line, "vn:" ) )
					{
						cNormals = (i16)atoi( line.erase( 0, 3 ).c_str() );
						tNormals = new CVEFloat3[ cPositions ];
						for ( i = 0; i < cNormals; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare3( &line, "vn " ) );
							line.erase( 0, 3 );

							tNormals[ i ].x = (f32)atof( line.substr( 0, temp = line.find_first_of( ' ' ) ).c_str() );
							line.erase( 0, temp + 1 );
							tNormals[ i ].y = (f32)atof( line.substr( 0, temp = line.find_first_of( ' ' ) ).c_str() );
							line.erase( 0, temp + 1 );
							tNormals[ i ].z = (f32)atof( line.substr( 0, line.find_first_of( ' ' ) ).c_str() );
						}
					}
					else if ( compare3( &line, "vt:" ) )
					{
						cUVs = (i16)atoi( line.erase( 0, 3 ).c_str() );
						tUVs = new CVEFloat2[ cUVs ];
						for ( i = 0; i < cUVs; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare3( &line, "vt " ) );
							line.erase( 0, 3 );

							tUVs[ i ].x = (f32)atof( line.substr( 0, temp = line.find_first_of( ' ' ) ).c_str() );
							line.erase( 0, temp + 1 );
							tUVs[ i ].y = (f32)atof( line.substr( 0, line.find_first_of( ' ' ) ).c_str() );
						}
					}
					else if ( compare2( &line, "v:" ) )
					{
						cVertices = (i16)atoi( line.erase( 0, 2 ).c_str() );
						tVertices = new CVEVertex[ cVertices ];
						int pos, nor, uv;
						for ( i = 0; i < cVertices; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare2( &line, "v " ) );
							line.erase( 0, 2 );

							pos = atoi( line.substr( 0, temp = line.find_first_of( '/' ) ).c_str() );
							line.erase( 0, temp + 1 );
							uv  = atoi( line.substr( 0, temp = line.find_first_of( '/' ) ).c_str() );
							line.erase( 0, temp + 1 );
							nor = atoi( line.substr( 0, line.find_first_of( '/' ) ).c_str() );
							tVertices[ i ] = { tPositions[ pos - 1 ], tNormals[ nor - 1 ], tUVs[ uv - 1 ] };
						}
					}
					else if ( compare2( &line, "f:" ) )
					{
						cIndices = (i16)atoi( line.erase( 0, 2 ).c_str() );
						tIndices = new i16[ cIndices * 3 ];
						for ( i = 0; i < cIndices; i++ )
						{
							std::getline( file, line );
							CVE_ASSERT( compare2( &line, "f " ) );
							line.erase( 0, 2 );

							tIndices[ i * 3 + 0 ] = atoi( line.substr( 0, temp = line.find_first_of( '/' ) ).c_str() );
							line.erase( 0, temp + 1 );
							tIndices[ i * 3 + 1 ] = atoi( line.substr( 0, temp = line.find_first_of( '/' ) ).c_str() );
							line.erase( 0, temp + 1 );
							tIndices[ i * 3 + 2 ] = atoi( line.substr( 0, line.find_first_of( '/' ) ).c_str() );
						}
					}
					else if ( !( compare3( &line, "vt:" ) || compare3( &line, "vt " ) ) )
					{
						throw std::ifstream::failbit;
					}
				}

				file.close();
			}
			catch ( std::ifstream::failure e )
			{
				std::cerr << "Could not find a .cvobj with name:" << filePath;// << "\n\n" << e.what() << "\n\n" << e.code().message;

				return false;
			}

			delete[] tPositions;
			delete[] tNormals;
			delete[] tUVs;

			mesh->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			mesh->VertexStride = VERTEX_SIZE;
			mesh->VertexIndexCount = cIndices;

			D3D11_BUFFER_DESC bufferDesc = { 0 };
			D3D11_SUBRESOURCE_DATA initialData { 0 };

			// create the vertex buffer
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.ByteWidth = cVertices * VERTEX_SIZE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			initialData.pSysMem = tVertices;
			RENDER_MGR.createBuffer( &bufferDesc, &initialData, &mesh->VertexBuffer );

			// create the index buffer
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.ByteWidth = cVertices * VERTEX_SIZE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			initialData.pSysMem = tVertices;
			RENDER_MGR.createBuffer( &bufferDesc, &initialData, &mesh->IndexBuffer );

			delete[] tVertices;
			delete[] tIndices;

			return true;
		}
		*/
	}
}