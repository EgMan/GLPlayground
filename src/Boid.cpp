#pragma once
#include "stdafx.h"
#include "Vector3D.cpp"
#include <vector>

class Boid
{
public:
	Vector3D velocity, position;
	Boid(Vector3D position, Vector3D velocity) :
		position(position),
		velocity(velocity)
	{}

	void tick(std::vector<Boid *> others, Vector3D boundaryCenter, double boundaryRadius)
	{
		double distFromCenter = (position - boundaryCenter).abs();
		if (distFromCenter > boundaryRadius)
		{
			velocity = turnTowards(boundaryCenter - position, .2);
		}
		else if (others.size() > 0)
		{
			Vector3D averagePosition = Vector3D(0, 0, 0);
			Vector3D averageHeading = Vector3D(0, 0, 0);
			Vector3D averageAwayHeading = Vector3D(0, 0, 0);
			Boid* closestNeighbor = others[0];
			double closestNeighborDist = (closestNeighbor->position - position).abs();
			for (Boid* b : others)
			{
				double dist = (b->position - position).abs();
				averageAwayHeading += (position - b->position).normalize().scalarProd(1/dist);
				averagePosition += b->position;
				averageHeading += b->velocity;

				if (dist < closestNeighborDist)
				{
					closestNeighbor = b;
					closestNeighborDist = dist;
				}
			}
			averagePosition = averagePosition.scalarProd(1.0 / others.size());

			// separation: steer to avoid crowding local flockmates
			Vector3D separation = turnTowards(averageAwayHeading, .2f);

			// alignment : steer towards the average heading of local flockmates
			Vector3D alignment = turnTowards(averageHeading, .08f);

			// cohesion : steer to move towards the average position(center of mass) of local flockmates
			Vector3D cohesion = turnTowards(averagePosition - position, .15f);

			// combine above forces so none takes priority over the other
			velocity = combineTurns(separation, alignment, cohesion).scalarProd(velocity.abs());

			// attraction to the center
			velocity = turnTowards(boundaryCenter - position, distFromCenter / 100.0);
		}
		position += velocity;
	}

	Vector3D turnTowards(Vector3D direction, float speed)
    {
		Vector3D avg = velocity.normalize() + direction.normalize().scalarProd(speed);
		if (avg.abs() == 0.0) return direction.normalize();
		return (avg).normalize().scalarProd(velocity.abs());
    }

	Vector3D turnAway(Vector3D direction, float speed)
	{
		return turnTowards(direction.scalarProd(-1), speed);
	}

	Vector3D combineTurns(Vector3D separation, Vector3D alignment, Vector3D cohesion)
	{
		return (separation + alignment + cohesion).normalize();
	}
};
