#pragma once
//------------------------------------------------------------------------------
/**
    @file naxfileformatstructs.h
    
    nvx2 file format structures.
    
    (C) 2009 Radon Labs GmbH
    (C) 2013-2016 Individual contributors, see AUTHORS file
*/

#include "core/app.h"
//#include "core/types.h"
#include <vector>
#include "MyMathLibrary/Matrix3DHeader.h"
#include "MyMathLibrary/Vector3DHeader.h"
#include "MyMathLibrary/Vector2DHeader.h"
typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef unsigned char  ubyte;

typedef int IndexT;     // the index type
typedef int SizeT;      // the size type



namespace CoreGraphics
{
#pragma pack(push, 1)

#define NEBULA3_NVX2_MAGICNUMBER 'NVX2'

//------------------------------------------------------------------------------
/** 
    NVX2 file format structs.
    NOTE: keep all header-structs 4-byte aligned!
*/
struct Nvx2Header
{
	uint magic;
	uint numGroups;
	uint numVertices;
	uint vertexWidth;
	uint numIndices;
	uint numEdges;
	uint vertexComponentMask;
};

struct Nvx2Group
{
	uint firstVertex;
	uint numVertices;
	uint firstTriangle;
	uint numTriangles;
	uint firstEdge;
	uint numEdges;
};

struct Nvx2Vertex
{
	float Coordinates[3];
    unsigned char NormalB4N[4];
	float UVs[2];
	unsigned char TangentB4N[4];
	unsigned char BinormalB4N[4];
    unsigned char WeightsUB4N[4];
	unsigned char IndicesUB4[4];

};

class VertexComponent
{
public:
    /// component semantic
    enum SemanticName
    {
        Position = 0,
        Normal = 1,
        TexCoord1 = 2,        
        Tangent = 3,
        Binormal = 4,
        Color = 5,
        TexCoord2 = 6,
        SkinWeights = 7,
        SkinJIndices = 8,

        Invalid,
    };

    /// component format
    enum Format
    {
        Float,      //> one-component float, expanded to (float, 0, 0, 1)
        Float2,     //> two-component float, expanded to (float, float, 0, 1)
        Float3,     //> three-component float, expanded to (float, float, float, 1)
        Float4,     //> four-component float
        UByte4,     //> four-component unsigned byte
		Byte4,		//> four-component signed byte
        Short2,     //> two-component signed short, expanded to (value, value, 0, 1)
        Short4,     //> four-component signed short
        UByte4N,    //> four-component normalized unsigned byte (value / 255.0f)
		Byte4N,		//> four-component normalized signed byte (value / 127.0f)
        Short2N,    //> two-component normalized signed short (value / 32767.0f)
        Short4N,    //> four-component normalized signed short (value / 32767.0f)

        // PS3-specific
        Float16,
        Float16_2,
        Float16_3,
        Float16_4,
    };
    
    /// access type hint, this is only relevant on the Wii
    enum AccessType
    {
        None,
        Direct,     //> component has direct value (non-indexed)
        Index8,     //> component is indexed with 8-bit indices           
        Index16,    //> component is indexed with 16-bit indices
        Index32,    //> component is indexed with 32-bit indices
    };

	/// stride type tells if the compoent should be per-instance or per-vertex
	enum StrideType
	{
		PerVertex,
		PerInstance
	};

    /// default constructor
    VertexComponent();
    /// constructor
    VertexComponent(SemanticName semName, IndexT semIndex, Format format, IndexT streamIndex=0, StrideType strideType=PerVertex, SizeT stride=0);

	/*
	
	/// get semantic name
    SemanticName GetSemanticName() const;
    /// get semantic index
    IndexT GetSemanticIndex() const;
    /// get vertex component format
    Format GetFormat() const;
    /// get stream index
    IndexT GetStreamIndex() const;
    /// get the byte size of the vertex component
    SizeT GetByteSize() const;
    /// get a unique signature of the vertex component
    Util::String GetSignature() const;
    /// get access type
    AccessType GetAccessType() const;
	/// get stride type
	StrideType GetStrideType() const;
	/// get stride between instances
	SizeT GetStride() const;
    /// convert string to semantic name
    static SemanticName StringToSemanticName(const Util::String& str);
    /// convert semantic name to string
    static Util::String SemanticNameToString(SemanticName n);
    /// convert string to format
    static Format StringToFormat(const Util::String& str);
    /// convert format to string
    static Util::String FormatToString(Format f);
    /// get the byte offset of this component (only valid when part of a VertexLayout)
    IndexT GetByteOffset() const;

	*/

protected:
    //friend class VertexLayoutBase;

    /// set the vertex byte offset (called from VertexLayoutBase::Setup())
    void SetByteOffset(IndexT offset);

    SemanticName semName;
    IndexT semIndex;
    Format format;
    AccessType accessType;
    IndexT streamIndex;
    IndexT byteOffset;
    StrideType strideType;
    SizeT stride;
};

class nvx2parser
{
public:
    bool SetupFromNvx2(const char*);

    vector<Nvx2Group> ListOfGroups;
	GLuint VertexBuffer;
    GLuint BoneDataBuffer;
    GLuint NormalsBuffer;
    vector<Nvx2Vertex> vertices;

	enum N2VertexComponent
    {
        N2Coord        = (1<<0),      // 3 floats
        N2Normal       = (1<<1),      // 3 floats
        N2NormalB4N   = (1<<2),      // 4 unsigned bytes, normalized
        N2Uv0          = (1<<3),      // 2 floats
        N2Uv0S2        = (1<<4),      // 2 shorts, 4.12 fixed point
        N2Uv1          = (1<<5),      // 2 floats
        N2Uv1S2        = (1<<6),      // 2 shorts, 4.12 fixed point
        N2Uv2          = (1<<7),      // 2 floats
        N2Uv2S2        = (1<<8),      // 2 shorts, 4.12 fixed point
        N2Uv3          = (1<<9),      // 2 floats
        N2Uv3S2        = (1<<10),     // 2 shorts, 4.12 fixed point
        N2Color        = (1<<11),     // 4 floats
        N2ColorUB4N    = (1<<12),     // 4 unsigned bytes, normalized
        N2Tangent      = (1<<13),     // 3 floats
        N2TangentB4N  = (1<<14),     // 4 unsigned bytes, normalized
        N2Binormal     = (1<<15),     // 3 floats
        N2BinormalB4N = (1<<16),     // 4 unsigned bytes, normalized
        N2Weights      = (1<<17),     // 4 floats
        N2WeightsUB4N  = (1<<18),     // 4 unsigned bytes, normalized
        N2JIndices     = (1<<19),     // 4 floats
        N2JIndicesUB4  = (1<<20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1<<N2NumVertexComponents) - 1),
    };

	vector<CoreGraphics::VertexComponent> vertexComponents; 
};

inline VertexComponent::VertexComponent() :
    semName(Invalid),
    semIndex(0),
    format(Float),
    accessType(Index16),
    streamIndex(0),
    byteOffset(0),
	strideType(PerVertex),
	stride(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline VertexComponent::VertexComponent(SemanticName semName_, IndexT semIndex_, Format format_, IndexT streamIndex_, StrideType strideType_, SizeT stride_) :
    semName(semName_),
    semIndex(semIndex_),
    format(format_),
    accessType(Index16),
    streamIndex(streamIndex_),
    byteOffset(0),
	strideType(strideType_),
	stride(stride_)
{
    // empty
}

#pragma pack(pop)
} // namespace CoreGraphics  