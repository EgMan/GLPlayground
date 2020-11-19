#pragma once
#include "stdafx.h"
#include "Boid.cpp"

#include <vector>


class BoidSystem
{
private:
    double viewRadius;
    Vector3D boundaryCenter;
    double boundaryRadius;
public:
    std::vector<Boid> boids = {};

    BoidSystem(Vector3D boundaryCenter, double boundaryRadius, double viewRadius):
        boundaryCenter(boundaryCenter),
        boundaryRadius(boundaryRadius),
        viewRadius(viewRadius)
    {}

    void addBoid(Vector3D position, Vector3D velocity)
    {
        boids.push_back(*new Boid(position, velocity));
    }
    void tick()
    {
        std::vector<std::vector<Boid *>> connectivity;
        for (int ii = 0; ii < boids.size(); ii++)
        {
            connectivity.push_back(std::vector<Boid *>());
        }
        for (int i = 0; i < boids.size(); i++)
        {
            for (int j = i + 1; j < boids.size(); j++)
            {
                if ((boids[i].position - boids[j].position).abs() < viewRadius)
                {
                    connectivity[i].push_back(& boids[j]);
                    connectivity[j].push_back(& boids[i]);

                }
            }
        }
        for (int jj = 0; jj < boids.size(); jj++)
        {
            boids[jj].tick(connectivity[jj], boundaryCenter, boundaryRadius);
        }
    }
};
