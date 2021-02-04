//------------------------------------------------------------------------------
//  nvx2streamreader.cc
//  (C) 2007 Radon Labs GmbH
//  (C) 2013-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "nvx2fileformatstructs.h"

#include <iostream>
#include <fstream>

namespace CoreGraphics
{

bool nvx2parser::SetupFromNvx2(const char* nvx2filepath)
{
    ifstream MeshFile(nvx2filepath, ifstream::binary);
		
    if( !MeshFile.is_open() ){
        printf("Can't open file!\n");
        getchar();
        return false;
    }

    MeshFile.seekg (0, MeshFile.end);
    int length = MeshFile.tellg();
    MeshFile.seekg (0, MeshFile.beg);

    cout << "File length: " << length << endl;

    char *theNvxBuffer = new char[length];
    char * NvxBuffer = theNvxBuffer;

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

    for (uint i = 0; i < nvxHeader->numGroups ; i++)
    {
        Nvx2Group* nvxGroup = (Nvx2Group*) NvxBuffer;
        this->ListOfGroups.push_back(*nvxGroup);
        //cout << "Size Of Group: " << sizeof(Nvx2Group) << endl << endl;
        NvxBuffer += sizeof(Nvx2Group);
    }

    //
    //

    for (int i = 0; i < N2NumVertexComponents; i++)
    {
        VertexComponent::SemanticName sem;
        VertexComponent::Format fmt;
        IndexT index = 0;
        if (nvxHeader->vertexComponentMask & (1<<i))
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
                    //n_error("Invalid Nebula2 VertexComponent in Nvx2StreamReader::SetupVertexComponents");
                    sem = VertexComponent::Position;
                    fmt = VertexComponent::Float3;
                    break;
            }

            this->vertexComponents.push_back(VertexComponent(sem, index, fmt));
        }
    }

    cout << "VC Size: " << this->vertexComponents.size() << endl << endl;



    for (uint i = 0; i < ListOfGroups[0].numVertices ; i++)
    {
        Nvx2Vertex* vertice = (Nvx2Vertex*) NvxBuffer;

        this->vertices.push_back(*vertice);
        NvxBuffer += sizeof(float) * nvxHeader->vertexWidth;
    }

    delete [] theNvxBuffer;

    int SizeOfList = vertices.size()*10;
    float *ListToBuffer = new float[SizeOfList];
    int SizeOfSecondList = vertices.size()*8;
    unsigned char *BoneDataListToBuffer = new unsigned char[SizeOfSecondList];
    int SizeOfThirdList = vertices.size()*12;
    char *NormalsListToBuffer = new char[SizeOfThirdList];

    int TriangleCounter = 3;
    Vector3D Edge1;
    Vector3D Edge2;
    Vector2D DeltaUV1;
    Vector2D DeltaUV2;
    float f;

    for (size_t i = 0; i < vertices.size(); i++)
    {
        if (TriangleCounter == 3)
        {
            Edge1 = Vector3D(vertices[i + 1].Coordinates[0] - vertices[i].Coordinates[0], vertices[i + 1].Coordinates[1] - vertices[i].Coordinates[1], vertices[i + 1].Coordinates[2] - vertices[i].Coordinates[2], 1.0);
            Edge2 = Vector3D(vertices[i + 2].Coordinates[0] - vertices[i].Coordinates[0], vertices[i + 2].Coordinates[1] - vertices[i].Coordinates[1], vertices[i + 2].Coordinates[2] - vertices[i].Coordinates[2], 1.0);

            DeltaUV1 = Vector2D(vertices[i + 1].UVs[0] - vertices[i].UVs[0] , vertices[i + 1].UVs[1] - vertices[i].UVs[1]);
            DeltaUV2 = Vector2D(vertices[i + 2].UVs[0] - vertices[i].UVs[0] , vertices[i + 2].UVs[1] - vertices[i].UVs[1]);

            f = 1.0f / (DeltaUV1.vektor[0] * DeltaUV2.vektor[1] -  DeltaUV2.vektor[0] * DeltaUV1.vektor[1]);

            TriangleCounter = 0;
        }

        int n = i*10;

        ListToBuffer[n] = vertices[i].Coordinates[0];
        ListToBuffer[n + 1] = vertices[i].Coordinates[1];
        ListToBuffer[n + 2] = vertices[i].Coordinates[2];
        ListToBuffer[n + 3] = 1.0;
        ListToBuffer[n + 4] = vertices[i].UVs[0];
        ListToBuffer[n + 5] = vertices[i].UVs[1];
        
        ListToBuffer[n + 6] = f * (DeltaUV2.vektor[1] * Edge1.vektor[0] - DeltaUV1.vektor[1] * Edge2.vektor[0]);
        ListToBuffer[n + 7] = f * (DeltaUV2.vektor[1] * Edge1.vektor[1] - DeltaUV1.vektor[1] * Edge2.vektor[1]);
        ListToBuffer[n + 8] = f * (DeltaUV2.vektor[1] * Edge1.vektor[2] - DeltaUV1.vektor[1] * Edge2.vektor[2]);
        ListToBuffer[n + 9] = 0.0;

        int m = i*8;

        BoneDataListToBuffer[m] = vertices[i].WeightsUB4N[0];
        BoneDataListToBuffer[m + 1] = vertices[i].WeightsUB4N[1];
        BoneDataListToBuffer[m + 2] = vertices[i].WeightsUB4N[2];
        BoneDataListToBuffer[m + 3] = vertices[i].WeightsUB4N[3];

        BoneDataListToBuffer[m + 4] = vertices[i].IndicesUB4[0];
        BoneDataListToBuffer[m + 5] = vertices[i].IndicesUB4[1];
        BoneDataListToBuffer[m + 6] = vertices[i].IndicesUB4[2];
        BoneDataListToBuffer[m + 7] = vertices[i].IndicesUB4[3];

        int k = i*12;

        NormalsListToBuffer[k] = vertices[i].NormalB4N[0];
        NormalsListToBuffer[k + 1] = vertices[i].NormalB4N[1];
        NormalsListToBuffer[k + 2] = vertices[i].NormalB4N[2];
        NormalsListToBuffer[k + 3] = 0.0;

        NormalsListToBuffer[k + 4] = vertices[i].TangentB4N[0];
        NormalsListToBuffer[k + 5] = vertices[i].TangentB4N[1];
        NormalsListToBuffer[k + 6] = vertices[i].TangentB4N[2];
        NormalsListToBuffer[k + 7] = 0.0;

        NormalsListToBuffer[k + 8] = vertices[i].BinormalB4N[0];
        NormalsListToBuffer[k + 9] = vertices[i].BinormalB4N[1];
        NormalsListToBuffer[k + 10] = vertices[i].BinormalB4N[2];
        NormalsListToBuffer[k + 11] = 0.0;

        TriangleCounter++;
    }

    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SizeOfList, ListToBuffer, GL_STATIC_DRAW);

    glGenBuffers(1, &BoneDataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, BoneDataBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned char) * SizeOfSecondList, BoneDataListToBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &NormalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, NormalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(char) * SizeOfThirdList, NormalsListToBuffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete [] ListToBuffer;
    delete [] BoneDataListToBuffer;
    delete [] NormalsListToBuffer;

    return true;
}

}