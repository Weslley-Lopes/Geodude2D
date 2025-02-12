#pragma once
#include "Node.h"

class InitialCondition
{
    private:
        Node* node_;
        DegreeOfFreedom* dof_;
        double value_;

    public:

        InitialCondition(Node* const node, DegreeOfFreedom* const dof, const double value);

        ~InitialCondition();

        Node* getNode() const;

        DegreeOfFreedom* getDegreeOfFreedom() const;

        double getValue() const;
};