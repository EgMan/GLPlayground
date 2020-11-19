#pragma once
#include "stdafx.h"
class Vector3D
{
public:
    double x, y, z;
    Vector3D(double x = 0.0, double y = 0.0, double z = 0.0) :
        x(x),
        y(y),
        z(z)
    {}

    Vector3D operator + (Vector3D const& other)
    {
        Vector3D result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        return result;
    }

    void operator += (Vector3D const& other)
    {
        x = x + other.x;
        y = y + other.y;
        z = z + other.z;
    }

    Vector3D operator - (Vector3D const& other)
    {
        Vector3D result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;
        return result;
    }

    double dot(Vector3D const& other)
    {
        double result = 0.0;
        result += x * other.x;
        result += y * other.y;
        result += z * other.z;
        return result;
    }

    Vector3D scalarProd(double const& other)
    {
        Vector3D result;
        result.x = x * other;
        result.y = y * other;
        result.z = z * other;
        return result;
    }

    double abs()
    {
        return pow(pow(x, 2) + pow(y, 2) + pow(z, 2), 0.5);
    }

    void zeroOut()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    Vector3D normalize()
    {
        Vector3D result;
        double abs = this->abs();
        result.x = x / abs;
        result.y = y / abs;
        result.z = z / abs;
        return result;
    }
};

