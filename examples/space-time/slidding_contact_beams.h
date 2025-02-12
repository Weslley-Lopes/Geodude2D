//--------------------------------------------------------
// EXEMPLO 8 - MARQUES (2006)
// Distância até o contato = 0,01
// Velocidade da barra = 202,2
// Intervalo de tempo = 0,5e-6
// Distância percorrida por passo de tempo = 0,0001011
// Número de passos de tempo para contato = 99
//--------------------------------------------------------
ContactGeometry *solid_geo = new ContactGeometry(0);
// -----------------------------------------------------------------------------------------
// PROJECTILE SOLID

Point *p0 = solid_geo->addPoint({0.0, 0.0, 0.0});
Point *p1 = solid_geo->addPoint({10.0, 0.0, 0.0});
Point *p2 = solid_geo->addPoint({10.0, 1.0, 0.0});
Point *p3 = solid_geo->addPoint({0.0, 1.0, 0.0});

Line *l0 = solid_geo->addLine({p0, p1});
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addLine({p2, p3});
Line *l3 = solid_geo->addLine({p3, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l3});

solid_geo->transfiniteLine({l0}, 22);
solid_geo->transfiniteLine({l1}, 3);
solid_geo->transfiniteLine({l2}, 22);
solid_geo->transfiniteLine({l3}, 3);

solid_geo->addDirichletBoundaryCondition({l3},  Variable::ALL_VARIABLES, ConstrainedDOF::X, 0.0);
solid_geo->addDirichletBoundaryCondition({l3},  Variable::VELOCITY, ConstrainedDOF::Y, -0.1);
solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, ConstrainedDOF::Y, -0.25);

solid_geo->setProjectileInterfaces({l0});

// -----------------------------------------------------------------------------------------
// TARGET SOLID

Point *p4 = solid_geo->addPoint({7.50, -1.1, 0.0});
Point *p5 = solid_geo->addPoint({17.50, -1.1, 0.0});
Point *p6 = solid_geo->addPoint({17.50, -0.1, 0.0});
Point *p7 = solid_geo->addPoint({7.50, -0.1, 0.0});

Line *l4 = solid_geo->addLine({p4, p5});
Line *l5 = solid_geo->addLine({p5, p6});
Line *l6 = solid_geo->addLine({p6, p7});
Line *l7 = solid_geo->addLine({p7, p4});

Surface *s1 = solid_geo->addPlaneSurface({l4, l5, l6, l7});

solid_geo->transfiniteLine({l4}, 22);
solid_geo->transfiniteLine({l5}, 3);
solid_geo->transfiniteLine({l6}, 22);
solid_geo->transfiniteLine({l7}, 3);

solid_geo->addDirichletBoundaryCondition({l5},  Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s1}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s1}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);

solid_geo->setTargetInterfaces({l6});
// -------------------------------------------------------------------------------
//Material *mat = new ElasticSolid(NEO_HOOKEAN, 1000., 0.3, 0.1);
Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 1000., 0.3, 0.1);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, FUNG4, TWO_FIELDS, 1.e-3);
time_domain->setInContactIntegratorType(FUNG3);
time_domain->setInReleaseIntegratorType(FUNG3);
time_domain->setFungParameter(4./3.);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain(solid_geo, time_domain);

solid_problem->applyMaterial({s0}, mat);
solid_problem->applyMaterial({s1}, mat);
solid_problem->setForceType(CONSTANT);
solid_problem->generateMesh(T6, FRONT, "teste", "", true, false);
solid_problem->buildFullContactProblem();
solid_problem->setNumberOfSteps(50000);
solid_problem->setExportFrequency(50);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, 0.0, 0.0);
solid_problem->showIterativeInformation(false);
solid_problem->solveContactProblem();