#include "OutputGraphic.h"

OutputGraphic::OutputGraphic(std::string fileName,
                             Variable variable,
                             ConstrainedDOF direction,
                             Node *const &node)
    : fileName_(fileName),
      variable_(variable),
      direction_(direction),
      node_(node)
{
}

// OutputGraphic::OutputGraphic(Variable variable,
//                              ConstrainedDOF direction,
//                              const int &particle,
//                              const int &node)
//     : variable_(variable),
//       direction_(direction),
//       particle_(particle),
//       nodeIndex_(node),
//       xMax_(0.0),
//       yMax_(0.0),
//       xMin_(1.e10),
//       yMin_(1.e10)
// {
//     std::string var, dir;
//     switch (variable)
//     {
//     case POSITION:
//         var = "pos_";
//         break;
//     case VELOCITY:
//         var = "vel_";
//         break;
//     case ACCELERATION:
//         var = "accel_";
//         break;
//     case CONTACT_FORCE:
//         var = "force_";
//         break;
//     default:
//         break;
//     }
//     switch (direction)
//     {
//     case X:
//         dir = "x_";
//         break;
//     case Y:
//         dir = "y_";
//         break;
//     case Z:
//         dir = "z_";
//         break;
//     default:
//         break;
//     }
//     fileName_ = "particle_" + std::to_string(particle) + "_" + var + dir + "node_" + std::to_string(node);
// }

OutputGraphic::OutputGraphic(Variable variable,
                             ConstrainedDOF direction,
                             const int &particle)
    : variable_(variable),
      direction_(direction),
      particle_(particle),
      xMax_(0.0),
      yMax_(0.0),
      xMin_(1.e10),
      yMin_(1.e10)
{
    std::string var, dir;
    switch (variable)
    {
    case POSITION:
        var = "pos_";
        break;
    case VELOCITY:
        var = "vel_";
        break;
    case ACCELERATION:
        var = "accel_";
        break;
    case ROTATION:
        var = "rot_";
        break;
    case ANGULAR_VELOCITY:
        var = "ang_vel_";
        break;
    case ANGULAR_ACCELERATION:
        var = "ang_accel_";
        break;
    case CONTACT_FORCE:
        var = "force_";
        break;
    default:
        break;
    }
    switch (direction)
    {
    case X:
        dir = "x_";
        break;
    case Y:
        dir = "y_";
        break;
    case Z:
        dir = "z_";
        break;
    default:
        break;
    }
    fileName_ = "particle_" + std::to_string(particle) + "_" + var + dir;
}

OutputGraphic::OutputGraphic(Variable variable,
                             const int &particle)
    : variable_(variable),
      particle_(particle),
      xMax_(0.0),
      yMax_(0.0),
      xMin_(1.e10),
      yMin_(1.e10)
{
    std::string var, dir;
    switch (variable)
    {
    case STRAIN_ENERGY:
        fileName_ = "particle_" + std::to_string(particle) + "_" + "strain_energy_";
        break;
    case KINETIC_ENERGY:
        fileName_ = "particle_" + std::to_string(particle) + "_" + "kinetic_energy_";
        break;
    case EXTERNAL_ENERGY:
        fileName_ = "particle_" + std::to_string(particle) + "_" + "external_energy_";
        break;
    case RIGIDITY_ENERGY:
        fileName_ = "particle_" + std::to_string(particle) + "_" + "rigidity_energy_";
        break;
    case CONTACT_ENERGY:
        fileName_ = "particle_" + std::to_string(particle) + "_" + "contact_energy_";
        break;
    case TOTAL_ENERGY:
        fileName_ = "particle_" + std::to_string(particle) + "_" + "total_energy_";
        break;
    default:
        break;
    }
}

OutputGraphic::~OutputGraphic()
{
}

void OutputGraphic::setXMax(const double max)
{
    xMax_ = max;
}

void OutputGraphic::setYMax(const double max)
{
    yMax_ = max;
}

void OutputGraphic::setXMin(const double min)
{
    xMin_ = min;
}

void OutputGraphic::setYMin(const double min)
{
    yMin_ = min;
}

std::string OutputGraphic::getFileName()
{
    return fileName_;
}

Variable OutputGraphic::getVariable()
{
    return variable_;
}

ConstrainedDOF OutputGraphic::getConstrainedDOF()
{
    return direction_;
}

Node *OutputGraphic::getNode()
{
    return node_;
}

int OutputGraphic::getParticle() const
{
    return particle_;
}

int OutputGraphic::getNodeIndex() const
{
    return nodeIndex_;
}

double OutputGraphic::getXMax() const
{
    return xMax_;
}

double OutputGraphic::getYMax() const
{
    return yMax_;
}

double OutputGraphic::getXMin() const
{
    return xMin_;
}

double OutputGraphic::getYMin() const
{
    return yMin_;
}
