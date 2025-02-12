#include "AnalysisParameters.h"

AnalysisParameters::AnalysisParameters()
    : dimension_(2),
      numberOfSteps_(1),
      maxNonlinearIterations_(3),
      nonlinearTolerance_(1.0e-6),
      integrationMethod_(0.5),
      deltat_(1.0),
      initialDeltat_(1.0),
      currentTime_(0.0),
      gravity_{0.0, 0.0, 0.0},
      rhoInf_(1.0),
      alphaM_(1.0),
      alphaF_(1.0),
      gamma_(0.5),
      beta_(0.25),
      alpha_(1.2),
      meshLength_(1.0),
      modelVolume_(0.0),
      exportFrequency_(1),
      initialAccel_(false),
      isStaticAnalysis_(false),
      useLumpedMass_(true) {}

AnalysisParameters::~AnalysisParameters() {}

void AnalysisParameters::setDimension(const int dimension)
{
    dimension_ = dimension;
}

void AnalysisParameters::setNumberOfSteps(const int numberOfSteps)
{
    numberOfSteps_ = numberOfSteps;
}

void AnalysisParameters::setMaxNonlinearIterations(const int maxNonlinearIterations)
{
    maxNonlinearIterations_ = maxNonlinearIterations;
}

void AnalysisParameters::setNonlinearTolerance(const double nonlinearTolerance)
{
    nonlinearTolerance_ = nonlinearTolerance;
}

void AnalysisParameters::setErrorTolerance(const double &tol)
{
    errorTolerance_ = tol;
}

void AnalysisParameters::setDeltat(const double &deltat)
{
    deltat_ = deltat;
}

void AnalysisParameters::setInitialDeltat(const double &deltat)
{
    initialDeltat_ = deltat;
}

void AnalysisParameters::setIntegrationMethod(const double &method)
{
    integrationMethod_ = method;
}

void AnalysisParameters::setCurrentTime(const double time)
{
    currentTime_ = time;
}

void AnalysisParameters::incrementTime(const double deltat)
{
    currentTime_ += deltat;
}

void AnalysisParameters::setGravity(const double gravity_x,
                                    const double gravity_y,
                                    const double gravity_z)
{
    gravity_[0] = gravity_x;
    gravity_[1] = gravity_y;
    gravity_[2] = gravity_z;
}

void AnalysisParameters::setSpectralRadius(const double rhoInf)
{
    rhoInf_ = rhoInf;
    alphaM_ = (2.0 - rhoInf_) / (1.0 + rhoInf_);
    alphaF_ = 1.0 / (1.0 + rhoInf_);
    gamma_ = 0.5 + alphaM_ - alphaF_;
    beta_ = 0.25 * (1.0 + alphaM_ - alphaF_) * (1.0 + alphaM_ - alphaF_);

    // Extreme cases 
    // rhoInf = 1.0; alphaM = 0.5; alphaF = 0.5; gamma = 0.5; beta = 0.25
    // rhoInf = 0.0; alphaM = 2.0; alphaF = 1.0; gamma = 1.5; beta = 1.0
}

void AnalysisParameters::setGeneralizedAlphas(const double alphaM, const double alphaF)
{
    alphaM_ = alphaM;
    alphaF_ = alphaF;
}

void AnalysisParameters::setNewmarkParameters(const double beta, const double gamma)
{
    beta_ = beta;
    gamma_ = gamma;
}

void AnalysisParameters::setAlpha(const double alpha)
{
    alpha_ = alpha;
}

void AnalysisParameters::setMeshLength(const double h)
{
    meshLength_ = h;
}

void AnalysisParameters::setCellLength(const double h)
{
    cellLength_ = h;
}

void AnalysisParameters::setModelVolume(const double volume)
{
    modelVolume_ = volume;
}

void AnalysisParameters::setInitialAccel(const bool initialAccel)
{
    initialAccel_ = initialAccel;
}

void AnalysisParameters::setExportFrequency(const int &freq)
{
    exportFrequency_ = freq;
}

void AnalysisParameters::setStaticAnalysis(const bool &isStaticAnalysis)
{
    isStaticAnalysis_ = isStaticAnalysis;
    if (isStaticAnalysis_)
    {
        rhoInf_ = 1.0;
        alphaM_ = 1.0;
        alphaF_ = 1.0;
        gamma_ = 1.0;
        beta_ = 1.0;
    }
}

void AnalysisParameters::setLumpedMass(const bool &useLumpedMass)
{
    useLumpedMass_ = useLumpedMass;
}

int AnalysisParameters::getDimension() const
{
    return dimension_;
}

int AnalysisParameters::getNumberOfSteps() const
{
    return numberOfSteps_;
}

int AnalysisParameters::getMaxNonlinearIterations() const
{
    return maxNonlinearIterations_;
}

double AnalysisParameters::getNonlinearTolerance() const
{
    return nonlinearTolerance_;
}

double AnalysisParameters::getErrorTolerance() const
{
    return errorTolerance_;
}

double AnalysisParameters::getDeltat() const
{
    return deltat_;
}

double AnalysisParameters::getInitialDeltat() const
{
    return initialDeltat_;
}

double AnalysisParameters::getIntegrationMethod() const
{
    return integrationMethod_;
}

double AnalysisParameters::getCurrentTime() const
{
    return currentTime_;
}

double *AnalysisParameters::getGravity()
{
    return gravity_;
}

double AnalysisParameters::getSpectralRadius() const
{
    return rhoInf_;
}

double AnalysisParameters::getAlphaM() const
{
    return alphaM_;
}

double AnalysisParameters::getAlphaF() const
{
    return alphaF_;
}

double AnalysisParameters::getGamma() const
{
    return gamma_;
}

double AnalysisParameters::getBeta() const
{
    return beta_;
}

double AnalysisParameters::getAlpha() const
{
    return alpha_;
}

double AnalysisParameters::getMeshLength() const
{
    return meshLength_;
}

double AnalysisParameters::getCellLength() const
{
    return cellLength_;
}

double AnalysisParameters::getModelVolume() const
{
    return modelVolume_;
}

bool AnalysisParameters::getInitialAccel() const
{
    return initialAccel_;
}

bool AnalysisParameters::isStaticAnalysis() const
{
    return isStaticAnalysis_;
}

int AnalysisParameters::getExportFrequency() const
{
    return exportFrequency_;
}

bool AnalysisParameters::useLumpedMass() const
{
    return useLumpedMass_;
}