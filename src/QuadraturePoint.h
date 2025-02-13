#pragma once
#include <vector>

class QuadraturePoint
{
private:
    double *xsi_;
    double *phi_;
    double **dphi_dxsi_;
    // double** d2phi_dxsi2_;
    double weight_;
    int dimension_;
    int numberOfNodes_;

public:
    QuadraturePoint(double *xsi,
                    double *phi,
                    double **dphi_dxsi,
                    // double** d2phi_dxsi2,
                    const double weight,
                    const int dimension,
                    const int numberOfNodes);

    ~QuadraturePoint();

    double *getParametricCoordinates() const;

    double *getShapeFunctionsValues() const;

    double **getShapeFunctionsDerivativesValues() const;

    double getWeight() const;

    int getDimension() const;

    int getNumberOfNodes() const;
};