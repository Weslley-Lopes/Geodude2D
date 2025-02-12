ContactGeometry *solid_geo = new ContactGeometry(0);

Point *p0 = solid_geo->addPoint({0.0, 0.0, 0.0});
Point *p1 = solid_geo->addPoint({1.0, 0.0, 0.0});
Point *p2 = solid_geo->addPoint({1.0, 0.05, 0.0});
Point *p3 = solid_geo->addPoint({0.0, 0.05, 0.0});

Line *l0 = solid_geo->addLine({p0, p1});
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addLine({p2, p3});
Line *l3 = solid_geo->addLine({p3, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l3});

solid_geo->transfiniteLine({l0}, 11);
solid_geo->transfiniteLine({l1}, 2);
solid_geo->transfiniteLine({l2}, 11);
solid_geo->transfiniteLine({l3}, 2);

solid_geo->addDirichletBoundaryCondition({l3}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);
solid_geo->addNeumannBoundaryCondition({l1}, 1.0, 0.0, 0.0);
solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);

Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 1.e4, 0.0, 1.0);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, FUNG4, TWO_FIELDS, 1.e-4);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain({solid_geo}, time_domain);

solid_problem->applyMaterial({s0}, mat);
solid_problem->setForceType(CONSTANT);
solid_problem->generateMesh(T6, FRONT, "teste", "", true, false);
solid_problem->setNumberOfSteps(3200);
solid_problem->setExportFrequency(2);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, 0.0, 0.0);
solid_problem->showIterativeInformation(false);

solid_problem->addGraphic("disp-A", DISPLACEMENT, X, "p1");
solid_problem->addGraphic("veloc-A", VELOCITY, X, "p1");
solid_problem->solveTransientProblem();