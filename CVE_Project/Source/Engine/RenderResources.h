#ifndef RENDER_RESOURCES_H
#define RENDER_RESOURCES_H

#include <d3d11.h>
#include "CVEDataTypes.h"
#include "CVEErrorCodes.h"

namespace CVE
{
	namespace Graphics
	{
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

		bool loadPNMeshFromCVOBJ( char filePath[], MeshResource* const mesh );
		bool loadPNUMeshFromCVOBJ( char filePath[], MeshResource* const mesh );
	}
}

#endif