#include "InitialCondition.h"

InitialCondition::InitialCondition(Node* const node, DegreeOfFreedom* const dof, const double value)
    : node_(node), dof_(dof), value_(value) {}

InitialCondition::~InitialCondition() {}

Node* InitialCondition::getNode() const
{
    return node_;
}

DegreeOfFreedom* InitialCondition::getDegreeOfFreedom() const
{
    return dof_;
}

double InitialCondition::getValue() const
{
    return value_;
}