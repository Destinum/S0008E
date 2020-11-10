#pragma once
#include "Vector3DHeader.h"

#define PI 3.14159265

class Matrix3D
{
public:

	Matrix3D();
	Matrix3D FillMatrix(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

	Matrix3D operator*(Matrix3D obj);
	Vector3D operator*(Vector3D obj);

	Matrix3D transpose();
	Matrix3D rotationX(float Q);
	Matrix3D rotationY(float Q);
	Matrix3D rotationZ(float Q);
	Matrix3D vectorRotation(float Q, Vector3D vector);
	Matrix3D inverse();

 	Matrix3D ProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar);
 	Matrix3D ViewMatrix(const Vector3D& Eye, const Vector3D& Center, const Vector3D& Up);
	
	Matrix3D QuaternionToMatrix(Vector3D);

	void ConvertToSingleList(float* ListToSaveIn[16]);

	float matris[4][4];
};

inline Matrix3D::Matrix3D() //Every matrix is created as an identity matrix
{
	for (int n = 0; n < 4; n++)
	{
		for (int i = 0; i < 4; i++)
		{
			this->matris[n][i] = 0;
		}
	}

	this->matris[0][0] = 1;
	this->matris[1][1] = 1;
	this->matris[2][2] = 1;
	this->matris[3][3] = 1;
}

inline Matrix3D Matrix3D::FillMatrix(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44)
{
	this->matris[0][0] = a11;
	this->matris[1][0] = a12;
	this->matris[2][0] = a13;
	this->matris[3][0] = a14;

	this->matris[0][1] = a21;
	this->matris[1][1] = a22;
	this->matris[2][1] = a23;
	this->matris[3][1] = a24;

	this->matris[0][2] = a31;
	this->matris[1][2] = a32;
	this->matris[2][2] = a33;
	this->matris[3][2] = a34;

	this->matris[0][3] = a41;
	this->matris[1][3] = a42;
	this->matris[2][3] = a43;
	this->matris[3][3] = a44;

	return *this;
}

inline Matrix3D Matrix3D::operator*(Matrix3D obj)
{
	Matrix3D TheMatrix;

	for (int n = 0; n < 4; n++)
	{
		for (int i = 0; i < 4; i++)
		{
			TheMatrix.matris[n][i] = this->matris[0][i] * obj.matris[n][0] + this->matris[1][i] * obj.matris[n][1] + this->matris[2][i] * obj.matris[n][2] + this->matris[3][i] * obj.matris[n][3];
		}
	}

	return TheMatrix;
}

inline Vector3D Matrix3D::operator*(Vector3D obj)
{
	Vector3D TheVector;

	for (int n = 0; n < 4; n++)
	{
		TheVector.vektor[n] = this->matris[0][n] * obj.vektor[0] + this->matris[1][n] * obj.vektor[1] + this->matris[2][n] * obj.vektor[2] + this->matris[3][n] * obj.vektor[3];
	}

	return TheVector;
}

inline Matrix3D Matrix3D::transpose()
{
	Matrix3D TheMatrix;

	for (int n = 0; n < 4; n++)
	{
		for (int i = 0; i < 4; i++)
		{
			TheMatrix.matris[n][i] = this->matris[i][n];
		}
	}

	return TheMatrix;
}

inline Matrix3D Matrix3D::rotationX(float Q)
{
	Matrix3D temp;

	temp.matris[1][1] = cos(Q*PI / 180.0f);
	temp.matris[2][1] = -sin(Q*PI / 180.0f);
	temp.matris[1][2] = sin(Q*PI / 180.0f);
	temp.matris[2][2] = cos(Q*PI / 180.0f);

	return temp;
}

inline Matrix3D Matrix3D::rotationY(float Q)
{
	Matrix3D temp;

	temp.matris[0][0] = cos(Q*PI / 180.0f);
	temp.matris[0][2] = -sin(Q*PI / 180.0f);
	temp.matris[2][0] = sin(Q*PI / 180.0f);
	temp.matris[2][2] = cos(Q*PI / 180.0f);
	
	return temp;
}

inline Matrix3D Matrix3D::rotationZ(float Q)
{
	Matrix3D temp;

	temp.matris[0][0] = cos(Q*PI / 180.0f);
	temp.matris[1][0] = -sin(Q*PI / 180.0f);
	temp.matris[0][1] = sin(Q*PI / 180.0f);
	temp.matris[1][1] = cos(Q*PI / 180.0f);

	return temp;
}

inline Matrix3D Matrix3D::vectorRotation(float Q, Vector3D r)
{
	Q *= PI / 180.0f;
	Matrix3D temp;
	Vector3D tempS =r.normalisera();
	
	float u = tempS.vektor[0] * tempS.vektor[0];
	float v = tempS.vektor[1] * tempS.vektor[1];
	float w = tempS.vektor[2] * tempS.vektor[2];

	float c = cos(Q);
	float s = sin(Q);
	float t = 1 - cos(Q);
	
	temp.matris[0][0] = u*t + c;
	temp.matris[0][1] = t * tempS.vektor[0] * tempS.vektor[1] + s * tempS.vektor[2];
	temp.matris[0][2] = t * tempS.vektor[0] * tempS.vektor[2] - s * tempS.vektor[1];

	temp.matris[1][0] = t * tempS.vektor[0] * tempS.vektor[1] - s * tempS.vektor[2];
	temp.matris[1][1] = v*t + c;
	temp.matris[1][2] = t * tempS.vektor[1] * tempS.vektor[2] + s * tempS.vektor[0];

	temp.matris[2][0] = t * tempS.vektor[0] * tempS.vektor[2] + s * tempS.vektor[1];
	temp.matris[2][1] = t * tempS.vektor[1] * tempS.vektor[2] - s * tempS.vektor[0];
	temp.matris[2][2] = w*t + c;

	return temp;
}

inline Matrix3D Matrix3D::inverse()
{
	Matrix3D noResult;
	Matrix3D Inverse;
	float determinant;

	float a11 = this->matris[0][0];
	float a12 = this->matris[1][0];
	float a13 = this->matris[2][0];
	float a14 = this->matris[3][0];

	float a21 = this->matris[0][1];
	float a22 = this->matris[1][1];
	float a23 = this->matris[2][1];
	float a24 = this->matris[3][1];

	float a31 = this->matris[0][2];
	float a32 = this->matris[1][2];
	float a33 = this->matris[2][2];
	float a34 = this->matris[3][2];

	float a41 = this->matris[0][3];
	float a42 = this->matris[1][3];
	float a43 = this->matris[2][3];
	float a44 = this->matris[3][3];

	determinant = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43 +
		a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41 +
		a13*a21*a32*a44 + a13*a22*a34*a41 + a13*a24*a31*a42 +
		a14*a21*a33*a42 + a14*a22*a31*a43 + a14*a23*a32*a41 -

		a11*a22*a34*a43 - a11*a23*a32*a44 - a11*a24*a33*a42 -
		a12*a21*a33*a44 - a12*a23*a34*a41 - a12*a24*a31*a43 -
		a13*a21*a34*a42 - a13*a22*a31*a44 - a13*a24*a32*a41 -
		a14*a21*a32*a43 - a14*a22*a33*a41 - a14*a23*a31*a42;
	
	if (determinant == 0.0)
	{
		cout << "This matrix does not have an inverse." << endl;
		return noResult;
	}

	Inverse.matris[0][0] = 1.0 / determinant * (a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a24*a33*a42);
	Inverse.matris[1][0] = 1.0 / determinant * (a12*a34*a43 + a13*a32*a44 + a14*a33*a42 - a12*a33*a44 - a13*a34*a42 - a14*a32*a43);
	Inverse.matris[2][0] = 1.0 / determinant * (a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42);
	Inverse.matris[3][0] = 1.0 / determinant * (a12*a24*a33 + a13*a22*a34 + a14*a23*a32 - a12*a23*a34 - a13*a24*a32 - a14*a22*a33);

	Inverse.matris[0][1] = 1.0 / determinant * (a21*a34*a43 + a23*a31*a44 + a24*a33*a41 - a21*a33*a44 - a23*a33*a41 - a24*a31*a43);
	Inverse.matris[1][1] = 1.0 / determinant * (a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41);
	Inverse.matris[2][1] = 1.0 / determinant * (a11*a24*a43 + a13*a21*a44 + a14*a23*a41 - a11*a23*a44 - a13*a24*a41 - a14*a21*a43);
	Inverse.matris[3][1] = 1.0 / determinant * (a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31);

	Inverse.matris[0][2] = 1.0 / determinant * (a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41);
	Inverse.matris[1][2] = 1.0 / determinant * (a11*a34*a42 + a12*a31*a44 + a14*a32*a41 - a11*a32*a44 - a12*a34*a41 - a14*a31*a42);
	Inverse.matris[2][2] = 1.0 / determinant * (a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41);
	Inverse.matris[3][2] = 1.0 / determinant * (a11*a24*a32 + a12*a21*a34 + a14*a22*a31 - a11*a22*a34 - a12*a24*a31 - a14*a21*a32);

	Inverse.matris[0][3] = 1.0 / determinant * (a21*a33*a42 + a22*a31*a43 + a23*a32*a41 - a21*a32*a43 - a22*a33*a41 - a23*a31*a42);
	Inverse.matris[1][3] = 1.0 / determinant * (a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41);
	Inverse.matris[2][3] = 1.0 / determinant * (a11*a23*a42 + a12*a21*a43 + a13*a22*a41 - a11*a22*a43 - a12*a23*a41 - a13*a21*a42);
	Inverse.matris[3][3] = 1.0 / determinant * (a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31);

	return Inverse;
}

inline Matrix3D Matrix3D::ProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar) // Left-handed projection
{
    Matrix3D out;

	fov *= PI / 180.0f;


	float tanHalfFovy = tan(fov / 2.0f);
    //float xScale = yScale / aspectRatio;

    out.matris[0][0] = 1/(aspectRatio*tanHalfFovy);
    out.matris[1][1] = 1/(tanHalfFovy);
    out.matris[2][2] = - (zFar + zNear) / (zFar - zNear);
    out.matris[2][3] = - 1;
    out.matris[3][3] = 0;
    out.matris[3][2] = - 2 * zNear * zFar / (zFar - zNear);

    return out;
}


inline Matrix3D Matrix3D::ViewMatrix(const Vector3D& Eye, const Vector3D& Center, const Vector3D& Up)
{
	Matrix3D out;

	Vector3D X, Y, Z;

	//Z = Eye - Center;

	 X.vektor[0] = Center.vektor[0] - Eye.vektor[0];
	 X.vektor[1] = Center.vektor[1] - Eye.vektor[1];
	 X.vektor[2] = Center.vektor[2] - Eye.vektor[2];
     X = X.normalisera();
	 
     Y = X.CrossProduct(Up);
	 Y = Y.normalisera();

     Z = Y.CrossProduct(X);
	 Z = Z.normalisera();

	 out.matris[0][0] = Y.vektor[0];
	 out.matris[1][0] = Y.vektor[1];
	 out.matris[2][0] = Y.vektor[2];
	 
	 out.matris[0][1] = Z.vektor[0];
	 out.matris[1][1] = Z.vektor[1];
	 out.matris[2][1] = Z.vektor[2];

	 out.matris[0][2] = -X.vektor[0];
	 out.matris[1][2] = -X.vektor[1];
	 out.matris[2][2] = -X.vektor[2];

	 out.matris[3][0] = -Y.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
	 out.matris[3][1] = -Z.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
	 out.matris[3][2] = X.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);

	return out;
}

inline Matrix3D Matrix3D::QuaternionToMatrix(Vector3D Quaternion)
{
	Matrix3D out;
	out.FillMatrix(
	Quaternion.vektor[3], Quaternion.vektor[2], -Quaternion.vektor[1], Quaternion.vektor[0], 
	-Quaternion.vektor[2], Quaternion.vektor[3], Quaternion.vektor[0], Quaternion.vektor[1], 
	Quaternion.vektor[1], -Quaternion.vektor[0], Quaternion.vektor[3], Quaternion.vektor[2], 
	-Quaternion.vektor[0], -Quaternion.vektor[1], -Quaternion.vektor[2], Quaternion.vektor[3]
	);



	Matrix3D temp;
	temp.FillMatrix(
	Quaternion.vektor[3], Quaternion.vektor[2], -Quaternion.vektor[1], -Quaternion.vektor[0], 
	-Quaternion.vektor[2], Quaternion.vektor[3], Quaternion.vektor[0], -Quaternion.vektor[1], 
	Quaternion.vektor[1], -Quaternion.vektor[0], Quaternion.vektor[3], -Quaternion.vektor[2], 
	Quaternion.vektor[0], Quaternion.vektor[1], Quaternion.vektor[2], Quaternion.vektor[3]
	);

	// out.matris[0][0] = 1 - 2*Quarternion.vektor[1] * Quarternion.vektor[1] - 2*Quarternion.vektor[2] * Quarternion.vektor[2];
	// out.matris[1][0] = 2 * Quarternion.vektor[0] * Quarternion.vektor[1] - 2 * Quarternion.vektor[2] * Quarternion.vektor[3];
	// out.matris[2][0] = 2 * Quarternion.vektor[0] * Quarternion.vektor[2] + 2 * Quarternion.vektor[1] * Quarternion.vektor[3];
	
	// out.matris[0][1] = 2 * Quarternion.vektor[0] * Quarternion.vektor[1] + 2 * Quarternion.vektor[2] * Quarternion.vektor[3];
	// out.matris[1][1] = 1 - 2 * Quarternion.vektor[0] * Quarternion.vektor[0] - 2 * Quarternion.vektor[2] * Quarternion.vektor[2];
	// out.matris[2][1] = 2 * Quarternion.vektor[1] * Quarternion.vektor[2] - 2 * Quarternion.vektor[0] * Quarternion.vektor[3];

	// out.matris[0][2] = 2 * Quarternion.vektor[0] * Quarternion.vektor[2] - 2 * Quarternion.vektor[1] * Quarternion.vektor[3];
	// out.matris[1][2] = 2 * Quarternion.vektor[1] * Quarternion.vektor[2] + 2 * Quarternion.vektor[0] * Quarternion.vektor[3];
	// out.matris[2][2] = 1 - 2 * Quarternion.vektor[0] * Quarternion.vektor[0] - 2 * Quarternion.vektor[1] * Quarternion.vektor[1];


	out = out*temp;

	out = out.transpose();

	return out;
}





























// #pragma once
// #include "Vector3DHeader.h"

// #define PI 3.14159265

// class Matrix3D
// {
// public:

// 	Matrix3D();
// 	Matrix3D FillMatrix(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

// 	Matrix3D operator*(Matrix3D obj);
// 	Vector3D operator*(Vector3D obj);

// 	Matrix3D transpose();
// 	Matrix3D rotationX(float Q);
// 	Matrix3D rotationY(float Q);
// 	Matrix3D rotationZ(float Q);
// 	Matrix3D vectorRotation(float Q, Vector3D vector);
// 	Matrix3D inverse();

// 	Matrix3D ProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar);
// 	Matrix3D ViewMatrix(const Vector3D& Eye, const Vector3D& Center, const Vector3D& Up);

// 	void ConvertToSingleList(float* ListToSaveIn[16]);

// 	float matris[4][4];
// };

// inline Matrix3D::Matrix3D() //Every matrix is created as an identity matrix
// {
// 	for (int n = 0; n < 4; n++)
// 	{
// 		for (int i = 0; i < 4; i++)
// 		{
// 			this->matris[n][i] = 0;
// 		}
// 	}

// 	this->matris[0][0] = 1;
// 	this->matris[1][1] = 1;
// 	this->matris[2][2] = 1;
// 	this->matris[3][3] = 1;
// }

// inline Matrix3D Matrix3D::FillMatrix(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44)
// {
// 	this->matris[0][0] = a11;
// 	this->matris[0][1] = a12;
// 	this->matris[0][2] = a13;
// 	this->matris[0][3] = a14;

// 	this->matris[1][0] = a21;
// 	this->matris[1][1] = a22;
// 	this->matris[1][2] = a23;
// 	this->matris[1][3] = a24;

// 	this->matris[2][0] = a31;
// 	this->matris[2][1] = a32;
// 	this->matris[2][2] = a33;
// 	this->matris[2][3] = a34;

// 	this->matris[3][0] = a41;
// 	this->matris[3][1] = a42;
// 	this->matris[3][2] = a43;
// 	this->matris[3][3] = a44;
// }

// inline Matrix3D Matrix3D::operator*(Matrix3D obj)
// {
// 	Matrix3D TheMatrix;

// 	for (int n = 0; n < 4; n++)
// 	{
// 		for (int i = 0; i < 4; i++)
// 		{
// 			TheMatrix.matris[n][i] = this->matris[i][0] * obj.matris[0][n] + this->matris[i][1] * obj.matris[1][n] + this->matris[i][2] * obj.matris[2][n] + this->matris[i][3] * obj.matris[3][n];
// 		}
// 	}

// 	return TheMatrix;
// }

// inline Vector3D Matrix3D::operator*(Vector3D obj)
// {
// 	Vector3D TheVector;

// 	for (int n = 0; n < 4; n++)
// 	{
// 		TheVector.vektor[n] = this->matris[n][0] * obj.vektor[0] + this->matris[n][1] * obj.vektor[1] + this->matris[n][2] * obj.vektor[2] + this->matris[n][3] * obj.vektor[3];
// 	}

// 	return TheVector;
// }

// inline Matrix3D Matrix3D::transpose()
// {
// 	Matrix3D TheMatrix;

// 	for (int n = 0; n < 4; n++)
// 	{
// 		for (int i = 0; i < 4; i++)
// 		{
// 			TheMatrix.matris[n][i] = this->matris[i][n];
// 		}
// 	}

// 	return TheMatrix;
// }

// inline Matrix3D Matrix3D::rotationX(float Q)
// {
// 	Matrix3D temp;

// 	temp.matris[1][1] = cos(Q*PI / 180.0f);
// 	temp.matris[1][2] = -sin(Q*PI / 180.0f);
// 	temp.matris[2][1] = sin(Q*PI / 180.0f);
// 	temp.matris[2][2] = cos(Q*PI / 180.0f);

// 	return temp;
// }

// inline Matrix3D Matrix3D::rotationY(float Q)
// {
// 	Matrix3D temp;

// 	temp.matris[0][0] = cos(Q*PI / 180.0f);
// 	temp.matris[2][0] = -sin(Q*PI / 180.0f);
// 	temp.matris[0][2] = sin(Q*PI / 180.0f);
// 	temp.matris[2][2] = cos(Q*PI / 180.0f);
	
// 	return temp;
// }

// inline Matrix3D Matrix3D::rotationZ(float Q)
// {
// 	Matrix3D temp;

// 	temp.matris[0][0] = cos(Q*PI / 180.0f);
// 	temp.matris[0][1] = -sin(Q*PI / 180.0f);
// 	temp.matris[1][0] = sin(Q*PI / 180.0f);
// 	temp.matris[1][1] = cos(Q*PI / 180.0f);

// 	return temp;
// }

// inline Matrix3D Matrix3D::vectorRotation(float Q, Vector3D r)
// {
// 	Q *= PI / 180.0f;
// 	Matrix3D temp;
// 	Vector3D tempS =r.normalisera();
	
// 	float u = tempS.vektor[0] * tempS.vektor[0];
// 	float v = tempS.vektor[1] * tempS.vektor[1];
// 	float w = tempS.vektor[2] * tempS.vektor[2];

// 	float c = cos(Q);
// 	float s = sin(Q);
// 	float t = 1 - cos(Q);
	
// 	temp.matris[0][0] = u*t + c;
// 	temp.matris[0][1] = t * tempS.vektor[0] * tempS.vektor[1] - s * tempS.vektor[2];
// 	temp.matris[0][2] = t * tempS.vektor[0] * tempS.vektor[2] + s * tempS.vektor[1];

// 	temp.matris[1][0] = t * tempS.vektor[0] * tempS.vektor[1] + s * tempS.vektor[2];
// 	temp.matris[1][1] = v*t + c;
// 	temp.matris[1][2] = t * tempS.vektor[1] * tempS.vektor[2] - s * tempS.vektor[0];

// 	temp.matris[2][0] = t * tempS.vektor[0] * tempS.vektor[2] - s * tempS.vektor[1];
// 	temp.matris[2][1] = t * tempS.vektor[1] * tempS.vektor[2] + s * tempS.vektor[0];
// 	temp.matris[2][2] = w*t + c;

// 	return temp;
// }

// inline Matrix3D Matrix3D::inverse()
// {
// 	Matrix3D noResult;
// 	Matrix3D Inverse;
// 	float determinant;

// 	float a11 = this->matris[0][0];
// 	float a12 = this->matris[0][1];
// 	float a13 = this->matris[0][2];
// 	float a14 = this->matris[0][3];

// 	float a21 = this->matris[1][0];
// 	float a22 = this->matris[1][1];
// 	float a23 = this->matris[1][2];
// 	float a24 = this->matris[1][3];

// 	float a31 = this->matris[2][0];
// 	float a32 = this->matris[2][1];
// 	float a33 = this->matris[2][2];
// 	float a34 = this->matris[2][3];

// 	float a41 = this->matris[3][0];
// 	float a42 = this->matris[3][1];
// 	float a43 = this->matris[3][2];
// 	float a44 = this->matris[3][3];

// 	determinant = a11*a22*a33*a44 + a11*a23*a34*a42 + a11*a24*a32*a43 +
// 		a12*a21*a34*a43 + a12*a23*a31*a44 + a12*a24*a33*a41 +
// 		a13*a21*a32*a44 + a13*a22*a34*a41 + a13*a24*a31*a42 +
// 		a14*a21*a33*a42 + a14*a22*a31*a43 + a14*a23*a32*a41 -

// 		a11*a22*a34*a43 + a11*a23*a32*a44 + a11*a24*a33*a42 -
// 		a12*a21*a33*a44 + a12*a23*a34*a41 + a12*a24*a31*a43 -
// 		a13*a21*a34*a42 + a13*a22*a31*a44 + a13*a24*a32*a41 -
// 		a14*a21*a32*a43 + a14*a22*a33*a41 + a14*a23*a31*a42;
	
// 	if (determinant = 0)
// 	{
// 		cout << "This matrix does not have an inverse." << endl;
// 		return noResult;
// 	}

// 	Inverse.matris[0][0] = 1 / determinant * (a22*a33*a44 + a23*a34*a42 + a24*a32*a43 - a22*a34*a43 - a23*a32*a44 - a23*a33*a42);
// 	Inverse.matris[0][1] = 1 / determinant * (a12*a34*a43 + a13*a32*a44 + a14*a33*a42 - a12*a33*a44 - a13*a34*a42 - a14*a32*a43);
// 	Inverse.matris[0][2] = 1 / determinant * (a12*a23*a44 + a13*a24*a42 + a14*a22*a43 - a12*a24*a43 - a13*a22*a44 - a14*a23*a42);
// 	Inverse.matris[0][3] = 1 / determinant * (a12*a24*a33 + a13*a22*a34 + a14*a23*a32 - a12*a23*a34 - a13*a24*a32 - a14*a22*a33);

// 	Inverse.matris[1][0] = 1 / determinant * (a21*a34*a43 + a23*a31*a44 + a24*a33*a41 - a21*a33*a44 - a23*a33*a41 - a24*a31*a43);
// 	Inverse.matris[1][1] = 1 / determinant * (a11*a33*a44 + a13*a34*a41 + a14*a31*a43 - a11*a34*a43 - a13*a31*a44 - a14*a33*a41);
// 	Inverse.matris[1][2] = 1 / determinant * (a11*a24*a43 + a13*a21*a44 + a14*a23*a41 - a11*a23*a44 - a13*a24*a41 - a14*a21*a43);
// 	Inverse.matris[1][3] = 1 / determinant * (a11*a23*a34 + a13*a24*a31 + a14*a21*a33 - a11*a24*a33 - a13*a21*a34 - a14*a23*a31);

// 	Inverse.matris[2][0] = 1 / determinant * (a21*a32*a44 + a22*a34*a41 + a24*a31*a42 - a21*a34*a42 - a22*a31*a44 - a24*a32*a41);
// 	Inverse.matris[2][1] = 1 / determinant * (a11*a34*a42 + a12*a31*a44 + a14*a32*a41 - a11*a32*a44 - a12*a34*a41 - a14*a31*a42);
// 	Inverse.matris[2][2] = 1 / determinant * (a11*a22*a44 + a12*a24*a41 + a14*a21*a42 - a11*a24*a42 - a12*a21*a44 - a14*a22*a41);
// 	Inverse.matris[2][3] = 1 / determinant * (a11*a24*a32 + a12*a21*a34 + a14*a22*a31 - a11*a22*a34 - a12*a24*a31 - a14*a21*a32);

// 	Inverse.matris[3][0] = 1 / determinant * (a21*a33*a42 + a22*a31*a43 + a23*a32*a41 - a21*a32*a43 - a22*a33*a41 - a23*a31*a42);
// 	Inverse.matris[3][1] = 1 / determinant * (a11*a32*a43 + a12*a33*a41 + a13*a31*a42 - a11*a33*a42 - a12*a31*a43 - a13*a32*a41);
// 	Inverse.matris[3][2] = 1 / determinant * (a11*a23*a42 + a12*a21*a43 + a13*a22*a41 - a11*a22*a43 - a12*a23*a41 - a13*a21*a42);
// 	Inverse.matris[3][3] = 1 / determinant * (a11*a22*a33 + a12*a23*a31 + a13*a21*a32 - a11*a23*a32 - a12*a21*a33 - a13*a22*a31);

// 	return Inverse;
// }

// inline Matrix3D Matrix3D::ProjectionMatrix(float fov, float aspectRatio, float zNear, float zFar) // Right-handed projection
// {
//     Matrix3D out;

// 	fov *= PI / 180.0f;

// 	float tanHalfFovy = tan(fov / 2.0f);
//     //float xScale = yScale / aspectRatio;

//     out.matris[0][0] = 1/(aspectRatio*tanHalfFovy);
//     out.matris[1][1] = 1/(tanHalfFovy);
//     out.matris[2][2] = - (zFar + zNear) / (zFar - zNear);
//     out.matris[2][3] = - 1;
//     out.matris[3][3] = 0;
//     out.matris[3][2] = - 2 * zNear * zFar / (zFar - zNear);

//     // float yScale = 1.0f / tan(fov / 2.0f);
//     // float xScale = yScale / aspectRatio;

//     // out.matris[0][0] = xScale;
//     // out.matris[1][1] = yScale;
//     // out.matris[2][2] = zFar / (zFar - zNear);
//     // out.matris[2][3] = 1;
//     // out.matris[3][3] = 0;
//     // out.matris[3][2] = - zNear * zFar / (zFar - zNear);

//     return out;
// }


// inline Matrix3D Matrix3D::ViewMatrix(const Vector3D& Eye, const Vector3D& Center, const Vector3D& Up)
// {
// 	Matrix3D out;

// 	Vector3D X, Y, Z;


// 	//Z = Eye - Center;

// 	 X.vektor[0] = Center.vektor[0] - Eye.vektor[0];
// 	 X.vektor[1] = Center.vektor[1] - Eye.vektor[1];
// 	 X.vektor[2] = Center.vektor[2] - Eye.vektor[2];
//      X = X.normalisera();
	 
//      Y = X.CrossProduct(Up);
// 	 Y = Y.normalisera();

//      Z = Y.CrossProduct(X);
// 	 Z = Z.normalisera();

// 	 out.matris[0][0] = Y.vektor[0];
// 	 out.matris[0][1] = Y.vektor[1];
// 	 out.matris[0][2] = Y.vektor[2];
	 
// 	 out.matris[1][0] = Z.vektor[0];
// 	 out.matris[1][1] = Z.vektor[1];
// 	 out.matris[1][2] = Z.vektor[2];

// 	 out.matris[2][0] = -X.vektor[0];
// 	 out.matris[2][1] = -X.vektor[1];
// 	 out.matris[2][2] = -X.vektor[2];

// 	 out.matris[0][3] = -Y.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
// 	 out.matris[1][3] = -Z.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
// 	 out.matris[2][3] = X.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
	 

//     // out.matris[0][0] = X.vektor[0];
//     // out.matris[0][1] = X.vektor[1];
//     // out.matris[0][2] = X.vektor[2];
//     // out.matris[0][3] = -X.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
//     // out.matris[1][0] = Y.vektor[0];
//     // out.matris[1][1] = Y.vektor[1];
//     // out.matris[1][2] = Y.vektor[2];
//     // out.matris[1][3] = -Y.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
//     // out.matris[2][0] = Z.vektor[0];
//     // out.matris[2][1] = Z.vektor[1];
//     // out.matris[2][2] = Z.vektor[2];
//     // out.matris[2][3] = -Z.punktprodukt(Eye.vektor[0], Eye.vektor[1], Eye.vektor[2]);
//     // // out.matris[3][0] = 0;
//     // // out.matris[3][1] = 0;
//     // // out.matris[3][2] = 0;
//    	// // out.matris[3][3] = 1.0f;


// 	return out;
// }


// inline void Matrix3D::ConvertToSingleList(float* ListToSaveIn[16])
// {
// 	// ListToSaveIn[0] = this->matris[0][0];
// 	// ListToSaveIn[1] = this->matris[1][0];
// 	// ListToSaveIn[2] = this->matris[2][0];
// 	// ListToSaveIn[3] = this->matris[3][0];

// 	// ListToSaveIn[4] = this->matris[0][1];
// 	// ListToSaveIn[5] = this->matris[1][1];
// 	// ListToSaveIn[6] = this->matris[2][1];
// 	// ListToSaveIn[7] = this->matris[3][1];

// 	// ListToSaveIn[8] = this->matris[0][2];
// 	// ListToSaveIn[9] = this->matris[1][2];
// 	// ListToSaveIn[10] = this->matris[2][2];
// 	// ListToSaveIn[11] = this->matris[3][2];

// 	// ListToSaveIn[12] = this->matris[0][3];
// 	// ListToSaveIn[13] = this->matris[1][3];
// 	// ListToSaveIn[14] = this->matris[2][3];
// 	// ListToSaveIn[15] = this->matris[3][3];
// }