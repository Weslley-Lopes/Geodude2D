//#include "src/SpaceTimeDomain.h"

//--------------------------------------------------------
// EXEMPLO 1 - MARQUES (2006)
// Distância até o contato = 0,05
// Velocidade da barra = 0,5
// Intervalo de tempo = 0,01
// Distância percorrida por passo de tempo = 0,005
// Número de passos de tempo para contato = 10
//--------------------------------------------------------
ContactGeometry *solid_geo = new ContactGeometry(0);

Point *p0 = solid_geo->addPoint({0.0, 0.0, 0.0});
Point *p1 = solid_geo->addPoint({1.0, 0.0, 0.0});
Point *p2 = solid_geo->addPoint({1.0, 1.0, 0.0});
Point *p3 = solid_geo->addPoint({0.0, 1.0, 0.0});

Line *l0 = solid_geo->addLine({p0, p1});
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addLine({p2, p3});
Line *l3 = solid_geo->addLine({p3, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l3});

solid_geo->transfiniteLine({l0}, 2);
solid_geo->transfiniteLine({l1}, 2);
solid_geo->transfiniteLine({l2}, 2);
solid_geo->transfiniteLine({l3}, 2);

solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, ConstrainedDOF::X, 0.5);

solid_geo->setProjectileInterfaces({l1});

// -------------------------------------------------------------------------------
// Contact data
Point *p4 = solid_geo->addPoint({1.05, -0.50, 0.0});
Point *p5 = solid_geo->addPoint({2.05, -0.50, 0.0});
Point *p6 = solid_geo->addPoint({2.05, 1.5, 0.0});
Point *p7 = solid_geo->addPoint({1.05, 1.5, 0.0});

Line *l4 = solid_geo->addLine({p4, p5});
Line *l5 = solid_geo->addLine({p5, p6});
Line *l6 = solid_geo->addLine({p6, p7});
Line *l7 = solid_geo->addLine({p7, p4});

solid_geo->transfiniteLine({l4}, 2);
solid_geo->transfiniteLine({l5}, 2);
solid_geo->transfiniteLine({l6}, 2);
solid_geo->transfiniteLine({l7}, 2);

Surface *s1 = solid_geo->addPlaneSurface({l4, l5, l6, l7});

solid_geo->transfiniteLine({l4}, 2);
solid_geo->transfiniteLine({l5}, 2);
solid_geo->transfiniteLine({l6}, 2);
solid_geo->transfiniteLine({l7}, 2);

solid_geo->addDirichletBoundaryCondition({l4, l5, l6, l7}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);

solid_geo->setTargetInterfaces({l7});

// -------------------------------------------------------------------------------

Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 1.0, 0.0, 1.0);

SolidDomain *solid_problem = new SolidDomain(solid_geo);

solid_problem->applyMaterial({s0}, mat);
solid_problem->applyMaterial({s1}, mat);
solid_problem->generateMesh(T3, FRONT, "test", "", true, false);
solid_problem->buildFullContactProblem();

solid_problem->setNumberOfSteps(100);
solid_problem->setExportFrequency(1);
solid_problem->setDeltat(0.01);
solid_problem->setLumpedMass(false);
solid_problem->setSpectralRadius(0.1);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, 0.0, 0.0);

solid_problem->addGraphic("disp-A", DISPLACEMENT, X, "p1");
solid_problem->addGraphic("veloc-A", VELOCITY, X, "p1");
solid_problem->addGraphic("stress-A", CAUCHY_STRESS, X, "p1");
solid_problem->solveContactProblem();