//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include "NAX3parser.h"
#include "nvx2streamreader.h"
#include "nvx2fileformatstructs.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include "objloader.hpp"
#include <cstring>

#include <sstream>


const GLchar* vs =
"#version 430\n"
//"layout(location=0) in vec3 pos;\n"
"layout(location=0) in vec4 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=2) in vec2 InTexture;\n"
"layout(location=3) in vec4 Weights;\n"
"layout(location=4) in ivec4 Indices;\n"
"layout(location=5) in float WeightSize;\n"

"out vec4 Color;\n"
"out vec2 TheTexture;\n"

"uniform mat4 MVP;\n"
"uniform mat4 ListOfJoints[21];\n"
//"uniform mat4 ScaleMatrix;\n"
//"uniform int JointIndex;\n"

"void main()\n"
"{\n"

"	vec4 Weights2 = Weights / 255.0;\n"
//"	vec4 Weights2 = Weights;\n"
//"	vec4 Weights2 = Weights / WeightSize;\n"

"	mat4 BoneTransformation = ListOfJoints[Indices[0]] * Weights2[0];\n"
"	BoneTransformation += ListOfJoints[Indices[1]] * Weights2[1];\n"
"	BoneTransformation += ListOfJoints[Indices[2]] * Weights2[2];\n"
"	BoneTransformation += ListOfJoints[Indices[3]] * Weights2[3];\n"

/*"	vec4 ThePos = ListOfJoints[Indices[0]] * (pos * Weights[0]);\n"
"	ThePos += ListOfJoints[Indices[1]] * (pos * Weights2[1]);\n"
"	ThePos += ListOfJoints[Indices[2]] * (pos * Weights2[2]);\n"
"	ThePos += ListOfJoints[Indices[3]] * (pos * Weights2[3]);\n"*/


/*"	float Scalar = Weights[0] / WeightSize;\n"
"	vec4 ScaleVector = pos * Scalar;\n"
"	vec4 ThePos = ListOfJoints[Indices[0]] * ScaleVector;\n"

"	Scalar = Weights[1] / WeightSize;\n"
"	ScaleVector = pos * Scalar;\n"
"	ThePos += ListOfJoints[Indices[1]] * ScaleVector;\n"

"	Scalar = Weights[2] / WeightSize;\n"
"	ScaleVector = pos * Scalar;\n"
"	ThePos += ListOfJoints[Indices[2]] * ScaleVector;\n"

"	Scalar = Weights[3] / WeightSize;\n"
"	ScaleVector = pos * Scalar;\n"
"	ThePos += ListOfJoints[Indices[3]] * ScaleVector;\n"*/

//"	gl_Position = vec4(pos, 1);\n"
//"	gl_Position = MVP * pos;\n"
//"	gl_Position = MVP * ListOfJoints[JointIndex] * ScaleMatrix * pos;\n"
"	vec4 ThePos = BoneTransformation * pos;\n"
"	gl_Position = MVP * ThePos;\n"
//"	gl_Position = MVP * vec4(ThePos.xyz, 1.0);\n"

"	Color = color;\n"
"	TheTexture = InTexture;\n"
"}\n";

const GLchar* ps =
"#version 430\n"

"struct Material\n" 
"{\n"
"    sampler2D diffuse;\n"
"    vec3      specular;\n"
"    float     shininess;\n"
"};\n"

"in vec4 Color;\n"
"in vec2 TheTexture;\n"

"uniform sampler2D ourTexture;\n"

"out vec4 color;\n"

"void main()\n"
"{\n"

//"	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));\n"
//"	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));\n"


//"	color = Color;\n"
"	color = texture(ourTexture, TheTexture);\n"
"}\n";


/*
const GLchar* vs =
"#version 430\n"
//"layout(location=0) in vec3 pos;\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"uniform mat4 MVP;\n"
"void main()\n"
"{\n"
//"	gl_Position = vec4(pos, 1);\n"
"	gl_Position = MVP * pos;\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";
*/

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ExampleApp::ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	// window->SetKeyPressFunction([this](int32, int32, int32, int32)
	// {
	// 	this->window->Close();
	// });


	GLfloat buf[] =
	{
		-0.5f,	-0.5f,	-1,		1,	// pos 0
		0,		0.5f,	-1,		1,	// pos 1
		0.5f,	-0.5f,	-1,		1	// pos 2
	};


	GLfloat bufColor[] = 
	{
		1,		0,		0,		1,	// color 0
		0,		1,		0,		1,	// color 1
		0,		0,		1,		1	// color 2
	};

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// setup vertex shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLint length = std::strlen(vs);
		glShaderSource(this->vertexShader, 1, &vs, &length);
		glCompileShader(this->vertexShader);

		// get error log
		GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = std::strlen(ps);
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		// get error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		// create a program object
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->pixelShader);
		glLinkProgram(this->program);
		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}

		this->MatrixID = glGetUniformLocation(this->program, "MVP");			//Added stuff
		this->TheJoints = glGetUniformLocation(this->program, "ListOfJoints");


		//setup vbo
		glGenBuffers(1, &this->triangle);
		glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);


		glGenBuffers(1, &this->color);
		glBindBuffer(GL_ARRAY_BUFFER, this->color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(bufColor), bufColor, GL_STATIC_DRAW);


		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/


//
//
//

void ExampleApp::computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(this->window->window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(this->window->window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	Vector3D direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle),
		0
	);
	
	// Right vector
	Vector3D right(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f),
		0
	);
	
	// Up vector
	Vector3D up = right.CrossProduct(direction);

	// Move forward
	if (glfwGetKey( window->window, GLFW_KEY_UP ) == GLFW_PRESS){
		position = position + direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window->window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position = position - direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window->window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position = position + right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window->window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position = position - right * deltaTime * speed;
	}

	//float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.


	Matrix3D Projection = Projection.ProjectionMatrix(initialFoV, 4.0f/3.0f, 0.1f, 100.0f);

	Matrix3D View = View.ViewMatrix(position, position + direction, up);
	//Matrix3D View = View.ViewMatrix(position, Vector3D(0,0,0,1), Vector3D(0,1,0,1));

	this->MVP = Projection * View;

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}


//
//
//

vector<float> SplitAndConvertToFloat(std::string strToSplit, char delimeter)
{
	std::stringstream ss(strToSplit);
	string item;
	//char* end = delimiter;
	std::vector<float> splittedStringsAsFloats;
	while (std::getline(ss, item, delimeter))
	{
		const char* itemAsChar = item.c_str();
		float value = atof(itemAsChar);
		splittedStringsAsFloats.push_back(value);
	}
	return splittedStringsAsFloats;
}

void ExampleApp::AssignElementToJoint(const tinyxml2::XMLElement* theElement)
{
	int index;
	theElement->QueryIntAttribute("index", &index);

	int ParentIndex;
	//theElement->QueryIntAttribute("parent", &ListOfJoints[index].parent);
	theElement->QueryIntAttribute("parent", &ParentIndex);
	ListOfJoints[index].parent = ParentIndex;

	if (ParentIndex >= 0)
		ListOfJoints[ParentIndex].ListOfChildren.push_back(index);

	const char* JointName;
	theElement->QueryStringAttribute("name", &JointName);
	ListOfJoints[index].name = JointName;
	


	const char* title;
	theElement->QueryStringAttribute("position", &title);
	ListOfJoints[index].VectorOfCoordinates = SplitAndConvertToFloat(title, ',');


	theElement->QueryStringAttribute("rotation", &title);
	vector<float> VectorOfRotations = SplitAndConvertToFloat(title, ',');
	ListOfJoints[index].rotation.vektor[0] = VectorOfRotations[0];
	ListOfJoints[index].rotation.vektor[1] = VectorOfRotations[1];
	ListOfJoints[index].rotation.vektor[2] = VectorOfRotations[2];
	ListOfJoints[index].rotation.vektor[3] = VectorOfRotations[3];

	Matrix3D TranslationMatrix;

	TranslationMatrix.matris[3][0] = ListOfJoints[index].VectorOfCoordinates[0];
	TranslationMatrix.matris[3][1] = ListOfJoints[index].VectorOfCoordinates[1];
	TranslationMatrix.matris[3][2] = ListOfJoints[index].VectorOfCoordinates[2];

	if (index > 0)
	{
		TranslationMatrix = ListOfJoints[ListOfJoints[index].parent].coordinates * TranslationMatrix;
	}

	Matrix3D RotationMatrix = RotationMatrix.QuaternionToMatrix(ListOfJoints[index].rotation);

	ListOfJoints[index].coordinates = TranslationMatrix * RotationMatrix;
}




bool ExampleApp::loadOBJ(const char * path, vector<Vector3D> & out_vertices, vector<Vector2D> & out_uvs, vector<Vector3D> & out_normals)
{
	printf("Loading OBJ file %s...\n", path);

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<Vector3D> temp_vertices; 
	vector<Vector2D> temp_uvs;
	vector<Vector3D> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			Vector3D vertex;
			fscanf(file, "%f %f %f\n", &vertex.vektor[0], &vertex.vektor[1], &vertex.vektor[2]);
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			Vector2D uv;
			fscanf(file, "%f %f\n", &uv.vektor[0], &uv.vektor[1]);
			uv.vektor[1] = -uv.vektor[1]; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			Vector3D normal;
			fscanf(file, "%f %f %f\n", &normal.vektor[0], &normal.vektor[1], &normal.vektor[2] );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2] );
			if (matches != 6){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			// uvIndices.push_back(uvIndex[0]);
			// uvIndices.push_back(uvIndex[1]);
			// uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		//unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		Vector3D vertex = temp_vertices[ vertexIndex-1 ];
		//Vector2D uv = temp_uvs[ uvIndex-1 ];
		Vector3D normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		//out_uvs.push_back(uv);
		out_normals.push_back(normal);
	
	}
	fclose(file);
	return true;
}

void ExampleApp::UpdateChildren(int index)
{
	for (int i = 0; i < ListOfJoints[index].ListOfChildren.size(); i++)
	{
		int ChildIndex = ListOfJoints[index].ListOfChildren[i];


		Matrix3D TranslationMatrix;

		TranslationMatrix.matris[3][0] = ListOfJoints[ChildIndex].VectorOfCoordinates[0];
		TranslationMatrix.matris[3][1] = ListOfJoints[ChildIndex].VectorOfCoordinates[1];
		TranslationMatrix.matris[3][2] = ListOfJoints[ChildIndex].VectorOfCoordinates[2];


		TranslationMatrix = ListOfJoints[index].coordinates * TranslationMatrix;


		Matrix3D RotationMatrix = RotationMatrix.QuaternionToMatrix(ListOfJoints[ChildIndex].rotation);

		ListOfJoints[ChildIndex].coordinates = TranslationMatrix * RotationMatrix;

		UpdateChildren(ChildIndex);
	}
}

void ExampleApp::ChangeAnimation(int* AnimationIndex, int* AnimationFrame)
{
	if (glfwGetKey( window->window, GLFW_KEY_1 ) == GLFW_PRESS && *AnimationIndex != 0)
	{
		*AnimationIndex = 0;
		*AnimationFrame = 0;
	}

	else if (glfwGetKey( window->window, GLFW_KEY_2 ) == GLFW_PRESS && *AnimationIndex != 1)
	{
		*AnimationIndex = 1;
		*AnimationFrame = 0;
	}

	else if (glfwGetKey( window->window, GLFW_KEY_3 ) == GLFW_PRESS && *AnimationIndex != 2)
	{
		*AnimationIndex = 2;
		*AnimationFrame = 0;
	}

	else if (glfwGetKey( window->window, GLFW_KEY_4 ) == GLFW_PRESS && *AnimationIndex != 3)
	{
		*AnimationIndex = 3;
		*AnimationFrame = 0;
	}

	else if (glfwGetKey( window->window, GLFW_KEY_5 ) == GLFW_PRESS && *AnimationIndex != 4)
	{
		*AnimationIndex = 4;
		*AnimationFrame = 0;
	}

	else if (glfwGetKey( window->window, GLFW_KEY_6 ) == GLFW_PRESS && *AnimationIndex != 5)
	{
		*AnimationIndex = 5;
		*AnimationFrame = 0;
	}

	else if (glfwGetKey( window->window, GLFW_KEY_7 ) == GLFW_PRESS && *AnimationIndex != 6)
	{
		*AnimationIndex = 6;
		*AnimationFrame = 0;
	}

	else if (glfwGetKey( window->window, GLFW_KEY_8 ) == GLFW_PRESS && *AnimationIndex != 7)
	{
		*AnimationIndex = 7;
		*AnimationFrame = 0;
	}
}

void ExampleApp::Run()
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
		

	tinyxml2::XMLDocument doc;



	const char* filepath = "/home/destinum/Documents/GrafikprogrammeringOchAlgoritmer/projects/Skeleton/footman/Unit_Footman.constants";
	tinyxml2::XMLError eResult = doc.LoadFile(filepath);
	if (eResult != tinyxml2::XML_SUCCESS) 
	   cout << "No loaded file" << endl;



	const tinyxml2::XMLElement* theElement = doc.FirstChildElement()->FirstChildElement()->FirstChildElement()->FirstChildElement()->FirstChildElement();
	
	for(int i = 0; i < 21; i++)
	{
		theElement = theElement->NextSiblingElement();
		AssignElementToJoint(theElement);
	}


	/*cout << ListOfJoints[0].name << endl;
	cout << ListOfJoints[1].name << endl;
	cout << ListOfJoints[20].name << endl;*/

	
		// Read the .obj file
		const char* filepathOfObject = "/home/destinum/Documents/GrafikprogrammeringOchAlgoritmer/projects/Skeleton/Mesh/sphere.obj";
		vector<Vector3D> vertices;
		vector<Vector2D>  uvs;
		vector<Vector3D>  normals; // Won't be used at the moment.
		bool res = ExampleApp::loadOBJ(filepathOfObject, vertices, uvs, normals);

		int SizeOfList = vertices.size()*4;
		float ListToBuffer[SizeOfList];

		for (int i = 0; i < vertices.size(); i++)
		{
			int n = i*4;
			ListToBuffer[n] = vertices[i].vektor[0];
			ListToBuffer[n + 1] = vertices[i].vektor[1];
			ListToBuffer[n + 2] = vertices[i].vektor[2];
			ListToBuffer[n + 3] = vertices[i].vektor[3];

			//cout << ListToBuffer[n] << " :: " << ListToBuffer[n + 1] << " :: " << ListToBuffer[n + 2] << " :: " << ListToBuffer[n + 3] << endl;
		}

		GLuint ObjectVertexBuffer;
		glGenBuffers(1, &ObjectVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, ObjectVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ListToBuffer), ListToBuffer, GL_STATIC_DRAW);


		GLuint LinesBuffer;
		glGenBuffers(1, &LinesBuffer);



	

		const char* NAX3filepath = "/home/destinum/Documents/GrafikprogrammeringOchAlgoritmer/projects/Skeleton/footman/Unit_Footman.nax3";
		CoreAnimation::NAX3parser AnimationParser;
		AnimationParser.SetupFromNax3(NAX3filepath);

		//Current Stuff

		const char* nvx2filepath = "/home/destinum/Documents/GrafikprogrammeringOchAlgoritmer/projects/Skeleton/footman/Unit_Footman.nvx2";
		CoreGraphics::nvx2parser ModelParser;
		ModelParser.SetupFromNvx2(nvx2filepath);



		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		//unsigned char *data1 = stbi_load("/home/destinum/Documents/GrafikprogrammeringOchAlgoritmer/projects/Skeleton/footman/Footman_Diffuse.tga", &width, &height, &nrChannels, 0);
		unsigned char *data2 = stbi_load("/home/destinum/Documents/GrafikprogrammeringOchAlgoritmer/projects/Skeleton/footman/Footman_Normal.tga", &width, &height, &nrChannels, 0); 
		//unsigned char *data3 = stbi_load("/home/destinum/Documents/GrafikprogrammeringOchAlgoritmer/projects/Skeleton/footman/Footman_Specular.tga", &width, &height, &nrChannels, 0); 
 
		if (/*data1 &&*/ data2 /*&& data3*/)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		//stbi_image_free(data1);
		stbi_image_free(data2);
		//stbi_image_free(data3);

		//cout << vertices.size() << endl;
		//cout << ModelParser.vertices.size() << endl;

		//Current Stuff Ends

	while (this->window->IsOpen())
	{
		static int AnimationIndex = 2;

		static double lastTime = glfwGetTime();
		static int AnimationFrame = 0;

		ChangeAnimation(&AnimationIndex, &AnimationFrame);

		// Compute time difference between current and last frame
		double currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		if (deltaTime >= 0.1)
		{
			lastTime = currentTime;

			for (int index = 0; index < 21; index++)
			{

				int currentKeyIndex = (AnimationParser.CurrentAnimationPosition[AnimationIndex] + AnimationFrame) * 84 + index * 4;

				Matrix3D TranslationMatrix;

				ListOfJoints[index].VectorOfCoordinates[0] = TranslationMatrix.matris[3][0] = AnimationParser.ListOfKeys[currentKeyIndex]->vektor[0];
				ListOfJoints[index].VectorOfCoordinates[1] = TranslationMatrix.matris[3][1] = AnimationParser.ListOfKeys[currentKeyIndex]->vektor[1];
				ListOfJoints[index].VectorOfCoordinates[2] = TranslationMatrix.matris[3][2] = AnimationParser.ListOfKeys[currentKeyIndex]->vektor[2];

				if (index > 0)
						TranslationMatrix = ListOfJoints[ListOfJoints[index].parent].coordinates * TranslationMatrix;


				Vector3D RotationVector(AnimationParser.ListOfKeys[currentKeyIndex + 1]->vektor[0], AnimationParser.ListOfKeys[currentKeyIndex + 1]->vektor[1], AnimationParser.ListOfKeys[currentKeyIndex + 1]->vektor[2], AnimationParser.ListOfKeys[currentKeyIndex + 1]->vektor[3]);
				Matrix3D RotationMatrix = RotationMatrix.QuaternionToMatrix(RotationVector);

				ListOfJoints[index].coordinates = TranslationMatrix * RotationMatrix;

			}

			/*Matrix3D RotationMatrix = RotationMatrix.QuaternionToMatrix(Vector3D( 0.0174524, 0, 0, 0.9998477));

			ListOfJoints[5].coordinates = ListOfJoints[5].coordinates * RotationMatrix;
			UpdateChildren(5);

			ListOfJoints[9].coordinates = ListOfJoints[9].coordinates * RotationMatrix;
			UpdateChildren(9);*/

			AnimationFrame += 1;
			
			if(AnimationFrame >= AnimationParser.ListOfClips[AnimationIndex]->numKeys)
			{
				AnimationFrame = 0;
			}
		}


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		
		// do stuff
		glUseProgram(this->program);

		computeMatricesFromInputs();

		glEnableVertexAttribArray(0);
		glUniformMatrix4fv(this->MatrixID, 1, GL_FALSE, &(this->MVP).matris[0][0]);


		int JointsRendered = 21;

		//Current Stuff

		Matrix3D JointCoordinates[JointsRendered];

		for (int i = 0; i < JointsRendered; i++)
		{
			JointCoordinates[i] = ListOfJoints[i].coordinates;
		}

		glUniformMatrix4fv(this->TheJoints, JointsRendered, GL_FALSE, &(JointCoordinates[0]).matris[0][0]);		//Transpose?????

		//Current Stuff End

		
		/*glBindBuffer(GL_ARRAY_BUFFER, ObjectVertexBuffer);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

		Matrix3D ScaleMatrix;
		ScaleMatrix.matris[0][0] = ScaleMatrix.matris[1][1] = ScaleMatrix.matris[2][2] = 0.03;
		glUniformMatrix4fv(glGetUniformLocation(this->program, "ScaleMatrix"), 1, GL_FALSE, &ScaleMatrix.matris[0][0]);

		for(int i = 1; i < JointsRendered; i++)
		{
			//glUniformMatrix4fv(this->MatrixID, 1, GL_FALSE, &(this->MVP * ListOfJoints[i].coordinates * ScaleMatrix).matris[0][0]);
			glUniform1i(glGetUniformLocation(program, "JointIndex"), i);
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		}
		

		glBindBuffer(GL_ARRAY_BUFFER, LinesBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		Matrix3D ScaleMatrix2;
		glUniformMatrix4fv(glGetUniformLocation(this->program, "ScaleMatrix"), 1, GL_FALSE, &ScaleMatrix2.matris[0][0]);

		for(int i = 2; i < JointsRendered; i++)
		{
			GLfloat lineVertices[] = 
			{
				0, 0, 0,
				ListOfJoints[i].VectorOfCoordinates[0], ListOfJoints[i].VectorOfCoordinates[1], ListOfJoints[i].VectorOfCoordinates[2]
				//ListOfJoints[i].coordinates.matris[3][0], ListOfJoints[i].coordinates.matris[3][1], ListOfJoints[i].coordinates.matris[3][2]
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);

			//glUniformMatrix4fv(this->MatrixID, 1, GL_FALSE, &(this->MVP * ListOfJoints[ ListOfJoints[i].parent].coordinates).matris[0][0]);
			glUniform1i(glGetUniformLocation(program, "JointIndex"), ListOfJoints[i].parent);
			glDrawArrays(GL_LINES, 0, 2);
		}*/


		//Current Stuff

		//glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);

		glBindBuffer(GL_ARRAY_BUFFER, ModelParser.VertexBuffer);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), NULL);
		//glBindTexture(GL_TEXTURE_2D, texture);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)( 4 * sizeof(float)));
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)( 6 * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, ModelParser.BoneDataBuffer);
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, 2 * sizeof(float), NULL);
		glVertexAttribIPointer(4, 4, GL_UNSIGNED_BYTE, 2 * sizeof(float), (void*)(sizeof(float)));


		//glBindBuffer(GL_ARRAY_BUFFER, this->color);
		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

		glDrawArrays(GL_TRIANGLES, 0, ModelParser.vertices.size());
		//glDrawArrays(GL_TRIANGLES, 0, 1);


		//glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);

		

		//Current Stuff Ends



		
		glBindBuffer(GL_ARRAY_BUFFER, 0);		//int index;

		glDisableVertexAttribArray(0);

		this->window->SwapBuffers();			//int index;
	}
}

} // namespace Example