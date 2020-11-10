#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "tinyxml2.h"
#include "MyMathLibrary/Matrix3DHeader.h"
#include "MyMathLibrary/Vector3DHeader.h"
#include "MyMathLibrary/Vector2DHeader.h"

#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();

	void AssignElementToJoint(const tinyxml2::XMLElement* Element);

	bool loadOBJ(const char * path, vector<Vector3D> & out_vertices, vector<Vector2D> & out_uvs, vector<Vector3D> & out_normals);

	void computeMatricesFromInputs();

	void UpdateChildren(int index);

	void ChangeAnimation(int* AnimationIndex, int* AnimationFrame);

	unsigned int LoadTexture(char const* filepath);

	Vector3D Slerp(Vector3D Quaternion1, Vector3D Quaternion2, float t);

	struct joint
	{
		string name;
		int parent;
		//joint* parent;

		Matrix3D coordinates;
		vector<float> VectorOfCoordinates;
		Vector3D rotation;
		Matrix3D InverseBindPose;

		vector<int> ListOfChildren;
	};

	joint ListOfJoints[21];
	//Display::Window* window;

private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	Display::Window* window;

	GLuint color;
	//GLuint triangle2;
	//GLuint color2;

	GLuint MatrixID;
	GLuint CameraPositionID;
	GLuint TheJoints;
	Matrix3D MVP;
	Matrix3D View;
	Matrix3D Projection;

	//GLuint MeshBuffer;


	//Camera controlls

	//Matrix3D ViewMatrix;
	//Matrix3D ProjectionMatrix;
	
	// Initial position : on +Z
	Vector3D position = Vector3D(0, 0, 10, 1); 
	// Initial horizontal angle : toward -Z
	float horizontalAngle = 3.14f;
	// Initial vertical angle : none
	float verticalAngle = 0.0f;
	// Initial Field of View
	float initialFoV = 45.0f;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.001f;
};
} // namespace Example