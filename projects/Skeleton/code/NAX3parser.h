#pragma once

#include <vector>

#include "MyMathLibrary/Matrix3DHeader.h"
#include "MyMathLibrary/Vector3DHeader.h"
#include "MyMathLibrary/Vector2DHeader.h"


typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef unsigned char  ubyte;


namespace CoreAnimation
{


//STRUCTS


struct Nax3Header
{
    uint magic;
    uint numClips;
    uint numKeys;
};

struct Nax3Clip
{
    ushort numCurves;
    ushort startKeyIndex;
    ushort numKeys;
    ushort keyStride;
    ushort keyDuration; 
    uchar preInfinityType;          // CoreAnimation::InfinityType::Code
    uchar postInfinityType;         // CoreAnimation::InfinityType::Code
    ushort numEvents;
    char name[50];                  // add up to 64 bytes size for Nax3Clip
};

struct Nax3AnimEvent
{
    char name[47];
    char category[15];
    ushort keyIndex;
};

struct Nax3Curve
{
    uint firstKeyIndex;
    uchar isActive;                 // 0 or 1
    uchar isStatic;                 // 0 or 1
    uchar curveType;                // CoreAnimation::CurveType::Code
    uchar _padding;                 // padding byte
    float staticKeyX;
    float staticKeyY;
    float staticKeyZ;
    float staticKeyW;
};

class NAX3parser
{
public:

    bool SetupFromNax3(const char*);

    vector<Nax3Clip> ListOfClips;
    vector<Nax3Curve> ListOfCurves;
    vector<Vector3D> ListOfKeys;
    vector<int> CurrentAnimationPosition;

};

}