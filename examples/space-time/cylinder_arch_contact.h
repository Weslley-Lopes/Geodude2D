//============================================================
// Cylindrical shell with dynamic snap through - Darcy(2021)
//============================================================
ContactGeometry *solid_geo = new ContactGeometry(0);

//--------------------------------------------------------------------------------------------------------------------
// PROJECTILE SOLID

Point *p0 = solid_geo->addPoint({0.0, 18.0, 0.0},1.0,false);
Point *p1 = solid_geo->addPoint({0.0, 13.0, 0.0});
Point *p2 = solid_geo->addPoint({0.0, 23.0, 0.0});

Line *l0 = solid_geo->addCircle({p1, p0, p2});
Line *l1 = solid_geo->addCircle({p2, p0, p1});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1});

solid_geo->transfiniteLine({l0}, 17);
solid_geo->transfiniteLine({l1}, 17);

solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, ConstrainedDOF::X, 0.0);

solid_geo->setProjectileInterfaces({l0});
solid_geo->setFallingSurface(s0);
//--------------------------------------------------------------------------------------------------------------------
// TARGET SOLID

Point *p3 = solid_geo->addPoint({10.0, 0.0, 0.0});
Point *p4 = solid_geo->addPoint({-10.0, 0.0, 0.0});
Point *p5 = solid_geo->addPoint({12.0, 0.0, 0.0});
Point *p6 = solid_geo->addPoint({-12.0, 0.0, 0.0});

Line *l2 = solid_geo->addSpline({p5, p6}, [](double x) { return (std::sqrt(std::pow(12.0,2.0) - x*x)); }, 1000);
Line *l3 = solid_geo->addLine({p6, p4});
Line *l4 = solid_geo->addSpline({p4, p3}, [](double x) { return (std::sqrt(std::pow(10.0,2.0) - x*x)); }, 1000);
Line *l5 = solid_geo->addLine({p3, p5});

Surface *s1 = solid_geo->addPlaneSurface({l2, l3, l4, l5});

solid_geo->transfiniteLine({l2}, 14);
solid_geo->transfiniteLine({l3}, 2);
solid_geo->transfiniteLine({l4}, 14);
solid_geo->transfiniteLine({l5}, 2);

solid_geo->addDirichletBoundaryCondition({l3}, Variable::ALL_VARIABLES, ConstrainedDOF::X, 0.0);
solid_geo->addDirichletBoundaryCondition({l5}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s1}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s1}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);

solid_geo->setTargetInterfaces({l2});
solid_geo->setFixedSurface(s1);
//--------------------------------------------------------------------------------------------------------------------

Material *mat = new ElasticSolid(NEO_HOOKEAN, 698.56, 0.32, 0.25);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, P2, TWO_FIELDS, 1.e-3);
time_domain->setInContactIntegratorType(FUNG3);
time_domain->setInReleaseIntegratorType(FUNG3);
time_domain->setFungParameter(4./3.);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain(solid_geo,time_domain);

solid_problem->applyMaterial({s0}, mat);
solid_problem->applyMaterial({s1}, mat);
solid_problem->setForceType(CONSTANT);
solid_problem->generateMesh(T10, FRONT, "teste", "", true, false);
solid_problem->buildFullContactProblem();
solid_problem->setNumberOfSteps(4000);
solid_problem->setExportFrequency(20);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, -9.81, 0.0);
solid_problem->showIterativeInformation(false);
solid_problem->solveContactProblem();