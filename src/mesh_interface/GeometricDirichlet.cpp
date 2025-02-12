#include "GeometricDirichlet.h"

GeometricDirichlet::GeometricDirichlet(const int index, const Variable variable,  const ConstrainedDOF dof, const double value)
    : index_(index), variable_(variable), dof_(dof), value_(value) {}

GeometricDirichlet::~GeometricDirichlet() {}

int GeometricDirichlet::getIndex() const
{
    return index_;
}

Variable GeometricDirichlet::getVariable() const
{
    return variable_;
}

ConstrainedDOF GeometricDirichlet::getDegreeOfFreedom() const
{
    return dof_;
}

double GeometricDirichlet::getValue() const
{
    return value_;
}

GeometricPointDirichlet::GeometricPointDirichlet(const int index, Point* const point, const Variable variable,  const ConstrainedDOF dof, const double value)
    : GeometricDirichlet(index, variable, dof, value), point_(point) {}

GeometricPointDirichlet::~GeometricPointDirichlet() {}

Point* GeometricPointDirichlet::getPoint() const
{
    return point_;
}

Line* GeometricPointDirichlet::getLine() const
{
    return nullptr;
}

Surface* GeometricPointDirichlet::getSurface() const
{
    return nullptr;
}

GeometricLineDirichlet::GeometricLineDirichlet(const int index, Line* const line, const Variable variable,  const ConstrainedDOF dof, const double value)
    : GeometricDirichlet(index, variable,  dof, value), line_(line) {}

GeometricLineDirichlet::~GeometricLineDirichlet() {}

Point* GeometricLineDirichlet::getPoint() const
{
    return nullptr;
}

Line* GeometricLineDirichlet::getLine() const
{
    return line_;
}

Surface* GeometricLineDirichlet::getSurface() const
{
    return nullptr;
}

GeometricSurfaceDirichlet::GeometricSurfaceDirichlet(const int index, Surface* const surface, const Variable variable,  const ConstrainedDOF dof, const double value)
    : GeometricDirichlet(index, variable,  dof, value), surface_(surface) {}

GeometricSurfaceDirichlet::~GeometricSurfaceDirichlet() {}

Point* GeometricSurfaceDirichlet::getPoint() const
{
    return nullptr;
}

Line* GeometricSurfaceDirichlet::getLine() const
{
    return nullptr;
}

Surface* GeometricSurfaceDirichlet::getSurface() const
{
    return surface_;
}