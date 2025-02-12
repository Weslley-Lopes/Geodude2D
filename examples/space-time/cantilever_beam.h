// ======================================================================================================
// VIGA COM CARREGAMENTO UNIFORMEMENTE DISTRIBUÍDO - BATHE (1975)
//=======================================================================================================
ContactGeometry *solid_geo = new ContactGeometry(0);

Point *p0 = solid_geo->addPoint({0.0, 0.0, 0.0});
Point *p1 = solid_geo->addPoint({10.0, 0.0, 0.0});
Point *p2 = solid_geo->addPoint({10.0, 1.0, 0.0});
Point *p3 = solid_geo->addPoint({0.0, 1.0, 0.0});

Line *l0 = solid_geo->addLine({p0, p1});
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addLine({p2, p3});
Line *l3 = solid_geo->addLine({p3, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l3});

solid_geo->transfiniteLine({l0}, 25);
solid_geo->transfiniteLine({l1}, 3);
solid_geo->transfiniteLine({l2}, 25);
solid_geo->transfiniteLine({l3}, 3);
//solid_geo->transfiniteSurface({s0}, "Right", {p0, p1, p2, p3});

solid_geo->addDirichletBoundaryCondition({l3}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);
solid_geo->addNeumannBoundaryCondition({l2}, 0.0, -2.85, 0.0);
solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);

Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 1.2e4, 0.2, 1.e-6);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, FUNG4, TWO_FIELDS, 4.5e-5);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain(solid_geo, time_domain);

solid_problem->applyMaterial({s0}, mat);
solid_problem->setForceType(CONSTANT);
solid_problem->generateMesh(T6, FRONT, "teste", "", true, false);
solid_problem->setNumberOfSteps(267);
solid_problem->setExportFrequency(1);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, 0.0, 0.0);

solid_problem->addGraphic("disp-A", DISPLACEMENT, Y, "p1");
solid_problem->showIterativeInformation(false);
solid_problem->solveTransientProblem();