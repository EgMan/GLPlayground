#pragma once
#include "stdafx.h"
#include "Vector3D.cpp"
class Rigidbody
{
private:
	const double bounceCeficient = 0.9;
public:
	Vector3D * position, * velocity, * acceleration;
	Rigidbody(Vector3D * initialPosition, Vector3D * initialVelocity):
		position(initialPosition),
		velocity(initialVelocity),
		acceleration(new Vector3D())
	{}
	void tick() {
		*velocity = *velocity + *acceleration;
		*position = *position + *velocity;
		acceleration->zeroOut();
	}
	void applyForce(double x, double y, double z)
	{
		acceleration->x = x;
		acceleration->y = y;
		acceleration->z = z;
	}
	double getDisgtanceTo(Rigidbody other)
	{
		return (*position - *other.position).abs();
	}
	void collide(Vector3D normalVect)
	{
		Vector3D normalVectNormalized = normalVect.normalize();
		//velocity = &(((normalVect * (2 * (*velocity * normalVect))) - *velocity) * -bounceCeficient);
		//velocity = &(((normalVect.scalarProd(2 * (velocity->dot(normalVect)))) - *velocity) * -bounceCeficient);
		*velocity = (normalVectNormalized.scalarProd( 2.0 * velocity->dot(normalVectNormalized)).operator -(*velocity)).scalarProd(-bounceCeficient);
	}
};

