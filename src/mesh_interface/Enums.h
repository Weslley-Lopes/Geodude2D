#pragma once

enum ConstrainedDOF
{
    X,
    Y,
    Z,
    ALL
};

enum Variable
{
    DISPLACEMENT,
    VELOCITY,
    ACCELERATION,
    POSITION,
    ROTATION,
    ANGULAR_VELOCITY,
    ANGULAR_ACCELERATION,
    CAUCHY_STRESS,
    CONTACT_FORCE,
    STRAIN_ENERGY,
    KINETIC_ENERGY,
    EXTERNAL_ENERGY,
    RIGIDITY_ENERGY,
    CONTACT_ENERGY,
    TOTAL_ENERGY,
    ALL_VARIABLES
};

enum ReferenceConfiguration
{
    INITIAL,
    PAST,
    CURRENT
};