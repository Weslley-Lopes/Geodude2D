// ======================================================================================================
// CRITICAL LOAD PROBLEM
//=======================================================================================================
#include <string>
#include "../src/SolidDomain.h"
// INPUT DATA
// dimensions
double b = 1.0;
double L = 10.0;
double I = b / 12.0;
double A = b;

// material properties
double E = 1.0; // young modulus
double nu = 0.2; // poisson coefficient
double rho = 1.0; // density

// load factor
double factor = 1.1;

// CALCULATED DATA
double pi = 3.14159265358979323846;
double Pcr = pi*pi*E*I / (4*L*L);

// loads
double qA = factor * Pcr;
double P = 0.01 * qA; // point load
double q = -qA / A;

// SOLID PROBLEM DEFINITION
Geometry *solid_geo = new Geometry(0);

Point *p0 = solid_geo->addPoint({0.0, 0.0, 0.0});
Point *p1 = solid_geo->addPoint({b, 0.0, 0.0});
Point *p2 = solid_geo->addPoint({b, L, 0.0});
Point *p3 = solid_geo->addPoint({0.0, L, 0.0});

Line *l0 = solid_geo->addLine({p0, p1});
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addLine({p2, p3});
Line *l3 = solid_geo->addLine({p3, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l3});

solid_geo->transfiniteLine({l0}, 3);
solid_geo->transfiniteLine({l1}, 21);
solid_geo->transfiniteLine({l2}, 3);
solid_geo->transfiniteLine({l3}, 21);

solid_geo->addDirichletBoundaryCondition({l0}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);
solid_geo->addNeumannBoundaryCondition({l2}, 0.0, q, 0.0);
solid_geo->addNeumannBoundaryCondition({p3}, P, 0.0, 0.0);

Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, E, nu, rho);

SolidDomain *solid_problem = new SolidDomain(solid_geo);

solid_problem->applyMaterial({s0}, mat);
solid_problem->generateMesh(T6, FRONT, "teste", "", true, false);
solid_problem->setNumberOfSteps(1000);
solid_problem->setExportFrequency(1);

solid_problem->setDeltat(1.0);
solid_problem->setLumpedMass(false);
solid_problem->setSpectralRadius(1.0);
solid_problem->setMaxNonlinearIterations(6);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, 0.0, 0.0);

solid_problem->addGraphic("disp-A", DISPLACEMENT, Y, "p1");
solid_problem->solveTransientProblem();

// Chama a função para abrir o ParaView com o arquivo VTU
std::string pastaArquivos = "/home/pcv_180172/SilvioVeras/CPP/Geodude2D/build/Debug/results/solidOutput..vtu";
solid_problem->abrirParaView(SolidDomain::ParaViewInitMode::OPEN_FILE, pastaArquivos);



//---opção para abrir o ParaView com um estado salvo----o estado precisar ser configurado manualmente 1 vez

//std::string pastaArquivos = "/home/pcv_180172/SilvioVeras/CPP/Geodude2D/build/Debug/results/solidOutput.pvsm";
//solid_problem->abrirParaView(SolidDomain::ParaViewInitMode::OPEN_STATE, pastaArquivos);

// opçaõ para abrir o ParaView com um script Python que inicializa o ParaView (trace) e faz a animação* ainda precisa de ajuste

//std::string pastaArquivos = "/home/pcv_180172/SilvioVeras/CPP/Geodude2D/build/Debug/results/solidOutput.py";
//solid_problem->abrirParaView(SolidDomain::ParaViewInitMode::EXE_SCRIPT, pastaArquivos);
