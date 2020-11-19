#include "stdafx.h"
#include "Rigidbody.cpp"
#include "SphereCollider.cpp"
#include <vector>
#include <tuple>
#include <iostream>

class PhysicsSystem
{
private:
	std::vector<std::tuple<Rigidbody *, SphereCollider *>> objects = {};
public:
	PhysicsSystem()
	{}
	void addRigidSphere(Rigidbody * rb, SphereCollider * sphere)
	{
		objects.push_back(std::make_tuple(rb, sphere));
	}
	void tick()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			std::tuple<Rigidbody*, SphereCollider*> obj = objects[i];
			Rigidbody * r = std::get<0>(obj);
			r->tick();
		}
		for (int i = 0; i < objects.size(); i++)
		{
			auto obj1 = objects[i];
			Rigidbody* rb1 = std::get<0>(obj1);
			SphereCollider* sc1 = std::get<1>(obj1);
			if (rb1->position->y < sc1->radius)
			{
				rb1->position->y = sc1->radius;
				rb1->collide(Vector3D(0,3,0));
			}
			for (int j = i; j < objects.size(); j++)
			{
				if (i == j) continue;
				auto obj2 = objects[j];
				Rigidbody * rb2 = std::get<0>(obj2);

				SphereCollider * sc2 = std::get<1>(obj2);

				// check for collisions
				double dist = rb1->getDisgtanceTo(*rb2);
				if (dist < sc1->radius + sc2->radius)
				{
					Vector3D normal = rb1->position->operator-(*rb2->position);
					Vector3D normalizedNormal = normal.normalize();
					double overlapDistance = sc1->radius + sc2->radius - normal.abs();
					//rb1->position = &rb1->position->operator+(normalizedNormal.scalarProd(overlapDistance / 2));
					//rb2->position = &rb2->position->operator+(normalizedNormal.scalarProd(overlapDistance / -2));
					rb1->collide(normal);
					rb2->collide(normal.scalarProd(-1));
				}
			}
		}
	}
};