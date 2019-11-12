//------------------------------------------------------------------------------
//  nvx2streamreader.cc
//  (C) 2007 Radon Labs GmbH
//  (C) 2013-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
//#include "stdneb.h"
#include "nvx2streamreader.h"
#include "nvx2fileformatstructs.h"
//#include "coregraphics/memoryvertexbufferloader.h"
//#include "coregraphics/memoryindexbufferloader.h"

#include <iostream>
#include <fstream>

namespace CoreGraphics
{

bool nvx2parser::SetupFromNvx2(const char* nvx2filepath)
{
    ifstream MeshFile(nvx2filepath, ifstream::binary );
		
		if( !MeshFile.is_open() ){
			printf("Can't open file!\n");
			getchar();
			return false;
		}

		MeshFile.seekg (0, MeshFile.end);
		int length = MeshFile.tellg();
		MeshFile.seekg (0, MeshFile.beg);

		cout << "File length: " << length << endl;

		char theNvxBuffer[length];
		char * NvxBuffer = theNvxBuffer;

		//int BufferIndex = 0;

		MeshFile.read(NvxBuffer, length);

		//
		//

		Nvx2Header* nvxHeader = (Nvx2Header*) NvxBuffer;
    	NvxBuffer += sizeof(Nvx2Header);

        //
        //

        cout << "magic: " << nvxHeader->magic << endl;
        cout << "numGroups: " << nvxHeader->numGroups << endl;
        cout << "numVertices: " << nvxHeader->numVertices << endl;
        cout << "vertexWidth: " << nvxHeader->vertexWidth << endl;
        cout << "numIndices: " << nvxHeader->numIndices << endl;
        cout << "numEdges: " << nvxHeader->numEdges << endl;
        cout << "vertexComponentMask: " << nvxHeader->vertexComponentMask << endl << endl;

        //
        //

        for (int i = 0; i < nvxHeader->numGroups ; i++)
        {
            Nvx2Group* nvxGroup = (Nvx2Group*) NvxBuffer;
            this->ListOfGroups.push_back(nvxGroup);
            //cout << "Size Of Group: " << sizeof(Nvx2Group) << endl << endl;
            NvxBuffer += sizeof(Nvx2Group);
        }

        //
        //

        cout << "firstVertex: " << ListOfGroups[0]->firstVertex << endl;
        cout << "numVertices: " << ListOfGroups[0]->numVertices << endl;
        cout << "firstTriangle: " << ListOfGroups[0]->firstTriangle << endl;
        cout << "numTriangles: " << ListOfGroups[0]->numTriangles << endl;
        cout << "firstEdge: " << ListOfGroups[0]->firstEdge << endl;
        cout << "numEdges: " << ListOfGroups[0]->numEdges << endl << endl;

        //
        //




        for (int i = 0; i < N2NumVertexComponents; i++)
        {
            VertexComponent::SemanticName sem;
            VertexComponent::Format fmt;
            IndexT index = 0;
            if (nvxHeader->vertexComponentMask & (1<<i))
            {
                //cout << "Has Coordinates: " << i << endl;

                switch (1<<i)
                {
                    case N2Coord:        sem = VertexComponent::Position;     fmt = VertexComponent::Float3; break;
                    case N2Normal:       sem = VertexComponent::Normal;       fmt = VertexComponent::Float3; break;
                    case N2NormalB4N:    sem = VertexComponent::Normal;       fmt = VertexComponent::Byte4N; break;
                    case N2Uv0:          sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Float2; index = 0; break;
                    case N2Uv0S2:        sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Short2; index = 0; break;
                    case N2Uv1:          sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Float2; index = 1; break;
                    case N2Uv1S2:        sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Short2; index = 1; break;
                    /*
                    case N2Uv2:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 2; break;
                    case N2Uv2S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 2; break;
                    case N2Uv3:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 3; break;
                    case N2Uv3S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 3; break;
                    */
                    case N2Color:        sem = VertexComponent::Color;        fmt = VertexComponent::Float4; break;
                    case N2ColorUB4N:    sem = VertexComponent::Color;        fmt = VertexComponent::UByte4N; break;
                    case N2Tangent:      sem = VertexComponent::Tangent;      fmt = VertexComponent::Float3; break;
                    case N2TangentB4N:   sem = VertexComponent::Tangent;      fmt = VertexComponent::Byte4N; break;
                    case N2Binormal:     sem = VertexComponent::Binormal;     fmt = VertexComponent::Float3; break;
                    case N2BinormalB4N:  sem = VertexComponent::Binormal;     fmt = VertexComponent::Byte4N; break;
                    case N2Weights:      sem = VertexComponent::SkinWeights;  fmt = VertexComponent::Float4; break;
                    case N2WeightsUB4N:  sem = VertexComponent::SkinWeights;  fmt = VertexComponent::UByte4N; break;
                    case N2JIndices:     sem = VertexComponent::SkinJIndices; fmt = VertexComponent::Float4; break;
                    case N2JIndicesUB4:  sem = VertexComponent::SkinJIndices; fmt = VertexComponent::UByte4; break;
                    default:
                        //n_error("Invalid Nebula2 VertexComponent in Nvx2StreamReader::SetupVertexComponents");
                        sem = VertexComponent::Position;
                        fmt = VertexComponent::Float3;
                        break;
                }
                //cout << "sem: " << sem << endl << "fmt: " << fmt << endl;

                this->vertexComponents.push_back(VertexComponent(sem, index, fmt));
            }
        }

        cout << "VC Size: " << this->vertexComponents.size() << endl << endl;
        //cout << this->vertexComponents[7].semName << endl;

        //
        //



        for (int i = 0; i < ListOfGroups[0]->numVertices ; i++)
        {
            //Vector3D* vertice = (Vector3D*) NvxBuffer;
            Nvx2Vertex* vertice = (Nvx2Vertex*) NvxBuffer;
            //Vector3D* fisk = (Vector3D*) (NvxBuffer + sizeof(float));
            
            //cout << vertice->vektor[0] << " :: " << vertice->vektor[1] << " :: " << vertice->vektor[2] << " :: " << fisk->vektor[0] << " :: " << fisk->vektor[1] << " :: " << fisk->vektor[2] << endl;
            //cout << vertice->vektor[1] << " :: " << fisk->vektor[0] << endl;

            this->vertices.push_back(vertice);
            NvxBuffer += sizeof(float) * nvxHeader->vertexWidth;
            //NvxBuffer += sizeof(Nvx2Vertex);
        }



        for (int i = 1000; i < 1500; i++)
        {
           
            //cout << i <<" Weight: " << this->vertices[i]->WeightsUB4N << endl;
            //cout << i <<" Indices: " << this->vertices[i]->IndicesUB4 << endl << endl;

            //unsigned char theone = this->vertices[i]->WeightsUB4N[0] + this->vertices[i]->WeightsUB4N[1] + this->vertices[i]->WeightsUB4N[2] + this->vertices[i]->WeightsUB4N[3];
            float theone = this->vertices[i]->WeightsUB4N[0] + this->vertices[i]->WeightsUB4N[1] + this->vertices[i]->WeightsUB4N[2] + this->vertices[i]->WeightsUB4N[3];
            cout << i <<" Weight: " << +this->vertices[i]->WeightsUB4N[0] << " :: " << +this->vertices[i]->WeightsUB4N[1] << " :: " << +this->vertices[i]->WeightsUB4N[2] << " :: " << +this->vertices[i]->WeightsUB4N[3] << " ::::::: " << theone << endl;
            cout << i <<" Indices: " << +this->vertices[i]->IndicesUB4[0] << " :: " << +this->vertices[i]->IndicesUB4[1] << " :: " << +this->vertices[i]->IndicesUB4[2] << " :: " << +this->vertices[i]->IndicesUB4[3] << endl;

            cout << endl << endl;
        }

        cout << vertices.size() << endl;


        int SizeOfList = vertices.size()*7;
		float ListToBuffer[SizeOfList];
        int SizeOfSecondList = vertices.size()*8;
        unsigned char BoneDataListToBuffer[SizeOfSecondList];

        for (int i = 0; i < vertices.size(); i++)
		{
			int n = i*7;

            float WeightSize = this->vertices[i]->WeightsUB4N[0] + this->vertices[i]->WeightsUB4N[1] + this->vertices[i]->WeightsUB4N[2] + this->vertices[i]->WeightsUB4N[3];


			ListToBuffer[n] = vertices[i]->Coordinates[0];
			ListToBuffer[n + 1] = vertices[i]->Coordinates[1];
			ListToBuffer[n + 2] = vertices[i]->Coordinates[2];
            ListToBuffer[n + 3] = 1.0;
            ListToBuffer[n + 4] = vertices[i]->UVs[0];
            ListToBuffer[n + 5] = vertices[i]->UVs[1];
            //ListToBuffer[n + 6] = vertices[i]->WeightsUB4N;
            //ListToBuffer[n + 7] = vertices[i]->IndicesUB4;
            ListToBuffer[n + 6] = WeightSize;

            //cout << ListToBuffer[n] << " :: " << ListToBuffer[n + 1] << " :: " << ListToBuffer[n + 2] << " :: " << ListToBuffer[n + 3] << endl;

            //if(ListToBuffer[n] > 0.2)
            //    cout << ListToBuffer[n] << " :: " << ListToBuffer[n + 1] << " :: " << ListToBuffer[n + 2] << " :: " << ListToBuffer[n + 3] << endl;
            int m = i*8;

            BoneDataListToBuffer[m] = vertices[i]->WeightsUB4N[0];
            BoneDataListToBuffer[m + 1] = vertices[i]->WeightsUB4N[1];
            BoneDataListToBuffer[m + 2] = vertices[i]->WeightsUB4N[2];
            BoneDataListToBuffer[m + 3] = vertices[i]->WeightsUB4N[3];
            BoneDataListToBuffer[m + 4] = vertices[i]->IndicesUB4[0];
            BoneDataListToBuffer[m + 5] = vertices[i]->IndicesUB4[1];
            BoneDataListToBuffer[m + 6] = vertices[i]->IndicesUB4[2];
            BoneDataListToBuffer[m + 7] = vertices[i]->IndicesUB4[3];
		}


        glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ListToBuffer), ListToBuffer, GL_STATIC_DRAW);

        glGenBuffers(1, &BoneDataBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, BoneDataBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(BoneDataListToBuffer), BoneDataListToBuffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


    return true;
}

}





#if NEBULA3_LEGACY_SUPPORT
namespace Legacy
{
__ImplementClass(Legacy::Nvx2StreamReader, 'N2SR', IO::StreamReader);

using namespace CoreGraphics;
using namespace Util;
using namespace Math;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
Nvx2StreamReader::Nvx2StreamReader() :
    usage(Base::ResourceBase::UsageImmutable),
    access(Base::ResourceBase::AccessNone),
    rawMode(false),
    mapPtr(0),
    groupDataPtr(0),
    vertexDataPtr(0),
    indexDataPtr(0),
    groupDataSize(0),
    vertexDataSize(0),
    indexDataSize(0),
    numGroups(0),
    numVertices(0),
    vertexWidth(0),
    numIndices(0),
    numEdges(0),
    vertexComponentMask(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Nvx2StreamReader::~Nvx2StreamReader()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
Nvx2StreamReader::Open()
{
    n_assert(0 == this->primGroups.Size());
    n_assert(this->stream->CanBeMapped());
    n_assert(this->vertexComponents.IsEmpty());
    n_assert(0 == this->mapPtr);
    if (StreamReader::Open())
    {
        // map the stream to memory
        this->mapPtr = this->stream->Map();
        n_assert(0 != this->mapPtr);

        // read data
        this->ReadHeaderData();
        this->ReadPrimitiveGroups();
        this->SetupVertexComponents();
        if (!this->rawMode)
        {
            this->SetupVertexBuffer();
            this->SetupIndexBuffer();
            this->UpdateGroupBoundingBoxes();
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::Close()
{
    this->mapPtr = 0;
    this->groupDataPtr = 0;
    this->vertexDataPtr = 0;
    this->indexDataPtr = 0;
    this->vertexBuffer = 0;
    this->indexBuffer = 0;
    this->primGroups.Clear();
    this->vertexComponents.Clear();
    stream->Unmap();
    StreamReader::Close();
}

//------------------------------------------------------------------------------
/**
    This reads the nvx2 header data and checks whether the file is 
    actually an nvx2 file through the magic number. All header data
    will be read into member variables, and pointers to the
    start of the group-, vertex- and index-data will be setup.
    NOTE: we assume that the file is in the correct byte order!
*/
void
Nvx2StreamReader::ReadHeaderData()
{
    n_assert(0 != this->mapPtr);
    
    // endian-convert header
    struct Nvx2Header* header = (struct Nvx2Header*) this->mapPtr;
    header->numIndices *= 3; // header holds number of tris, not indices

    // check magic number
    if (FourCC(header->magic) != FourCC('NVX2'))
    {
        // not a nvx2 file, break hard
        n_error("Nvx2StreamReader: '%s' is not a nvx2 file!", this->stream->GetURI().AsString().AsCharPtr());
    }    
    this->numGroups = header->numGroups;
    this->numVertices = header->numVertices;
    this->vertexWidth = header->vertexWidth;
    this->numIndices = header->numIndices;
    this->numEdges = header->numEdges;
    this->vertexComponentMask = header->vertexComponentMask;
    this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
    this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
	this->indexDataSize = this->numIndices * sizeof(int);

    this->groupDataPtr = header + 1;
    this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
    this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;
}

//------------------------------------------------------------------------------
/**
	Question here, basevertex is supposed to be a vertex offset into the vertex buffer.
	However, the indices describe where to fetch the vertex data, so why would we need it
	if we are using static buffers?
*/
void
Nvx2StreamReader::ReadPrimitiveGroups()
{
    n_assert(this->primGroups.IsEmpty());
    n_assert(this->numGroups > 0);
    n_assert(0 != this->groupDataPtr);
    Nvx2Group* group = (Nvx2Group*) this->groupDataPtr;
    IndexT i;
    for (i = 0; i < (SizeT)this->numGroups; i++)
    {
        // setup a primitive group object
        PrimitiveGroup primGroup;
        //primGroup.SetBaseVertex(group->firstVertex);
        primGroup.SetNumVertices(group->numVertices);
        primGroup.SetBaseIndex(group->firstTriangle * 3);
        primGroup.SetNumIndices(group->numTriangles * 3);
		primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
        this->primGroups.Append(primGroup);

        // set top next group
        group++;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::SetupVertexComponents()
{
    n_assert(this->vertexComponents.IsEmpty());

    IndexT i;
    for (i = 0; i < N2NumVertexComponents; i++)
    {
        VertexComponent::SemanticName sem;
        VertexComponent::Format fmt;
        IndexT index = 0;
        if (vertexComponentMask & (1<<i))
        {
            switch (1<<i)
            {
                case N2Coord:        sem = VertexComponent::Position;     fmt = VertexComponent::Float3; break;
                case N2Normal:       sem = VertexComponent::Normal;       fmt = VertexComponent::Float3; break;
                case N2NormalB4N:    sem = VertexComponent::Normal;       fmt = VertexComponent::Byte4N; break;
                case N2Uv0:          sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Float2; index = 0; break;
                case N2Uv0S2:        sem = VertexComponent::TexCoord1;    fmt = VertexComponent::Short2; index = 0; break;
                case N2Uv1:          sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Float2; index = 1; break;
                case N2Uv1S2:        sem = VertexComponent::TexCoord2;    fmt = VertexComponent::Short2; index = 1; break;
                    /*
                case N2Uv2:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 2; break;
                case N2Uv2S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 2; break;
                case N2Uv3:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 3; break;
                case N2Uv3S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 3; break;
                */
                case N2Color:        sem = VertexComponent::Color;        fmt = VertexComponent::Float4; break;
                case N2ColorUB4N:    sem = VertexComponent::Color;        fmt = VertexComponent::UByte4N; break;
                case N2Tangent:      sem = VertexComponent::Tangent;      fmt = VertexComponent::Float3; break;
                case N2TangentB4N:   sem = VertexComponent::Tangent;      fmt = VertexComponent::Byte4N; break;
                case N2Binormal:     sem = VertexComponent::Binormal;     fmt = VertexComponent::Float3; break;
                case N2BinormalB4N:  sem = VertexComponent::Binormal;     fmt = VertexComponent::Byte4N; break;
                case N2Weights:      sem = VertexComponent::SkinWeights;  fmt = VertexComponent::Float4; break;
                case N2WeightsUB4N:  sem = VertexComponent::SkinWeights;  fmt = VertexComponent::UByte4N; break;
                case N2JIndices:     sem = VertexComponent::SkinJIndices; fmt = VertexComponent::Float4; break;
                case N2JIndicesUB4:  sem = VertexComponent::SkinJIndices; fmt = VertexComponent::UByte4; break;
                default:
                    n_error("Invalid Nebula2 VertexComponent in Nvx2StreamReader::SetupVertexComponents");
                    sem = VertexComponent::Position;
                    fmt = VertexComponent::Float3;
                    break;
            }
            this->vertexComponents.Append(VertexComponent(sem, index, fmt));
        }
    }
}

//------------------------------------------------------------------------------
/**
    Since nvx2 files don't contain any bounding box information
    we need to compute per-primitive-group bounding boxes
    manually by walking the triangle indices. This may be inefficient
    with large meshes.
*/
void
Nvx2StreamReader::UpdateGroupBoundingBoxes()
{
    n_assert(0 != this->vertexDataPtr);
    n_assert(0 != this->indexDataPtr);
    n_assert(this->primGroups.Size() > 0);

    float* vertexPtr = (float*) this->vertexDataPtr;
    ushort* indexPtr = (ushort*) this->indexDataPtr;
    IndexT groupIndex;
    for (groupIndex = 0; groupIndex < this->primGroups.Size(); groupIndex++)
    {
        PrimitiveGroup& group = this->primGroups[groupIndex];        
        bbox box;
        box.begin_extend();
        point p;
        IndexT ii;
        for (ii = 0; ii < group.GetNumIndices(); ii++)
        {
            float* curVertexPtr = vertexPtr + (indexPtr[ii + group.GetBaseIndex()] * this->vertexWidth);
            p.set(curVertexPtr[0], curVertexPtr[1], curVertexPtr[2]);
            box.extend(p);
        }
        group.SetBoundingBox(box);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::SetupVertexBuffer()
{
    n_assert(!this->rawMode);
    n_assert(0 != this->vertexDataPtr);
    n_assert(this->vertexDataSize > 0);
    n_assert(this->numVertices > 0);    
    n_assert(this->vertexComponents.Size() > 0);

    // setup new vertex buffer
    if (!this->vertexBuffer.isvalid())
    {
        this->vertexBuffer = VertexBuffer::Create();
    }
    if (!this->vertexBufferLoader.isvalid())
    {
        this->vertexBufferLoader = MemoryVertexBufferLoader::Create();
    }
    this->vertexBufferLoader->Setup(this->vertexComponents, this->numVertices, this->vertexDataPtr, 
                                    this->vertexDataSize, this->usage, this->access);
    this->vertexBuffer->SetLoader(this->vertexBufferLoader.upcast<ResourceLoader>());
    this->vertexBuffer->Load();
    this->vertexBuffer->SetLoader(0);
    this->vertexBufferLoader = 0;
    n_assert(this->vertexBuffer->GetState() == VertexBuffer::Loaded);
}

//------------------------------------------------------------------------------
/**
*/
void
Nvx2StreamReader::SetupIndexBuffer()
{
    n_assert(!this->rawMode);
    n_assert(0 != this->indexDataPtr);
    n_assert(this->indexDataSize > 0);
    n_assert(this->numIndices > 0);
    
    // setup a new index buffer
    if (!this->indexBuffer.isvalid())
    {
        this->indexBuffer = IndexBuffer::Create();
    }
    if (!this->indexBufferLoader.isvalid())
    {
        this->indexBufferLoader = MemoryIndexBufferLoader::Create();
    }

	this->indexBufferLoader->Setup(IndexType::Index32, this->numIndices, this->indexDataPtr, 
		this->indexDataSize, this->usage, this->access);

    
    this->indexBuffer->SetLoader(this->indexBufferLoader.upcast<ResourceLoader>());
    this->indexBuffer->Load();
    this->indexBuffer->SetLoader(0);
    this->indexBufferLoader = 0;
    n_assert(this->indexBuffer->GetState() == IndexBuffer::Loaded);
}

} // namespace Legacy

#endif // NEBULA3_LEGACY_SUPPORT