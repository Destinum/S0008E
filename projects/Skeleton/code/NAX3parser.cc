#include "NAX3parser.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace CoreAnimation
{

	bool NAX3parser::SetupFromNax3(const char* NAX3filepath)
	{

		ifstream AnimationFile(NAX3filepath, ifstream::binary);
		
		if( !AnimationFile.is_open() ){
			printf("Can't open file!\n");
			getchar();
			return false;
		}

		AnimationFile.seekg (0, AnimationFile.end);
		int length = AnimationFile.tellg();
		AnimationFile.seekg (0, AnimationFile.beg);

		char *theBuffer = new char[length];
		char * buffer = theBuffer;

		AnimationFile.read(buffer, length);

		//
		//

		Nax3Header* naxHeader = (Nax3Header*) buffer;
    	buffer += sizeof(Nax3Header);

		for (uint ClipIndex = 0; ClipIndex < naxHeader->numClips; ClipIndex++)
		{
			Nax3Clip* naxClip = (Nax3Clip*) buffer;
			ListOfClips.push_back(naxClip);
			buffer += sizeof(Nax3Clip);

			if (ClipIndex == 0)
				CurrentAnimationPosition.push_back(0);
			
			else
				CurrentAnimationPosition.push_back(CurrentAnimationPosition[ClipIndex - 1] + ListOfClips[ClipIndex - 1]->numKeys);

			for(ushort EventIndex = 0; EventIndex < naxClip->numEvents; EventIndex++)
			{
				buffer += sizeof(Nax3AnimEvent);
			}

			for(ushort CurveIndex = 0; CurveIndex < naxClip->numCurves; CurveIndex++)
			{
				if(ClipIndex == 0)
				{
					Nax3Curve* naxCurve = (Nax3Curve*) buffer;
					this->ListOfCurves.push_back(naxCurve);
				}
				
				buffer += sizeof(Nax3Curve);
			}
		}

		for (uint KeyIndex = 0; KeyIndex < naxHeader->numKeys; KeyIndex++)
		{
			Vector3D* KeyVector = (Vector3D*) buffer;

			this->ListOfKeys.push_back(KeyVector);

			buffer += sizeof(Vector3D);
		}

		// if (AnimationFile)
		// 	cout << "all characters read successfully." << endl;
		// else
		// 	cout << "error: only " << AnimationFile.gcount() << " could be read" << endl;

		AnimationFile.close();
		delete [] theBuffer;
		return true;
	}

	

}