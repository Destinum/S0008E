#pragma once
#include <iostream>
#include "math.h"
using namespace std;

class Vector3D
{
public:
	Vector3D(float, float, float, float);
	Vector3D();
	Vector3D operator+(const Vector3D &obj);
	Vector3D operator-(const Vector3D &obj);
	Vector3D operator*(const Vector3D &obj);
	Vector3D operator*(float);
	void operator=(const Vector3D &obj);
	void addition(float, float, float);
	void subtraktion(float, float, float);
	float punktprodukt(float, float, float);
	float langd();
	Vector3D normalisera();
	void set(float, float, float);
	Vector3D CrossProduct(Vector3D);

	float vektor[4];
};

inline Vector3D::Vector3D(float x, float y, float z, float w)			//constructor for given values.
{
	this->vektor[0] = x;
	this->vektor[1] = y;
	this->vektor[2] = z;
	this->vektor[3] = w;
}

inline Vector3D::Vector3D()			//Constructor for no given values.
{
	this->vektor[0] = 0;
	this->vektor[1] = 0;
	this->vektor[2] = 0;
	this->vektor[3] = 1;
}

inline Vector3D Vector3D::operator+(const Vector3D &obj) //Addition operator between two vectors.
{
	Vector3D temp;

	temp.vektor[0] = this->vektor[0] + obj.vektor[0];
	temp.vektor[1] = this->vektor[1] + obj.vektor[1];
	temp.vektor[2] = this->vektor[2] + obj.vektor[2];
	temp.vektor[3] = this->vektor[3];

	return temp;
}

inline Vector3D Vector3D::operator-(const Vector3D &obj) //Subtraktion operator between two vektors.
{
	Vector3D temp;

	temp.vektor[0] = this->vektor[0] - obj.vektor[0];
	temp.vektor[1] = this->vektor[1] - obj.vektor[1];
	temp.vektor[2] = this->vektor[2] - obj.vektor[2];
	temp.vektor[3] = this->vektor[3];

	return temp;
}

inline Vector3D Vector3D::operator*(const Vector3D &obj) //Multiplication operator between two vektors.
{
	Vector3D temp;

	temp.vektor[0] = this->vektor[0] * obj.vektor[0];
	temp.vektor[1] = this->vektor[1] * obj.vektor[1];
	temp.vektor[2] = this->vektor[2] * obj.vektor[2];
	temp.vektor[3] = this->vektor[3] * obj.vektor[3];

	return temp;
}

inline Vector3D Vector3D::operator*(float scalar) //Multiplication operator between vector and a scalar.
{
	//this->vektor[3] *= scalar;

	Vector3D temp;

	temp.vektor[0] = this->vektor[0] * scalar;
	temp.vektor[1] = this->vektor[1] * scalar;
	temp.vektor[2] = this->vektor[2] * scalar;
	temp.vektor[3] = this->vektor[3];

	return temp;
}

inline void Vector3D::operator=(const Vector3D &obj) //Operator for giving a vector the values of another vector.
{
	this->vektor[0] = obj.vektor[0];
	this->vektor[1] = obj.vektor[1];
	this->vektor[2] = obj.vektor[2];
	this->vektor[3] = obj.vektor[3];
}


inline void Vector3D::addition(float x, float y, float z) //Adding given values to a vector.
{
	this->vektor[0] += x;
	this->vektor[1] += y;
	this->vektor[2] += z;
}

inline void Vector3D::subtraktion(float x, float y, float z) //Substracting given values from a vector.
{
	this->vektor[0] -= x;
	this->vektor[1] -= y;
	this->vektor[2] -= z;
}

inline float Vector3D::punktprodukt(float x, float y, float z) //Calculates the dot product of two vectors (the second give in the form of values).
{
	return (this->vektor[0] * x) + (this->vektor[1] * y) + (this->vektor[2] * z);
}

inline float Vector3D::langd() //Calculates the lenght of a vector.
{
	return sqrt(this->vektor[0] * this->vektor[0] + this->vektor[1] * this->vektor[1] + this->vektor[2] * this->vektor[2]);
}

inline Vector3D Vector3D::normalisera() //Returns the normalized version of a given vector.
{
	Vector3D v(this->vektor[0] / this->langd(), this->vektor[1] / this->langd(), this->vektor[2] / this->langd(), this->vektor[3]);

	return v;
}

inline void Vector3D::set(float x, float y, float z) //Set values of a vector.
{
	this->vektor[0] = x;
	this->vektor[1] = y;
	this->vektor[2] = z;
}

inline Vector3D Vector3D::CrossProduct(Vector3D Vector) //Calculates the cross product of two given vectors.
{
	// Vector3D Returnable(
	// 		this->vektor[3] * Vector.vektor[3] - this->vektor[0] * Vector.vektor[0] - this->vektor[1] * Vector.vektor[1] - this->vektor[2] * Vector.vektor[2],
	// 		this->vektor[3] * Vector.vektor[0] + this->vektor[0] * Vector.vektor[3] + this->vektor[1] * Vector.vektor[2] - this->vektor[2] * Vector.vektor[1],
	// 		this->vektor[3] * Vector.vektor[1] + this->vektor[1] * Vector.vektor[3] + this->vektor[2] * Vector.vektor[0] - this->vektor[0] * Vector.vektor[2],
	// 		this->vektor[3] * Vector.vektor[2] + this->vektor[2] * Vector.vektor[3] + this->vektor[0] * Vector.vektor[1] - this->vektor[1] * Vector.vektor[0]
	// );

	Vector3D Returnable;

	Returnable.vektor[0] = this->vektor[1] * Vector.vektor[2] - this->vektor[2] * Vector.vektor[1];
	Returnable.vektor[1] = this->vektor[2] * Vector.vektor[0] - this->vektor[0] * Vector.vektor[2];
	Returnable.vektor[2] = this->vektor[0] * Vector.vektor[1] - this->vektor[1] * Vector.vektor[0];

	return Returnable;
}