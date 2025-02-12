#include "Material.h"

Material::Material(const double &density,
                   const MaterialType &type,
                   const PlaneAnalysis &planeAnalysis)
    : density_(density),
      type_(type),
      planeAnalysis_(planeAnalysis) {}

double Material::getDensity() const
{
    return density_;
}

MaterialType Material::getType() const
{
    return type_;
}

PlaneAnalysis Material::getPlaneAnalysis() const
{
    return planeAnalysis_;
}

void Material::setDensity(const double &density)
{
    density_ = density;
}

void Material::setType(const MaterialType &type)
{
    type_ = type;
}

void Material::setPlaneAnalysis(const PlaneAnalysis &planeAnalysis)
{
    planeAnalysis_ = planeAnalysis;
}

ElasticSolid::ElasticSolid(const ConstitutiveModel &model,
                           const double &young,
                           const double &poisson,
                           const double &density)
    : Material(density, MaterialType::ELASTIC_SOLID),
      model_(model),
      young_(young),
      poisson_(poisson) {}

ElasticSolid::~ElasticSolid() {}

void ElasticSolid::setYoung(const double &young)
{
    young_ = young;
}

void ElasticSolid::setPoisson(const double &poisson)
{
    poisson_ = poisson;
}

double ElasticSolid::getYoung() const
{
    return young_;
}

double ElasticSolid::getPoisson() const
{
    return poisson_;
}

void ElasticSolid::getPlaneStressTensor(const double E[3],
                                        const double CI[2][2],
                                        double S[3]) const
{
    switch (model_)
    {
    case SAINT_VENANT_KIRCHHOFF:
    {
        switch (planeAnalysis_)
        {
        case PLANE_STRAIN:
        {
            const double prop1 = young_ / ((1.0 + poisson_) * (1.0 - 2.0 * poisson_));
            const double prop2 = 1.0 - poisson_;
            const double prop3 = young_ / (1.0 + poisson_);
            S[0] = prop1 * (prop2 * E[0] + poisson_ * E[1]); // S[0][0]
            S[1] = prop1 * (prop2 * E[1] + poisson_ * E[0]); // S[1][1]
            S[2] = prop3 * E[2];                             // S[0][1]
            break;
        }
        case PLANE_STRESS:
        {
            const double prop1 = young_ / (1.0 - (poisson_ * poisson_));
            const double prop3 = young_ / (1.0 + poisson_);
            S[0] = prop1 * (E[0] + poisson_ * E[1]); // S[0][0]
            S[1] = prop1 * (E[1] + poisson_ * E[0]); // S[1][1]
            S[2] = prop3 * E[2];                     // S[0][1]
            break;
        }
        }
        break;
    }

    case NEO_HOOKEAN:
    {
        // Plane strain case
        double C[2][2] = {};
        C[0][0] = 2. * E[0] + 1.0;
        C[0][1] = 2. * E[2];
        C[1][0] = 2. * E[2];
        C[1][1] = 2. * E[1] + 1.0;
        const double prop1 = young_ / (2.0 * (1.0 + poisson_));                               // G or mi
        const double prop2 = poisson_ * young_ / ((1.0 + poisson_) * (1.0 - 2.0 * poisson_)); // lambda = Lamé constant
        const double prop3 = sqrt(C[0][0] * C[1][1] - C[0][1] * C[1][0]);                     // Jacobian determinant

        S[0] = prop1 * (1.0 - CI[0][0]) + prop2 * log(prop3) * CI[0][0]; // S[0][0]
        S[1] = prop1 * (1.0 - CI[1][1]) + prop2 * log(prop3) * CI[1][1]; // S[1][1]
        S[2] = prop1 * (0.0 - CI[0][1]) + prop2 * log(prop3) * CI[0][1]; // S[0][1]
    }

    default:
        break;
    }
}

void ElasticSolid::getPlaneStressTensorDerivative(const double dE_dy[3],
                                                  const double dx_dy[2][2],
                                                  double dS_dy[3]) const
{
    switch (planeAnalysis_)
    {
    case PLANE_STRAIN:
    {
        const double prop1 = young_ / ((1.0 + poisson_) * (1.0 - 2.0 * poisson_));
        const double prop2 = 1.0 - poisson_;
        const double prop3 = young_ / (1.0 + poisson_);
        dS_dy[0] = prop1 * (prop2 * dE_dy[0] + poisson_ * dE_dy[1]); // S[0][0]
        dS_dy[1] = prop1 * (prop2 * dE_dy[1] + poisson_ * dE_dy[0]); // S[1][1]
        dS_dy[2] = prop3 * dE_dy[2];                                 // S[0][1]
        break;
    }
    case PLANE_STRESS:
    {
        const double prop1 = young_ / (1.0 - (poisson_ * poisson_));
        const double prop3 = young_ / (1.0 + poisson_);
        dS_dy[0] = prop1 * (dE_dy[0] + poisson_ * dE_dy[1]); // S[0][0]
        dS_dy[1] = prop1 * (dE_dy[1] + poisson_ * dE_dy[0]); // S[1][1]
        dS_dy[2] = prop3 * dE_dy[2];                         // S[0][1]
        break;
    }
    }
}

void ElasticSolid::getPlaneStressTensorDerivative(int ndofs,
                                                  const double dE_dy[][3],
                                                  const double dx_dy[2][2],
                                                  double dS_dy[][3]) const
{
    switch (planeAnalysis_)
    {
    case PLANE_STRAIN:
    {
        const double prop1 = young_ / ((1.0 + poisson_) * (1.0 - 2.0 * poisson_));
        const double prop2 = 1.0 - poisson_;
        const double prop3 = young_ / (1.0 + poisson_);
        for (unsigned int i = 0; i < ndofs; i++)
        {
            dS_dy[i][0] = prop1 * (prop2 * dE_dy[i][0] + poisson_ * dE_dy[i][1]); // dS_dy[0][0]
            dS_dy[i][1] = prop1 * (prop2 * dE_dy[i][1] + poisson_ * dE_dy[i][0]); // dS_dy[1][1]
            dS_dy[i][2] = prop3 * dE_dy[i][2];                                    // dS_dy[0][1]
        }
        break;
    }
    case PLANE_STRESS:
    {
        const double prop1 = young_ / (1.0 - (poisson_ * poisson_));
        const double prop3 = young_ / (1.0 + poisson_);
        for (unsigned int i = 0; i < ndofs; i++)
        {
            dS_dy[i][0] = prop1 * (dE_dy[i][0] + poisson_ * dE_dy[i][1]); // dS_dy[0][0]
            dS_dy[i][1] = prop1 * (dE_dy[i][1] + poisson_ * dE_dy[i][0]); // dS_dy[1][1]
            dS_dy[i][2] = prop3 * dE_dy[i][2];                            // dS_dy[0][1]
        }
        break;
    }
    }
}

void ElasticSolid::getPlaneStressTensorAndDerivative(int ndofs,
                                                     const double E[3],
                                                     const double dE_dy[][3],
                                                     const double CI[2][2],
                                                     double S[3],
                                                     double dS_dy[][3]) const
{
    switch (model_)
    {
    case SAINT_VENANT_KIRCHHOFF:
    {
        switch (planeAnalysis_)
        {
        case PLANE_STRAIN:
        {
            const double prop1 = young_ / ((1.0 + poisson_) * (1.0 - 2.0 * poisson_));
            const double prop2 = 1.0 - poisson_;
            const double prop3 = young_ / (1.0 + poisson_);
            S[0] = prop1 * (prop2 * E[0] + poisson_ * E[1]); // S[0][0]
            S[1] = prop1 * (prop2 * E[1] + poisson_ * E[0]); // S[1][1]
            S[2] = prop3 * E[2];                             // S[0][1]
            for (unsigned int i = 0; i < ndofs; i++)
            {
                dS_dy[i][0] = prop1 * (prop2 * dE_dy[i][0] + poisson_ * dE_dy[i][1]); // dS_dy[0][0]
                dS_dy[i][1] = prop1 * (prop2 * dE_dy[i][1] + poisson_ * dE_dy[i][0]); // dS_dy[1][1]
                dS_dy[i][2] = prop3 * dE_dy[i][2];                                    // dS_dy[0][1]
            }
            break;
        }
        case PLANE_STRESS:
        {
            const double prop1 = young_ / (1.0 - (poisson_ * poisson_));
            const double prop3 = young_ / (1.0 + poisson_);
            S[0] = prop1 * (E[0] + poisson_ * E[1]); // S[0][0]
            S[1] = prop1 * (E[1] + poisson_ * E[0]); // S[1][1]
            S[2] = prop3 * E[2];                     // S[0][1]
            for (unsigned int i = 0; i < ndofs; i++)
            {
                dS_dy[i][0] = prop1 * (dE_dy[i][0] + poisson_ * dE_dy[i][1]); // dS_dy[0][0]
                dS_dy[i][1] = prop1 * (dE_dy[i][1] + poisson_ * dE_dy[i][0]); // dS_dy[1][1]
                dS_dy[i][2] = prop3 * dE_dy[i][2];                            // dS_dy[0][1]
            }
            break;
        }
        }
        break;
    }

    case NEO_HOOKEAN:
    {
        // switch (planeAnalysis_)
        //{
        // case PLANE_STRAIN:
        //{

        // Plane strain case
        double C[2][2] = {};
        C[0][0] = 2. * E[0] + 1.0;
        C[0][1] = 2. * E[2];
        C[1][0] = 2. * E[2];
        C[1][1] = 2. * E[1] + 1.0;
        const double prop1 = young_ / (2.0 * (1.0 + poisson_));                               // G or mi
        const double prop2 = poisson_ * young_ / ((1.0 + poisson_) * (1.0 - 2.0 * poisson_)); // lambda = Lamé constant
        const double prop3 = sqrt(C[0][0] * C[1][1] - C[0][1] * C[1][0]);                     // Jacobian determinant

        S[0] = prop1 * (1.0 - CI[0][0]) + prop2 * log(prop3) * CI[0][0]; // S[0][0]
        S[1] = prop1 * (1.0 - CI[1][1]) + prop2 * log(prop3) * CI[1][1]; // S[1][1]
        S[2] = prop1 * (0.0 - CI[0][1]) + prop2 * log(prop3) * CI[0][1]; // S[0][1]

        double Ct[2][2][2][2] = {};
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    for (int l = 0; l < 2; l++)
                    {
                        Ct[i][j][k][l] = -2.0 * prop1 * (-0.5 * (CI[i][k] * CI[j][l] + CI[i][l] * CI[j][k])) +
                                         prop2 * CI[i][j] * CI[k][l] +
                                         2.0 * prop2 * log(prop3) * (-0.5 * (CI[i][k] * CI[j][l] + CI[i][l] * CI[j][k]));
                    }
                }
            }
        }
        /*double Ct[4][4] = {};
        Ct[0][0] = (-2 * prop1 * (-0.5 * (CI[0][0] * CI[0][0] + CI[0][0] * CI[0][0]))) + (lame * CI[0][0] * CI[0][0]) + (2 * lame * log(J) * (-0.5 * (CI[0][0] * CI[0][0] + CI[0][0] * CI[0][0])));
        Ct[0][1] = (-2 * prop1 * (-0.5 * (CI[0][0] * CI[0][1] + CI[0][1] * CI[0][0]))) + (lame * CI[0][0] * CI[0][1]) + (2 * lame * log(J) * (-0.5 * (CI[0][0] * CI[0][1] + CI[0][1] * CI[0][0])));
        Ct[0][2] = (-2 * prop1 * (-0.5 * (CI[0][0] * CI[1][0] + CI[0][0] * CI[1][0]))) + (lame * CI[0][1] * CI[0][0]) + (2 * lame * log(J) * (-0.5 * (CI[0][0] * CI[1][0] + CI[0][0] * CI[1][0])));
        Ct[0][3] = (-2 * prop1 * (-0.5 * (CI[0][0] * CI[1][1] + CI[0][1] * CI[1][0]))) + (lame * CI[0][1] * CI[0][1]) + (2 * lame * log(J) * (-0.5 * (CI[0][0] * CI[1][1] + CI[0][1] * CI[1][0])));
        Ct[1][0] = (-2 * prop1 * (-0.5 * (CI[0][1] * CI[0][0] + CI[0][0] * CI[0][1]))) + (lame * CI[0][0] * CI[1][0]) + (2 * lame * log(J) * (-0.5 * (CI[0][1] * CI[0][0] + CI[0][0] * CI[0][1])));
        Ct[1][1] = (-2 * prop1 * (-0.5 * (CI[0][1] * CI[0][1] + CI[0][1] * CI[0][1]))) + (lame * CI[0][0] * CI[1][1]) + (2 * lame * log(J) * (-0.5 * (CI[0][1] * CI[0][1] + CI[0][1] * CI[0][1])));
        Ct[1][2] = (-2 * prop1 * (-0.5 * (CI[0][1] * CI[1][0] + CI[0][0] * CI[1][1]))) + (lame * CI[0][1] * CI[1][0]) + (2 * lame * log(J) * (-0.5 * (CI[0][1] * CI[1][0] + CI[0][0] * CI[1][1])));
        Ct[1][3] = (-2 * prop1 * (-0.5 * (CI[0][1] * CI[1][1] + CI[0][1] * CI[1][1]))) + (lame * CI[0][1] * CI[1][1]) + (2 * lame * log(J) * (-0.5 * (CI[0][1] * CI[1][1] + CI[0][1] * CI[1][1])));
        Ct[2][0] = (-2 * prop1 * (-0.5 * (CI[1][0] * CI[0][0] + CI[1][0] * CI[0][0]))) + (lame * CI[1][0] * CI[0][0]) + (2 * lame * log(J) * (-0.5 * (CI[1][0] * CI[0][0] + CI[1][0] * CI[0][0])));
        Ct[2][1] = (-2 * prop1 * (-0.5 * (CI[1][0] * CI[0][1] + CI[1][1] * CI[0][0]))) + (lame * CI[1][0] * CI[0][1]) + (2 * lame * log(J) * (-0.5 * (CI[1][0] * CI[0][1] + CI[1][1] * CI[0][0])));
        Ct[2][2] = (-2 * prop1 * (-0.5 * (CI[1][0] * CI[1][0] + CI[1][0] * CI[1][0]))) + (lame * CI[1][1] * CI[0][0]) + (2 * lame * log(J) * (-0.5 * (CI[1][0] * CI[1][0] + CI[1][0] * CI[1][0])));
        Ct[2][3] = (-2 * prop1 * (-0.5 * (CI[1][0] * CI[1][1] + CI[1][1] * CI[1][0]))) + (lame * CI[1][1] * CI[0][1]) + (2 * lame * log(J) * (-0.5 * (CI[1][0] * CI[1][1] + CI[1][1] * CI[1][0])));
        Ct[3][0] = (-2 * prop1 * (-0.5 * (CI[1][1] * CI[0][0] + CI[1][0] * CI[0][1]))) + (lame * CI[1][0] * CI[1][0]) + (2 * lame * log(J) * (-0.5 * (CI[1][1] * CI[0][0] + CI[1][0] * CI[0][1])));
        Ct[3][1] = (-2 * prop1 * (-0.5 * (CI[1][1] * CI[0][1] + CI[1][1] * CI[0][1]))) + (lame * CI[1][0] * CI[1][1]) + (2 * lame * log(J) * (-0.5 * (CI[1][1] * CI[0][1] + CI[1][1] * CI[0][1])));
        Ct[3][2] = (-2 * prop1 * (-0.5 * (CI[1][1] * CI[1][0] + CI[1][0] * CI[1][1]))) + (lame * CI[1][1] * CI[1][0]) + (2 * lame * log(J) * (-0.5 * (CI[1][1] * CI[1][0] + CI[1][0] * CI[1][1])));
        Ct[3][3] = (-2 * prop1 * (-0.5 * (CI[1][1] * CI[1][1] + CI[1][1] * CI[1][1]))) + (lame * CI[1][1] * CI[1][1]) + (2 * lame * log(J) * (-0.5 * (CI[1][1] * CI[1][1] + CI[1][1] * CI[1][1])));*/

        for (unsigned int i = 0; i < ndofs; i++)
        {
            dS_dy[i][0] = Ct[0][0][0][0] * dE_dy[i][0] + Ct[0][0][1][1] * dE_dy[i][1] + Ct[0][0][0][1] * dE_dy[i][2] + Ct[0][0][1][0] * dE_dy[i][2]; // dS_dy[0][0]
            dS_dy[i][1] = Ct[1][1][0][0] * dE_dy[i][0] + Ct[1][1][1][1] * dE_dy[i][1] + Ct[1][1][0][1] * dE_dy[i][2] + Ct[1][1][1][0] * dE_dy[i][2]; // dS_dy[0][0]
            dS_dy[i][2] = Ct[0][1][0][0] * dE_dy[i][0] + Ct[0][1][1][1] * dE_dy[i][1] + Ct[0][1][0][1] * dE_dy[i][2] + Ct[0][1][1][0] * dE_dy[i][2]; // dS_dy[0][0]
        }
        break;
        //}
        // case PLANE_STRESS:
        //{
        /*const double prop1 = young_ / (1.0 - (poisson_ * poisson_));
        const double prop3 = young_ / (1.0 + poisson_);
        S[0] = prop1 * (E[0] + poisson_ * E[1]); // S[0][0]
        S[1] = prop1 * (E[1] + poisson_ * E[0]); // S[1][1]
        S[2] = prop3 * E[2];                     // S[0][1]*/
        // std::cout << "Plane stress case for Neo-Hookean constitutive model has to be implemented.";
        // exit(EXIT_FAILURE);
        // break;
        //}
        //}
        // break;
    }
    }
}

void ElasticSolid::getStressTensor(const double E[6],
                                   const double dx_dy[3][3],
                                   double S[6]) const
{
    double transvYoung = 0.5 * young_ / (1.0 + poisson_);
    double lame = 2.0 * transvYoung * poisson_ / (1.0 - 2.0 * poisson_);
    double trE = E[0] + E[1] + E[2];
    S[0] = 2.0 * transvYoung * E[0] + lame * trE; // S[0][0]
    S[1] = 2.0 * transvYoung * E[1] + lame * trE; // S[1][1]
    S[2] = 2.0 * transvYoung * E[2] + lame * trE; // S[2][2]
    S[3] = 2.0 * transvYoung * E[3];              // S[0][1]
    S[4] = 2.0 * transvYoung * E[4];              // S[0][2]
    S[5] = 2.0 * transvYoung * E[5];              // S[1][2]
}

void ElasticSolid::getStressTensorDerivative(const double dE_dy[6],
                                             const double dx_dy[3][3],
                                             double dS_dy[6]) const
{
    double transvYoung = 0.5 * young_ / (1.0 + poisson_);
    double lame = 2.0 * transvYoung * poisson_ / (1.0 - 2.0 * poisson_);
    double trdE_dy = dE_dy[0] + dE_dy[1] + dE_dy[2];
    dS_dy[0] = 2.0 * transvYoung * dE_dy[0] + lame * trdE_dy; // dS_dy[0][0]
    dS_dy[1] = 2.0 * transvYoung * dE_dy[1] + lame * trdE_dy; // dS_dy[1][1]
    dS_dy[2] = 2.0 * transvYoung * dE_dy[2] + lame * trdE_dy; // dS_dy[2][2]
    dS_dy[3] = 2.0 * transvYoung * dE_dy[3];                  // dS_dy[0][1]
    dS_dy[4] = 2.0 * transvYoung * dE_dy[4];                  // dS_dy[0][2]
    dS_dy[5] = 2.0 * transvYoung * dE_dy[5];                  // dS_dy[1][2]
}

void ElasticSolid::getStressTensorDerivative(int ndofs,
                                             const double dE_dy[][6],
                                             const double dx_dy[3][3],
                                             double dS_dy[][6]) const
{
    double transvYoung = 0.5 * young_ / (1.0 + poisson_);
    double lame = 2.0 * transvYoung * poisson_ / (1.0 - 2.0 * poisson_);
    for (unsigned int i = 0; i < ndofs; i++)
    {
        double trdE_dy = dE_dy[i][0] + dE_dy[i][1] + dE_dy[i][2];
        dS_dy[i][0] = 2.0 * transvYoung * dE_dy[i][0] + lame * trdE_dy; // dS_dy[0][0]
        dS_dy[i][1] = 2.0 * transvYoung * dE_dy[i][1] + lame * trdE_dy; // dS_dy[1][1]
        dS_dy[i][2] = 2.0 * transvYoung * dE_dy[i][2] + lame * trdE_dy; // dS_dy[2][2]
        dS_dy[i][3] = 2.0 * transvYoung * dE_dy[i][3];                  // dS_dy[0][1]
        dS_dy[i][4] = 2.0 * transvYoung * dE_dy[i][4];                  // dS_dy[0][2]
        dS_dy[i][5] = 2.0 * transvYoung * dE_dy[i][5];                  // dS_dy[1][2]
    }
}

void ElasticSolid::getStressTensorAndDerivative(int ndofs,
                                                const double E[6],
                                                const double dE_dy[][6],
                                                const double dx_dy[3][3],
                                                double S[6],
                                                double dS_dy[][6]) const
{
    double transvYoung = 0.5 * young_ / (1.0 + poisson_);
    double lame = 2.0 * transvYoung * poisson_ / (1.0 - 2.0 * poisson_);
    double trE = E[0] + E[1] + E[2];
    S[0] = 2.0 * transvYoung * E[0] + lame * trE; // S[0][0]
    S[1] = 2.0 * transvYoung * E[1] + lame * trE; // S[1][1]
    S[2] = 2.0 * transvYoung * E[2] + lame * trE; // S[2][2]
    S[3] = 2.0 * transvYoung * E[3];              // S[0][1]
    S[4] = 2.0 * transvYoung * E[4];              // S[0][2]
    S[5] = 2.0 * transvYoung * E[5];              // S[1][2]
    for (unsigned int i = 0; i < ndofs; i++)
    {
        double trdE_dy = dE_dy[i][0] + dE_dy[i][1] + dE_dy[i][2];
        dS_dy[i][0] = 2.0 * transvYoung * dE_dy[i][0] + lame * trdE_dy; // dS_dy[0][0]
        dS_dy[i][1] = 2.0 * transvYoung * dE_dy[i][1] + lame * trdE_dy; // dS_dy[1][1]
        dS_dy[i][2] = 2.0 * transvYoung * dE_dy[i][2] + lame * trdE_dy; // dS_dy[2][2]
        dS_dy[i][3] = 2.0 * transvYoung * dE_dy[i][3];                  // dS_dy[0][1]
        dS_dy[i][4] = 2.0 * transvYoung * dE_dy[i][4];                  // dS_dy[0][2]
        dS_dy[i][5] = 2.0 * transvYoung * dE_dy[i][5];                  // dS_dy[1][2]
    }
}

NewtonianFluid::NewtonianFluid(const double &viscosity,
                               const double &density)
    : Material(density, MaterialType::NEWTONIAN_INCOMPRESSIBLE_FLUID),
      viscosity_(viscosity) {}

NewtonianFluid::~NewtonianFluid() {}

double NewtonianFluid::getViscosity() const
{
    return viscosity_;
}

void NewtonianFluid::setViscosity(const double &viscosity)
{
    viscosity_ = viscosity;
}

void NewtonianFluid::getPlaneStressTensor(const double dE_dt[3],
                                          const double dx_dy[2][2],
                                          double S[3]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] - dx_dy[0][1] * dx_dy[1][0];
    jac = 1.0 / jac;

    const double I[2][2] = {{1.0, 0.0}, {0.0, 1.0}};
    double C_current[2][2][2][2];
    double C_reference[2][2][2][2];

    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    for (unsigned int A = 0; A < 2; A++)
                    {
                        for (unsigned int B = 0; B < 2; B++)
                        {
                            for (unsigned int C = 0; C < 2; C++)
                            {
                                for (unsigned int D = 0; D < 2; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    S[0] = C_reference[0][0][0][0] * dE_dt[0] + C_reference[0][0][1][1] * dE_dt[1] + 2.0 * C_reference[0][0][0][1] * dE_dt[2]; // S[0][0]
    S[1] = C_reference[1][1][0][0] * dE_dt[0] + C_reference[1][1][1][1] * dE_dt[1] + 2.0 * C_reference[1][1][0][1] * dE_dt[2]; // S[1][1]
    S[2] = C_reference[0][1][0][0] * dE_dt[0] + C_reference[0][1][1][1] * dE_dt[1] + 2.0 * C_reference[0][1][0][1] * dE_dt[2]; // S[0][1]
}

void NewtonianFluid::getPlaneStressTensorDerivative(const double dE_dtdy[3],
                                                    const double dx_dy[2][2],
                                                    double dS_dy[3]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] - dx_dy[0][1] * dx_dy[1][0];
    jac = 1.0 / jac;

    const double I[2][2] = {{1.0, 0.0}, {0.0, 1.0}};
    double C_current[2][2][2][2];
    double C_reference[2][2][2][2];

    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    for (unsigned int A = 0; A < 2; A++)
                    {
                        for (unsigned int B = 0; B < 2; B++)
                        {
                            for (unsigned int C = 0; C < 2; C++)
                            {
                                for (unsigned int D = 0; D < 2; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    dS_dy[0] = C_reference[0][0][0][0] * dE_dtdy[0] + C_reference[0][0][1][1] * dE_dtdy[1] +
               2.0 * C_reference[0][0][0][1] * dE_dtdy[2]; // dS_dy[0][0]
    dS_dy[1] = C_reference[1][1][0][0] * dE_dtdy[0] + C_reference[1][1][1][1] * dE_dtdy[1] +
               2.0 * C_reference[1][1][0][1] * dE_dtdy[2]; // dS_dy[1][1]
    dS_dy[2] = C_reference[0][1][0][0] * dE_dtdy[0] + C_reference[0][1][1][1] * dE_dtdy[1] +
               2.0 * C_reference[0][1][0][1] * dE_dtdy[2]; // dS_dy[0][1]
}

void NewtonianFluid::getPlaneStressTensorDerivative(int ndofs,
                                                    const double dE_dtdy[][3],
                                                    const double dx_dy[2][2],
                                                    double dS_dy[][3]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] - dx_dy[0][1] * dx_dy[1][0];
    jac = 1.0 / jac;

    const double I[2][2] = {{1.0, 0.0}, {0.0, 1.0}};
    double C_current[2][2][2][2];
    double C_reference[2][2][2][2];

    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    for (unsigned int A = 0; A < 2; A++)
                    {
                        for (unsigned int B = 0; B < 2; B++)
                        {
                            for (unsigned int C = 0; C < 2; C++)
                            {
                                for (unsigned int D = 0; D < 2; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (unsigned int i = 0; i < ndofs; i++)
    {
        dS_dy[i][0] = C_reference[0][0][0][0] * dE_dtdy[i][0] + C_reference[0][0][1][1] * dE_dtdy[i][1] +
                      2.0 * C_reference[0][0][0][1] * dE_dtdy[i][2]; // dS_dy[0][0]
        dS_dy[i][1] = C_reference[1][1][0][0] * dE_dtdy[i][0] + C_reference[1][1][1][1] * dE_dtdy[i][1] +
                      2.0 * C_reference[1][1][0][1] * dE_dtdy[i][2]; // dS_dy[1][1]
        dS_dy[i][2] = C_reference[0][1][0][0] * dE_dtdy[i][0] + C_reference[0][1][1][1] * dE_dtdy[i][1] +
                      2.0 * C_reference[0][1][0][1] * dE_dtdy[i][2]; // dS_dy[0][1]
    }
}

void NewtonianFluid::getPlaneStressTensorAndDerivative(int ndofs,
                                                       const double dE_dt[3],
                                                       const double dE_dtdy[][3],
                                                       const double dx_dy[2][2],
                                                       double S[3],
                                                       double dS_dy[][3]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] - dx_dy[0][1] * dx_dy[1][0];
    jac = 1.0 / jac;

    const double I[2][2] = {{1.0, 0.0}, {0.0, 1.0}};
    double C_current[2][2][2][2];
    double C_reference[2][2][2][2];

    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 2; i++)
    {
        for (unsigned int j = 0; j < 2; j++)
        {
            for (unsigned int k = 0; k < 2; k++)
            {
                for (unsigned int l = 0; l < 2; l++)
                {
                    for (unsigned int A = 0; A < 2; A++)
                    {
                        for (unsigned int B = 0; B < 2; B++)
                        {
                            for (unsigned int C = 0; C < 2; C++)
                            {
                                for (unsigned int D = 0; D < 2; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    S[0] = C_reference[0][0][0][0] * dE_dt[0] + C_reference[0][0][1][1] * dE_dt[1] + 2.0 * C_reference[0][0][0][1] * dE_dt[2]; // S[0][0]
    S[1] = C_reference[1][1][0][0] * dE_dt[0] + C_reference[1][1][1][1] * dE_dt[1] + 2.0 * C_reference[1][1][0][1] * dE_dt[2]; // S[1][1]
    S[2] = C_reference[0][1][0][0] * dE_dt[0] + C_reference[0][1][1][1] * dE_dt[1] + 2.0 * C_reference[0][1][0][1] * dE_dt[2]; // S[0][1]

    for (unsigned int i = 0; i < ndofs; i++)
    {
        dS_dy[i][0] = C_reference[0][0][0][0] * dE_dtdy[i][0] + C_reference[0][0][1][1] * dE_dtdy[i][1] +
                      2.0 * C_reference[0][0][0][1] * dE_dtdy[i][2]; // dS_dy[0][0]
        dS_dy[i][1] = C_reference[1][1][0][0] * dE_dtdy[i][0] + C_reference[1][1][1][1] * dE_dtdy[i][1] +
                      2.0 * C_reference[1][1][0][1] * dE_dtdy[i][2]; // dS_dy[1][1]
        dS_dy[i][2] = C_reference[0][1][0][0] * dE_dtdy[i][0] + C_reference[0][1][1][1] * dE_dtdy[i][1] +
                      2.0 * C_reference[0][1][0][1] * dE_dtdy[i][2]; // dS_dy[0][1]
    }
}

void NewtonianFluid::getStressTensor(const double dE_dt[6],
                                     const double dx_dy[3][3],
                                     double S[6]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] * dx_dy[2][2] +
                 dx_dy[0][1] * dx_dy[1][2] * dx_dy[2][0] +
                 dx_dy[0][2] * dx_dy[1][0] * dx_dy[2][1] -
                 dx_dy[0][2] * dx_dy[1][1] * dx_dy[2][0] -
                 dx_dy[0][1] * dx_dy[1][0] * dx_dy[2][2] -
                 dx_dy[0][0] * dx_dy[1][2] * dx_dy[2][1];
    jac = 1.0 / jac;

    const double I[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    double C_current[3][3][3][3];
    double C_reference[3][3][3][3];

    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    for (unsigned int A = 0; A < 3; A++)
                    {
                        for (unsigned int B = 0; B < 3; B++)
                        {
                            for (unsigned int C = 0; C < 3; C++)
                            {
                                for (unsigned int D = 0; D < 3; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    S[0] = C_reference[0][0][0][0] * dE_dt[0] + C_reference[0][0][1][1] * dE_dt[1] + C_reference[0][0][2][2] * dE_dt[2] +
           (C_reference[0][0][0][1] * dE_dt[3] + C_reference[0][0][0][2] * dE_dt[4] + C_reference[0][0][1][2] * dE_dt[5]) * 2.0; // S[0][0]
    S[1] = C_reference[1][1][0][0] * dE_dt[0] + C_reference[1][1][1][1] * dE_dt[1] + C_reference[1][1][2][2] * dE_dt[2] +
           (C_reference[1][1][0][1] * dE_dt[3] + C_reference[1][1][0][2] * dE_dt[4] + C_reference[1][1][1][2] * dE_dt[5]) * 2.0; // S[1][1]
    S[2] = C_reference[2][2][0][0] * dE_dt[0] + C_reference[2][2][1][1] * dE_dt[1] + C_reference[2][2][2][2] * dE_dt[2] +
           (C_reference[2][2][0][1] * dE_dt[3] + C_reference[2][2][0][2] * dE_dt[4] + C_reference[2][2][1][2] * dE_dt[5]) * 2.0; // S[2][2]
    S[3] = C_reference[0][1][0][0] * dE_dt[0] + C_reference[0][1][1][1] * dE_dt[1] + C_reference[0][1][2][2] * dE_dt[2] +
           (C_reference[0][1][0][1] * dE_dt[3] + C_reference[0][1][0][2] * dE_dt[4] + C_reference[0][1][1][2] * dE_dt[5]) * 2.0; // S[0][1]
    S[4] = C_reference[0][2][0][0] * dE_dt[0] + C_reference[0][2][1][1] * dE_dt[1] + C_reference[0][2][2][2] * dE_dt[2] +
           (C_reference[0][2][0][1] * dE_dt[3] + C_reference[0][2][0][2] * dE_dt[4] + C_reference[0][2][1][2] * dE_dt[5]) * 2.0; // S[0][2]
    S[5] = C_reference[1][2][0][0] * dE_dt[0] + C_reference[1][2][1][1] * dE_dt[1] + C_reference[1][2][2][2] * dE_dt[2] +
           (C_reference[1][2][0][1] * dE_dt[3] + C_reference[1][2][0][2] * dE_dt[4] + C_reference[1][2][1][2] * dE_dt[5]) * 2.0; // S[1][2]
}

void NewtonianFluid::getStressTensorDerivative(const double dE_dtdy[6],
                                               const double dx_dy[3][3],
                                               double dS_dy[6]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] * dx_dy[2][2] +
                 dx_dy[0][1] * dx_dy[1][2] * dx_dy[2][0] +
                 dx_dy[0][2] * dx_dy[1][0] * dx_dy[2][1] -
                 dx_dy[0][2] * dx_dy[1][1] * dx_dy[2][0] -
                 dx_dy[0][1] * dx_dy[1][0] * dx_dy[2][2] -
                 dx_dy[0][0] * dx_dy[1][2] * dx_dy[2][1];
    jac = 1.0 / jac;

    const double I[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    double C_current[3][3][3][3];
    double C_reference[3][3][3][3];

    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    for (unsigned int A = 0; A < 3; A++)
                    {
                        for (unsigned int B = 0; B < 3; B++)
                        {
                            for (unsigned int C = 0; C < 3; C++)
                            {
                                for (unsigned int D = 0; D < 3; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    dS_dy[0] = C_reference[0][0][0][0] * dE_dtdy[0] + C_reference[0][0][1][1] * dE_dtdy[1] + C_reference[0][0][2][2] * dE_dtdy[2] +
               (C_reference[0][0][0][1] * dE_dtdy[3] + C_reference[0][0][0][2] * dE_dtdy[4] + C_reference[0][0][1][2] * dE_dtdy[5]) * 2.0; // dS_dy[0][0]
    dS_dy[1] = C_reference[1][1][0][0] * dE_dtdy[0] + C_reference[1][1][1][1] * dE_dtdy[1] + C_reference[1][1][2][2] * dE_dtdy[2] +
               (C_reference[1][1][0][1] * dE_dtdy[3] + C_reference[1][1][0][2] * dE_dtdy[4] + C_reference[1][1][1][2] * dE_dtdy[5]) * 2.0; // dS_dy[1][1]
    dS_dy[2] = C_reference[2][2][0][0] * dE_dtdy[0] + C_reference[2][2][1][1] * dE_dtdy[1] + C_reference[2][2][2][2] * dE_dtdy[2] +
               (C_reference[2][2][0][1] * dE_dtdy[3] + C_reference[2][2][0][2] * dE_dtdy[4] + C_reference[2][2][1][2] * dE_dtdy[5]) * 2.0; // dS_dy[2][2]
    dS_dy[3] = C_reference[0][1][0][0] * dE_dtdy[0] + C_reference[0][1][1][1] * dE_dtdy[1] + C_reference[0][1][2][2] * dE_dtdy[2] +
               (C_reference[0][1][0][1] * dE_dtdy[3] + C_reference[0][1][0][2] * dE_dtdy[4] + C_reference[0][1][1][2] * dE_dtdy[5]) * 2.0; // dS_dy[0][1]
    dS_dy[4] = C_reference[0][2][0][0] * dE_dtdy[0] + C_reference[0][2][1][1] * dE_dtdy[1] + C_reference[0][2][2][2] * dE_dtdy[2] +
               (C_reference[0][2][0][1] * dE_dtdy[3] + C_reference[0][2][0][2] * dE_dtdy[4] + C_reference[0][2][1][2] * dE_dtdy[5]) * 2.0; // dS_dy[0][2]
    dS_dy[5] = C_reference[1][2][0][0] * dE_dtdy[0] + C_reference[1][2][1][1] * dE_dtdy[1] + C_reference[1][2][2][2] * dE_dtdy[2] +
               (C_reference[1][2][0][1] * dE_dtdy[3] + C_reference[1][2][0][2] * dE_dtdy[4] + C_reference[1][2][1][2] * dE_dtdy[5]) * 2.0; // dS_dy[1][2]
}

void NewtonianFluid::getStressTensorDerivative(int ndofs,
                                               const double dE_dtdy[][6],
                                               const double dx_dy[3][3],
                                               double dS_dy[][6]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] * dx_dy[2][2] +
                 dx_dy[0][1] * dx_dy[1][2] * dx_dy[2][0] +
                 dx_dy[0][2] * dx_dy[1][0] * dx_dy[2][1] -
                 dx_dy[0][2] * dx_dy[1][1] * dx_dy[2][0] -
                 dx_dy[0][1] * dx_dy[1][0] * dx_dy[2][2] -
                 dx_dy[0][0] * dx_dy[1][2] * dx_dy[2][1];
    jac = 1.0 / jac;

    const double I[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    double C_current[3][3][3][3];
    double C_reference[3][3][3][3];

    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    for (unsigned int A = 0; A < 3; A++)
                    {
                        for (unsigned int B = 0; B < 3; B++)
                        {
                            for (unsigned int C = 0; C < 3; C++)
                            {
                                for (unsigned int D = 0; D < 3; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (unsigned int i = 0; i < ndofs; i++)
    {
        dS_dy[i][0] = C_reference[0][0][0][0] * dE_dtdy[i][0] + C_reference[0][0][1][1] * dE_dtdy[i][1] + C_reference[0][0][2][2] * dE_dtdy[i][2] +
                      (C_reference[0][0][0][1] * dE_dtdy[i][3] + C_reference[0][0][0][2] * dE_dtdy[i][4] + C_reference[0][0][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[0][0]
        dS_dy[i][1] = C_reference[1][1][0][0] * dE_dtdy[i][0] + C_reference[1][1][1][1] * dE_dtdy[i][1] + C_reference[1][1][2][2] * dE_dtdy[i][2] +
                      (C_reference[1][1][0][1] * dE_dtdy[i][3] + C_reference[1][1][0][2] * dE_dtdy[i][4] + C_reference[1][1][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[1][1]
        dS_dy[i][2] = C_reference[2][2][0][0] * dE_dtdy[i][0] + C_reference[2][2][1][1] * dE_dtdy[i][1] + C_reference[2][2][2][2] * dE_dtdy[i][2] +
                      (C_reference[2][2][0][1] * dE_dtdy[i][3] + C_reference[2][2][0][2] * dE_dtdy[i][4] + C_reference[2][2][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[2][2]
        dS_dy[i][3] = C_reference[0][1][0][0] * dE_dtdy[i][0] + C_reference[0][1][1][1] * dE_dtdy[i][1] + C_reference[0][1][2][2] * dE_dtdy[i][2] +
                      (C_reference[0][1][0][1] * dE_dtdy[i][3] + C_reference[0][1][0][2] * dE_dtdy[i][4] + C_reference[0][1][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[0][1]
        dS_dy[i][4] = C_reference[0][2][0][0] * dE_dtdy[i][0] + C_reference[0][2][1][1] * dE_dtdy[i][1] + C_reference[0][2][2][2] * dE_dtdy[i][2] +
                      (C_reference[0][2][0][1] * dE_dtdy[i][3] + C_reference[0][2][0][2] * dE_dtdy[i][4] + C_reference[0][2][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[0][2]
        dS_dy[i][5] = C_reference[1][2][0][0] * dE_dtdy[i][0] + C_reference[1][2][1][1] * dE_dtdy[i][1] + C_reference[1][2][2][2] * dE_dtdy[i][2] +
                      (C_reference[1][2][0][1] * dE_dtdy[i][3] + C_reference[1][2][0][2] * dE_dtdy[i][4] + C_reference[1][2][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[1][2]
    }
}

void NewtonianFluid::getStressTensorAndDerivative(int ndofs,
                                                  const double dE_dt[6],
                                                  const double dE_dtdy[][6],
                                                  const double dx_dy[3][3],
                                                  double S[6],
                                                  double dS_dy[][6]) const
{
    double jac = dx_dy[0][0] * dx_dy[1][1] * dx_dy[2][2] +
                 dx_dy[0][1] * dx_dy[1][2] * dx_dy[2][0] +
                 dx_dy[0][2] * dx_dy[1][0] * dx_dy[2][1] -
                 dx_dy[0][2] * dx_dy[1][1] * dx_dy[2][0] -
                 dx_dy[0][1] * dx_dy[1][0] * dx_dy[2][2] -
                 dx_dy[0][0] * dx_dy[1][2] * dx_dy[2][1];
    jac = 1.0 / jac;

    const double I[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    double C_current[3][3][3][3];
    double C_reference[3][3][3][3];

    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    C_current[i][j][k][l] = viscosity_ * (I[i][k] * I[j][l] + I[i][l] * I[j][k]);
                    C_reference[i][j][k][l] = 0.0;
                }
            }
        }
    }
    for (unsigned int i = 0; i < 3; i++)
    {
        for (unsigned int j = 0; j < 3; j++)
        {
            for (unsigned int k = 0; k < 3; k++)
            {
                for (unsigned int l = 0; l < 3; l++)
                {
                    for (unsigned int A = 0; A < 3; A++)
                    {
                        for (unsigned int B = 0; B < 3; B++)
                        {
                            for (unsigned int C = 0; C < 3; C++)
                            {
                                for (unsigned int D = 0; D < 3; D++)
                                {
                                    C_reference[i][j][k][l] += jac * dx_dy[i][A] * dx_dy[j][B] * dx_dy[k][C] * dx_dy[l][D] * C_current[A][B][C][D];
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    S[0] = C_reference[0][0][0][0] * dE_dt[0] + C_reference[0][0][1][1] * dE_dt[1] + C_reference[0][0][2][2] * dE_dt[2] +
           (C_reference[0][0][0][1] * dE_dt[3] + C_reference[0][0][0][2] * dE_dt[4] + C_reference[0][0][1][2] * dE_dt[5]) * 2.0; // S[0][0]
    S[1] = C_reference[1][1][0][0] * dE_dt[0] + C_reference[1][1][1][1] * dE_dt[1] + C_reference[1][1][2][2] * dE_dt[2] +
           (C_reference[1][1][0][1] * dE_dt[3] + C_reference[1][1][0][2] * dE_dt[4] + C_reference[1][1][1][2] * dE_dt[5]) * 2.0; // S[1][1]
    S[2] = C_reference[2][2][0][0] * dE_dt[0] + C_reference[2][2][1][1] * dE_dt[1] + C_reference[2][2][2][2] * dE_dt[2] +
           (C_reference[2][2][0][1] * dE_dt[3] + C_reference[2][2][0][2] * dE_dt[4] + C_reference[2][2][1][2] * dE_dt[5]) * 2.0; // S[2][2]
    S[3] = C_reference[0][1][0][0] * dE_dt[0] + C_reference[0][1][1][1] * dE_dt[1] + C_reference[0][1][2][2] * dE_dt[2] +
           (C_reference[0][1][0][1] * dE_dt[3] + C_reference[0][1][0][2] * dE_dt[4] + C_reference[0][1][1][2] * dE_dt[5]) * 2.0; // S[0][1]
    S[4] = C_reference[0][2][0][0] * dE_dt[0] + C_reference[0][2][1][1] * dE_dt[1] + C_reference[0][2][2][2] * dE_dt[2] +
           (C_reference[0][2][0][1] * dE_dt[3] + C_reference[0][2][0][2] * dE_dt[4] + C_reference[0][2][1][2] * dE_dt[5]) * 2.0; // S[0][2]
    S[5] = C_reference[1][2][0][0] * dE_dt[0] + C_reference[1][2][1][1] * dE_dt[1] + C_reference[1][2][2][2] * dE_dt[2] +
           (C_reference[1][2][0][1] * dE_dt[3] + C_reference[1][2][0][2] * dE_dt[4] + C_reference[1][2][1][2] * dE_dt[5]) * 2.0; // S[1][2]

    for (unsigned int i = 0; i < ndofs; i++)
    {
        dS_dy[i][0] = C_reference[0][0][0][0] * dE_dtdy[i][0] + C_reference[0][0][1][1] * dE_dtdy[i][1] + C_reference[0][0][2][2] * dE_dtdy[i][2] +
                      (C_reference[0][0][0][1] * dE_dtdy[i][3] + C_reference[0][0][0][2] * dE_dtdy[i][4] + C_reference[0][0][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[0][0]
        dS_dy[i][1] = C_reference[1][1][0][0] * dE_dtdy[i][0] + C_reference[1][1][1][1] * dE_dtdy[i][1] + C_reference[1][1][2][2] * dE_dtdy[i][2] +
                      (C_reference[1][1][0][1] * dE_dtdy[i][3] + C_reference[1][1][0][2] * dE_dtdy[i][4] + C_reference[1][1][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[1][1]
        dS_dy[i][2] = C_reference[2][2][0][0] * dE_dtdy[i][0] + C_reference[2][2][1][1] * dE_dtdy[i][1] + C_reference[2][2][2][2] * dE_dtdy[i][2] +
                      (C_reference[2][2][0][1] * dE_dtdy[i][3] + C_reference[2][2][0][2] * dE_dtdy[i][4] + C_reference[2][2][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[2][2]
        dS_dy[i][3] = C_reference[0][1][0][0] * dE_dtdy[i][0] + C_reference[0][1][1][1] * dE_dtdy[i][1] + C_reference[0][1][2][2] * dE_dtdy[i][2] +
                      (C_reference[0][1][0][1] * dE_dtdy[i][3] + C_reference[0][1][0][2] * dE_dtdy[i][4] + C_reference[0][1][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[0][1]
        dS_dy[i][4] = C_reference[0][2][0][0] * dE_dtdy[i][0] + C_reference[0][2][1][1] * dE_dtdy[i][1] + C_reference[0][2][2][2] * dE_dtdy[i][2] +
                      (C_reference[0][2][0][1] * dE_dtdy[i][3] + C_reference[0][2][0][2] * dE_dtdy[i][4] + C_reference[0][2][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[0][2]
        dS_dy[i][5] = C_reference[1][2][0][0] * dE_dtdy[i][0] + C_reference[1][2][1][1] * dE_dtdy[i][1] + C_reference[1][2][2][2] * dE_dtdy[i][2] +
                      (C_reference[1][2][0][1] * dE_dtdy[i][3] + C_reference[1][2][0][2] * dE_dtdy[i][4] + C_reference[1][2][1][2] * dE_dtdy[i][5]) * 2.0; // dS_dy[1][2]
    }
}

RigidParticle::RigidParticle(const double &young,
                             const double &poisson,
                             const double &density,
                             const double &normalDampingRatio,
                             const double &tangentialDampingRatio)
    : Material(density, MaterialType::RIGID),
      young_(young),
      poisson_(poisson),
      normalDampingRatio_(normalDampingRatio),
      tangentialDampingRatio_(tangentialDampingRatio),
      stiffnessProportionalityConstant_(0.01)
{
}

RigidParticle::~RigidParticle() {}

double RigidParticle::getYoung() const
{
    return young_;
}

double RigidParticle::getPoisson() const
{
    return poisson_;
}

double RigidParticle::getNormalDampingRatio() const
{
    return normalDampingRatio_;
}

double RigidParticle::getTangentialDampingRatio() const
{
    return tangentialDampingRatio_;
}

double RigidParticle::getStiffnessProportionalityConstant() const
{
    return stiffnessProportionalityConstant_;
}

void RigidParticle::getPlaneStressTensor(const double E[3],        // input
                                         const double dx_dy[2][2], // input
                                         double S[3]) const
{
} // output

void RigidParticle::getPlaneStressTensorDerivative(const double dE_dy[3],    // input
                                                   const double dx_dy[2][2], // input
                                                   double dS_dy[3]) const
{
} // output

void RigidParticle::getPlaneStressTensorDerivative(int ndofs,                // input
                                                   const double dE_dy[][3],  // input
                                                   const double dx_dy[2][2], // input
                                                   double dS_dy[][3]) const
{
} // output

void RigidParticle::getPlaneStressTensorAndDerivative(int ndofs,                // input
                                                      const double E[3],        // input
                                                      const double dE_dy[][3],  // input
                                                      const double dx_dy[2][2], // input
                                                      double S[3],              // output
                                                      double dS_dy[][3]) const
{
    switch (planeAnalysis_)
    {
    case PLANE_STRAIN:
    {
        const double prop1 = young_ / ((1.0 + poisson_) * (1.0 - 2.0 * poisson_));
        const double prop2 = 1.0 - poisson_;
        const double prop3 = young_ / (1.0 + poisson_);
        S[0] = prop1 * (prop2 * E[0] + poisson_ * E[1]); // S[0][0]
        S[1] = prop1 * (prop2 * E[1] + poisson_ * E[0]); // S[1][1]
        S[2] = prop3 * E[2];                             // S[0][1]
        for (unsigned int i = 0; i < ndofs; i++)
        {
            dS_dy[i][0] = prop1 * (prop2 * dE_dy[i][0] + poisson_ * dE_dy[i][1]); // dS_dy[0][0]
            dS_dy[i][1] = prop1 * (prop2 * dE_dy[i][1] + poisson_ * dE_dy[i][0]); // dS_dy[1][1]
            dS_dy[i][2] = prop3 * dE_dy[i][2];                                    // dS_dy[0][1]
        }
        break;
    }
    case PLANE_STRESS:
    {
        const double prop1 = young_ / (1.0 - (poisson_ * poisson_));
        const double prop3 = young_ / (1.0 + poisson_);
        S[0] = prop1 * (E[0] + poisson_ * E[1]); // S[0][0]
        S[1] = prop1 * (E[1] + poisson_ * E[0]); // S[1][1]
        S[2] = prop3 * E[2];                     // S[0][1]
        for (unsigned int i = 0; i < ndofs; i++)
        {
            dS_dy[i][0] = prop1 * (dE_dy[i][0] + poisson_ * dE_dy[i][1]); // dS_dy[0][0]
            dS_dy[i][1] = prop1 * (dE_dy[i][1] + poisson_ * dE_dy[i][0]); // dS_dy[1][1]
            dS_dy[i][2] = prop3 * dE_dy[i][2];                            // dS_dy[0][1]
        }
        break;
    }
    }
} // output

void RigidParticle::getStressTensor(const double E[6],        // input
                                    const double dx_dy[3][3], // input
                                    double S[6]) const
{
} // output

void RigidParticle::getStressTensorDerivative(const double dE_dy[6],    // input
                                              const double dx_dy[3][3], // input
                                              double dS_dy[6]) const
{
} // output

void RigidParticle::getStressTensorDerivative(int ndofs,                // input
                                              const double dE_dy[][6],  // input
                                              const double dx_dy[3][3], // input
                                              double dS_dy[][6]) const
{
} // output

void RigidParticle::getStressTensorAndDerivative(int ndofs,                // input
                                                 const double E[6],        // input
                                                 const double dE_dy[][6],  // input
                                                 const double dx_dy[3][3], // input
                                                 double S[6],              // output
                                                 double dS_dy[][6]) const
{
    double transvYoung = 0.5 * young_ / (1.0 + poisson_);
    double lame = 2.0 * transvYoung * poisson_ / (1.0 - 2.0 * poisson_);
    double trE = E[0] + E[1] + E[2];
    S[0] = 2.0 * transvYoung * E[0] + lame * trE; // S[0][0]
    S[1] = 2.0 * transvYoung * E[1] + lame * trE; // S[1][1]
    S[2] = 2.0 * transvYoung * E[2] + lame * trE; // S[2][2]
    S[3] = 2.0 * transvYoung * E[3];              // S[0][1]
    S[4] = 2.0 * transvYoung * E[4];              // S[0][2]
    S[5] = 2.0 * transvYoung * E[5];              // S[1][2]
    for (unsigned int i = 0; i < ndofs; i++)
    {
        double trdE_dy = dE_dy[i][0] + dE_dy[i][1] + dE_dy[i][2];
        dS_dy[i][0] = 2.0 * transvYoung * dE_dy[i][0] + lame * trdE_dy; // dS_dy[0][0]
        dS_dy[i][1] = 2.0 * transvYoung * dE_dy[i][1] + lame * trdE_dy; // dS_dy[1][1]
        dS_dy[i][2] = 2.0 * transvYoung * dE_dy[i][2] + lame * trdE_dy; // dS_dy[2][2]
        dS_dy[i][3] = 2.0 * transvYoung * dE_dy[i][3];                  // dS_dy[0][1]
        dS_dy[i][4] = 2.0 * transvYoung * dE_dy[i][4];                  // dS_dy[0][2]
        dS_dy[i][5] = 2.0 * transvYoung * dE_dy[i][5];                  // dS_dy[1][2]
    }
} // output

RigidWall::RigidWall(const double &density) : Material(density, MaterialType::RIGID)
{
}
RigidWall::~RigidWall()
{
}

void RigidWall::getPlaneStressTensor(const double E[3],        // input
                                     const double dx_dy[2][2], // input
                                     double S[3]) const
{
} // output

void RigidWall::getPlaneStressTensorDerivative(const double dE_dy[3],    // input
                                               const double dx_dy[2][2], // input
                                               double dS_dy[3]) const
{
} // output

void RigidWall::getPlaneStressTensorDerivative(int ndofs,                // input
                                               const double dE_dy[][3],  // input
                                               const double dx_dy[2][2], // input
                                               double dS_dy[][3]) const
{
} // output

void RigidWall::getPlaneStressTensorAndDerivative(int ndofs,                // input
                                                  const double E[3],        // input
                                                  const double dE_dy[][3],  // input
                                                  const double dx_dy[2][2], // input
                                                  double S[3],              // output
                                                  double dS_dy[][3]) const
{
} // output

void RigidWall::getStressTensor(const double E[6],        // input
                                const double dx_dy[3][3], // input
                                double S[6]) const
{
} // output

void RigidWall::getStressTensorDerivative(const double dE_dy[6],    // input
                                          const double dx_dy[3][3], // input
                                          double dS_dy[6]) const
{
} // output

void RigidWall::getStressTensorDerivative(int ndofs,                // input
                                          const double dE_dy[][6],  // input
                                          const double dx_dy[3][3], // input
                                          double dS_dy[][6]) const
{
} // output

void RigidWall::getStressTensorAndDerivative(int ndofs,                // input
                                             const double E[6],        // input
                                             const double dE_dy[][6],  // input
                                             const double dx_dy[3][3], // input
                                             double S[6],              // output
                                             double dS_dy[][6]) const
{
}