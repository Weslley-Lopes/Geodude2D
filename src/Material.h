#pragma once

#include <iostream>
#include <cmath>

enum class MaterialType
{
	ELASTIC_SOLID,
	ELASTIC_INCOMPRESSIBLE_SOLID,
	NEWTONIAN_INCOMPRESSIBLE_FLUID,
	RIGID
};

enum ConstitutiveModel
{
	SAINT_VENANT_KIRCHHOFF,
	NEO_HOOKEAN
};

enum PlaneAnalysis
{
	PLANE_STRESS,
	PLANE_STRAIN
};

class Material
{
public:
	Material(const double &density,
			 const MaterialType &type,
			 const PlaneAnalysis &planeAnalysis = PLANE_STRESS);

	virtual ~Material() = default;

	double getDensity() const;

	MaterialType getType() const;

	PlaneAnalysis getPlaneAnalysis() const;

	void setDensity(const double &density);

	void setType(const MaterialType &type);

	void setPlaneAnalysis(const PlaneAnalysis &planeAnalysis);

	// virtual double getYoung() const = 0;

	// virtual double getPoisson() const = 0;

	virtual void getPlaneStressTensor(const double E[3],		// input
									  const double dx_dy[2][2], // input
									  double S[3]) const = 0;	// output

	virtual void getPlaneStressTensorDerivative(const double dE_dy[3],		// input
												const double dx_dy[2][2],	// input
												double dS_dy[3]) const = 0; // output

	virtual void getPlaneStressTensorDerivative(int ndofs,					  // input
												const double dE_dy[][3],	  // input
												const double dx_dy[2][2],	  // input
												double dS_dy[][3]) const = 0; // output

	virtual void getPlaneStressTensorAndDerivative(int ndofs,					 // input
												   const double E[3],			 // input
												   const double dE_dy[][3],		 // input
												   const double dx_dy[2][2],	 // input
												   double S[3],					 // output
												   double dS_dy[][3]) const = 0; // output

	virtual void getStressTensor(const double E[6],		   // input
								 const double dx_dy[3][3], // input
								 double S[6]) const = 0;   // output

	virtual void getStressTensorDerivative(const double dE_dy[6],	   // input
										   const double dx_dy[3][3],   // input
										   double dS_dy[6]) const = 0; // output

	virtual void getStressTensorDerivative(int ndofs,					 // input
										   const double dE_dy[][6],		 // input
										   const double dx_dy[3][3],	 // input
										   double dS_dy[][6]) const = 0; // output

	virtual void getStressTensorAndDerivative(int ndofs,					// input
											  const double E[6],			// input
											  const double dE_dy[][6],		// input
											  const double dx_dy[3][3],		// input
											  double S[6],					// output
											  double dS_dy[][6]) const = 0; // output

protected:
	double density_;
	MaterialType type_;
	PlaneAnalysis planeAnalysis_;
};

class ElasticSolid : public Material
{
public:
	ElasticSolid(const ConstitutiveModel &model,
				 const double &young,
				 const double &poisson,
				 const double &density);

	~ElasticSolid();

	void setYoung(const double &young);

	void setPoisson(const double &poisson);

	double getYoung() const;

	double getPoisson() const;

	double getFrictionCoefficient() const;

	void getPlaneStressTensor(const double E[3],
							  const double dx_dy[2][2],
							  double S[3]) const override;

	void getPlaneStressTensorDerivative(const double dE_dy[3],
										const double dx_dy[2][2],
										double dS_dy[3]) const override;

	void getPlaneStressTensorDerivative(int ndofs,
										const double dE_dy[][3],
										const double dx_dy[2][2],
										double dS_dy[][3]) const override;

	void getPlaneStressTensorAndDerivative(int ndofs,						  // input
										   const double E[3],				  // input
										   const double dE_dy[][3],			  // input
										   const double CI[2][2],			  // input
										   double S[3],						  // output
										   double dS_dy[][3]) const override; // output

	void getStressTensor(const double E[6],
						 const double dx_dy[3][3],
						 double S[6]) const override;

	void getStressTensorDerivative(const double dE_dy[6],
								   const double dx_dy[3][3],
								   double dS_dy[6]) const override;

	void getStressTensorDerivative(int ndofs,
								   const double dE_dy[][6],
								   const double dx_dy[3][3],
								   double dS_dy[][6]) const override;

	void getStressTensorAndDerivative(int ndofs,						 // input
									  const double E[6],				 // input
									  const double dE_dy[][6],			 // input
									  const double dx_dy[3][3],			 // input
									  double S[6],						 // output
									  double dS_dy[][6]) const override; // output

private:
	double young_;
	double poisson_;
	ConstitutiveModel model_;
};

class NewtonianFluid : public Material
{
public:
	NewtonianFluid(const double &viscosity,
				   const double &density);

	~NewtonianFluid();

	double getViscosity() const;

	void setViscosity(const double &viscosity);

	void getPlaneStressTensor(const double dE_dt[3],
							  const double CI[2][2],
							  double S[3]) const override;

	void getPlaneStressTensorDerivative(const double dE_dtdy[3],
										const double dx_dy[2][2],
										double dS_dy[3]) const override;

	void getPlaneStressTensorDerivative(int ndofs,
										const double dE_dtdy[][3],
										const double dx_dy[2][2],
										double dS_dy[][3]) const override;

	void getPlaneStressTensorAndDerivative(int ndofs,						  // input
										   const double dE_dt[3],			  // input
										   const double dE_dtdy[][3],		  // input
										   const double dx_dy[2][2],		  // input
										   double S[3],						  // output
										   double dS_dy[][3]) const override; // output

	void getStressTensor(const double dE_dt[6],
						 const double dx_dy[3][3],
						 double S[6]) const override;

	void getStressTensorDerivative(const double dE_dtdy[6],
								   const double dx_dy[3][3],
								   double dS_dy[6]) const override;

	void getStressTensorDerivative(int ndofs,
								   const double dE_dtdy[][6],
								   const double dx_dy[3][3],
								   double dS_dy[][6]) const override;

	void getStressTensorAndDerivative(int ndofs,						 // input
									  const double dE_dt[6],			 // input
									  const double dE_dtdy[][6],		 // input
									  const double dx_dy[3][3],			 // input
									  double S[6],						 // output
									  double dS_dy[][6]) const override; // output
private:
	double viscosity_;
};

class RigidParticle : public Material
{
public:
	RigidParticle(const double &young,
				  const double &poisson,
				  const double &density,
				  const double &normalDampingRatio,
				  const double &tangentialDampingRatio);

	~RigidParticle();

	double getYoung() const;

	double getPoisson() const;

	double getStaticFrictionCoefficient() const;

	double getDynamicFrictionCoefficient() const;

	double getRollingFrictionCoefficient() const;

	double getNormalDampingRatio() const;

	double getTangentialDampingRatio() const;

	double getStiffnessProportionalityConstant() const;

	void getPlaneStressTensor(const double E[3],		   // input
							  const double dx_dy[2][2],	   // input
							  double S[3]) const override; // output

	void getPlaneStressTensorDerivative(const double dE_dy[3],			 // input
										const double dx_dy[2][2],		 // input
										double dS_dy[3]) const override; // output

	void getPlaneStressTensorDerivative(int ndofs,						   // input
										const double dE_dy[][3],		   // input
										const double dx_dy[2][2],		   // input
										double dS_dy[][3]) const override; // output

	void getPlaneStressTensorAndDerivative(int ndofs,						  // input
										   const double E[3],				  // input
										   const double dE_dy[][3],			  // input
										   const double dx_dy[2][2],		  // input
										   double S[3],						  // output
										   double dS_dy[][3]) const override; // output

	void getStressTensor(const double E[6],			  // input
						 const double dx_dy[3][3],	  // input
						 double S[6]) const override; // output

	void getStressTensorDerivative(const double dE_dy[6],			// input
								   const double dx_dy[3][3],		// input
								   double dS_dy[6]) const override; // output

	void getStressTensorDerivative(int ndofs,						  // input
								   const double dE_dy[][6],			  // input
								   const double dx_dy[3][3],		  // input
								   double dS_dy[][6]) const override; // output

	void getStressTensorAndDerivative(int ndofs,						 // input
									  const double E[6],				 // input
									  const double dE_dy[][6],			 // input
									  const double dx_dy[3][3],			 // input
									  double S[6],						 // output
									  double dS_dy[][6]) const override; // output

private:
	double young_;
	double poisson_;
	double normalDampingRatio_;
	double tangentialDampingRatio_;
	double stiffnessProportionalityConstant_;
};

class RigidWall : public Material
{
public:
	RigidWall(const double &density = 0);
	~RigidWall();

	void getPlaneStressTensor(const double E[3],		   // input
							  const double dx_dy[2][2],	   // input
							  double S[3]) const override; // output

	void getPlaneStressTensorDerivative(const double dE_dy[3],			 // input
										const double dx_dy[2][2],		 // input
										double dS_dy[3]) const override; // output

	void getPlaneStressTensorDerivative(int ndofs,						   // input
										const double dE_dy[][3],		   // input
										const double dx_dy[2][2],		   // input
										double dS_dy[][3]) const override; // output

	void getPlaneStressTensorAndDerivative(int ndofs,						  // input
										   const double E[3],				  // input
										   const double dE_dy[][3],			  // input
										   const double dx_dy[2][2],		  // input
										   double S[3],						  // output
										   double dS_dy[][3]) const override; // output

	void getStressTensor(const double E[6],			  // input
						 const double dx_dy[3][3],	  // input
						 double S[6]) const override; // output

	void getStressTensorDerivative(const double dE_dy[6],			// input
								   const double dx_dy[3][3],		// input
								   double dS_dy[6]) const override; // output

	void getStressTensorDerivative(int ndofs,						  // input
								   const double dE_dy[][6],			  // input
								   const double dx_dy[3][3],		  // input
								   double dS_dy[][6]) const override; // output

	void getStressTensorAndDerivative(int ndofs,						 // input
									  const double E[6],				 // input
									  const double dE_dy[][6],			 // input
									  const double dx_dy[3][3],			 // input
									  double S[6],						 // output
									  double dS_dy[][6]) const override; // output
};