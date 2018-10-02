#pragma once
#include <iostream>
#include "math.h"
using namespace std;

class Vector2D
{
public:
	Vector2D(float, float);
	Vector2D();
	Vector2D(const Vector2D &obj);
	~Vector2D(void);
	Vector2D operator+(const Vector2D &obj);
	Vector2D operator-(const Vector2D &obj);
	float operator*(const Vector2D &obj);
	void operator=(const Vector2D &obj);
	bool operator==(const Vector2D &obj);
	bool operator!=(const Vector2D &obj);
	void addition(float, float);
	void subtraktion(float, float);
	float punktprodukt(float, float);
	float langd();
	Vector2D normalisera();
	float getX();
	float getY();
	void set(float,float);

	float vektor[2];
};

inline Vector2D::Vector2D(float x, float y)			//Konstruktor f�r samtliga komponenter.
{
	this->vektor[0] = x;
	this-> vektor[1] = y;
}

inline Vector2D::Vector2D()			//Konstruktor f�r inga komponenter.
{
}

inline Vector2D::Vector2D(const Vector2D &obj)			//Konstruktor f�r kopiering.
{
	this->vektor[0] = obj.vektor[0];
	this-> vektor[1] = obj.vektor[1];
}

inline Vector2D::~Vector2D(void)
{
}

inline Vector2D Vector2D::operator+(const Vector2D &obj)
{
	Vector2D v(this->vektor[0] + obj.vektor[0], this->vektor[1] + obj.vektor[1]);
	return v;
}

inline Vector2D Vector2D::operator-(const Vector2D &obj)
{
	Vector2D v(this->vektor[0] - obj.vektor[0], this->vektor[1] - obj.vektor[1]);
	return v;
}

inline float Vector2D::operator*(const Vector2D &obj)
{
	return (this->vektor[0] * obj.vektor[0]) + (this->vektor[1] * obj.vektor[1]);
}

inline void Vector2D::operator=(const Vector2D &obj)
{
	this->vektor[0] = obj.vektor[0];
	this->vektor[1] = obj.vektor[1];
}

inline bool Vector2D::operator==(const Vector2D &obj)
{
	return (this->vektor[0] == obj.vektor[0] && this->vektor[1] == obj.vektor[1]);
}

inline bool Vector2D::operator!=(const Vector2D &obj)
{
	return !(this->vektor[0] == obj.vektor[0] && this->vektor[1] == obj.vektor[1]);
}

inline void Vector2D::addition(float x, float y)
{
	this->vektor[0] += x;
	this->vektor[1] += y;
}

inline void Vector2D::subtraktion(float x, float y)
{
	this->vektor[0] -= x;
	this->vektor[1] -= y;
}

inline float Vector2D::punktprodukt(float x, float y)
{
	return (this->vektor[0] * x) + (this->vektor[1] * y);
}

inline float Vector2D::langd()
{
	return sqrt(this->vektor[0]*this->vektor[0] + this->vektor[1]*this->vektor[1]);
}

inline Vector2D Vector2D::normalisera()
{
	 Vector2D v(this->vektor[0]/this->langd(), this->vektor[1]/this->langd());

	 return v;
}

inline float Vector2D::getX()
{
	return this->vektor[0];
}

inline float Vector2D::getY()
{
	return this->vektor[1];
}

inline void Vector2D::set(float x,float y)
{
	this->vektor[0] = x;
	this->vektor[1] = y;
}
