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

		//cout << "File length: " << length << endl;

		char theBuffer[length];
		char * buffer = theBuffer;

		//int BufferIndex = 0;

		AnimationFile.read(buffer, length);

		//
		//

		Nax3Header* naxHeader = (Nax3Header*) buffer;
    	buffer += sizeof(Nax3Header);
		//BufferIndex += sizeof(Nax3Header);

		/*cout << "Magic: "<< naxHeader->magic << endl;
		cout << "numClips: " << naxHeader->numClips << endl;
		cout << "numKeys: " << naxHeader->numKeys << endl;*/

		//if (naxHeader->numClips > 0)
		for (uint ClipIndex = 0; ClipIndex < naxHeader->numClips; ClipIndex++)
		{
			Nax3Clip* naxClip = (Nax3Clip*) buffer;
			ListOfClips.push_back(naxClip);
			buffer += sizeof(Nax3Clip);

			if (ClipIndex == 0)
				CurrentAnimationPosition.push_back(0);
			
			else
				CurrentAnimationPosition.push_back(CurrentAnimationPosition[ClipIndex - 1] + ListOfClips[ClipIndex - 1]->numKeys);

			/*cout << endl;
			cout << "numCurves: " << naxClip->numCurves << endl;
			cout << "startKeyIndex: " << naxClip->startKeyIndex << endl;
			cout << "numKeys: " << naxClip->numKeys << endl;
			cout << "keyStride: " << naxClip->keyStride << endl;
			cout << "keyDuration: " << naxClip->keyDuration << endl;
			cout << "preInfinityType: " << static_cast<unsigned>(naxClip->preInfinityType) << endl;
			cout << "postInfinityType: " << static_cast<unsigned>(naxClip->postInfinityType) << endl;
			cout << "numEvents: " << naxClip->numEvents << endl;
			cout << "name: " << naxClip->name << endl;*/

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

					/*cout << endl;
					cout << "Curve firstKeyIndex: " << naxCurve->firstKeyIndex << endl;
					cout << "Curve isActive: " << static_cast<unsigned>(naxCurve->isActive) << endl;
					cout << "Curve isStatic: " << static_cast<unsigned>(naxCurve->isStatic) << endl;
					cout << "Curve curveType: " << static_cast<unsigned>(naxCurve->curveType) << endl;
					cout << "Curve _padding: " << static_cast<unsigned>(naxCurve->_padding) << endl;
					cout << "Curve staticKeyX: " << naxCurve->staticKeyX << endl;
					cout << "Curve staticKeyY: " << naxCurve->staticKeyY << endl;
					cout << "Curve staticKeyZ: " << naxCurve->staticKeyZ << endl;
					cout << "Curve staticKeyW: " << naxCurve->staticKeyW << endl;*/
				}
				
				buffer += sizeof(Nax3Curve);
			}
		}

		for (uint KeyIndex = 0; KeyIndex < naxHeader->numKeys; KeyIndex++)
		{
			Vector3D* KeyVector = (Vector3D*) buffer;
			
			int fisk = 15;

			if (KeyIndex >= 84*0 && KeyIndex < 84*1/*KeyIndex == fisk || KeyIndex == fisk + 84 || KeyIndex == fisk + 84*2 || KeyIndex == 36 + 84*3 || KeyIndex == 36 + 84*4*/)
			{
				/*cout << endl << "Index: " << KeyIndex <<endl;
				cout << "X-Value: " << KeyVector->vektor[0] << endl;
				cout << "Y-Value: " << KeyVector->vektor[1] << endl;
				cout << "Z-Value: " << KeyVector->vektor[2] << endl;
				cout << "W-Value: " << KeyVector->vektor[3] << endl;*/
			}

			this->ListOfKeys.push_back(KeyVector);

			buffer += sizeof(Vector3D);
		}








		// if (AnimationFile)
		// 	cout << "all characters read successfully." << endl;
		// else
		// 	cout << "error: only " << AnimationFile.gcount() << " could be read" << endl;

		AnimationFile.close();
		//delete[] buffer;
		return true;
	}

	

}