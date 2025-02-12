#pragma once
#include <string>
#include "mesh_interface/Enums.h"
#include "Node.h"

class OutputGraphic
{
public:
    OutputGraphic(const std::string fileName, Variable variable, ConstrainedDOF direction, Node *const &node);
    // OutputGraphic(Variable variable, ConstrainedDOF direction, const int &particle, const int &node = 0);
    OutputGraphic(Variable variable, ConstrainedDOF direction, const int &particle);
    OutputGraphic(Variable variable, const int &particle);
    ~OutputGraphic();

    void setXMax(const double max);
    void setYMax(const double max);
    void setXMin(const double min);
    void setYMin(const double min);

    std::string getFileName();
    Variable getVariable();
    ConstrainedDOF getConstrainedDOF();
    Node *getNode();
    int getParticle() const;
    int getNodeIndex() const;
    double getXMax() const;
    double getYMax() const;
    double getXMin() const;
    double getYMin() const;

private:
    std::string fileName_;
    Variable variable_;
    ConstrainedDOF direction_;
    Node *node_;
    int particle_;
    int nodeIndex_;
    double xMax_;
    double yMax_;
    double xMin_;
    double yMin_;
};