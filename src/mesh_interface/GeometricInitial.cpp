#include "GeometricInitial.h"

GeometricInitial::GeometricInitial(const int index, const Variable variable, const ConstrainedDOF dof, const double value)
    : index_(index), variable_(variable), dof_(dof), value_(value) {}

GeometricInitial::~GeometricInitial() {}

int GeometricInitial::getIndex() const
{
    return index_;
}

Variable GeometricInitial::getVariable() const
{
    return variable_;
}

ConstrainedDOF GeometricInitial::getDegreeOfFreedom() const
{
    return dof_;
}

double GeometricInitial::getValue() const
{
    return value_;
}

GeometricPointInitial::GeometricPointInitial(const int index, Point* const point, const Variable variable, const ConstrainedDOF dof, const double value)
    : GeometricInitial(index, variable, dof, value), point_(point) {}

GeometricPointInitial::~GeometricPointInitial() {}

Point* GeometricPointInitial::getPoint() const
{
    return point_;
}

Line* GeometricPointInitial::getLine() const
{
    return nullptr;
}

Surface* GeometricPointInitial::getSurface() const
{
    return nullptr;
}

GeometricLineInitial::GeometricLineInitial(const int index, Line* const line, const Variable variable, const ConstrainedDOF dof, const double value)
    : GeometricInitial(index, variable, dof, value), line_(line) {}

GeometricLineInitial::~GeometricLineInitial() {}

Point* GeometricLineInitial::getPoint() const
{
    return nullptr;
}

Line* GeometricLineInitial::getLine() const
{
    return line_;
}

Surface* GeometricLineInitial::getSurface() const
{
    return nullptr;
}

GeometricSurfaceInitial::GeometricSurfaceInitial(const int index, Surface* const surface, const Variable variable, const ConstrainedDOF dof, const double value)
    : GeometricInitial(index, variable, dof, value), surface_(surface) {}

GeometricSurfaceInitial::~GeometricSurfaceInitial() {}

Point* GeometricSurfaceInitial::getPoint() const
{
    return nullptr;
}

Line* GeometricSurfaceInitial::getLine() const
{
    return nullptr;
}

Surface* GeometricSurfaceInitial::getSurface() const
{
    return surface_;
}