#pragma once
#include "ParametricElement.h"

class ParametricLineElement : public ParametricElement
{
    public:
    ParametricLineElement(const PartitionOfUnity elementType);

    ~ParametricLineElement();

    void setNodalParametricCoordinates() override;

    void getShapeFunctions(double* xsi, double*& phi) const override;

    void getShapeFunctionsDerivatives(double* xsi, double**& dphi_dxsi) const override;

    void getShapeFunctionsSecondDerivatives(double* xsi, double**& d2phi_dxsi2) const override; 

    static ParametricLineElement L2;

    static ParametricLineElement L3;

    static ParametricLineElement L4;
};