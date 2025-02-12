#pragma once
#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Enums.h"

class GeometricInitial
{
    protected:
        int index_;
        Variable variable_;
        ConstrainedDOF dof_;
        double value_;

    public:

        GeometricInitial(const int index, const Variable variable, const ConstrainedDOF dof, const double value);

        virtual ~GeometricInitial() = 0;

        int getIndex() const;

        Variable getVariable() const;

        ConstrainedDOF getDegreeOfFreedom() const;

        double getValue() const;

        virtual Point* getPoint() const = 0;

        virtual Line* getLine() const = 0;

        virtual Surface* getSurface() const = 0;
};

class GeometricPointInitial : public GeometricInitial
{
    private:
        Point* point_;

    public:

        GeometricPointInitial(const int index, Point* const point, const Variable variable, const ConstrainedDOF dof, const double value);

        ~GeometricPointInitial() override;

        Point* getPoint() const override;

        Line* getLine() const override;

        Surface* getSurface() const override;
};

class GeometricLineInitial : public GeometricInitial
{
    private:
        Line* line_;

    public:

        GeometricLineInitial(const int index, Line* const line, const Variable variable, const ConstrainedDOF dof, const double value);

        ~GeometricLineInitial() override;

        Point* getPoint() const override;

        Line* getLine() const override;

        Surface* getSurface() const override;
};

class GeometricSurfaceInitial : public GeometricInitial
{
    private:
        Surface* surface_;

    public:

        GeometricSurfaceInitial(const int index, Surface* const surface, const Variable variable, const ConstrainedDOF dof, const double value);

        ~GeometricSurfaceInitial() override;

        Point* getPoint() const override;

        Line* getLine() const override;

        Surface* getSurface() const override;
};