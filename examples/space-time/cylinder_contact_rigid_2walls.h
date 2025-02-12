//============================================================
// Cylindrical shell with dynamic snap through - Darcy(2021)
//============================================================
ContactGeometry *solid_geo = new ContactGeometry(0);

//--------------------------------------------------------------------------------------------------------------------
// PROJECTILE SOLID

/*Point *p0 = solid_geo->addPoint({0.0, 60., 0.0});
Point *p1 = solid_geo->addPoint({10., 70., 0.0});
Point *p2 = solid_geo->addPoint({0.0, 80., 0.0});

Line *l0 = solid_geo->addSpline({p0, p1}, [](double x) { return (-std::sqrt(std::pow(10.,2.0) - x*x)) + 70.; }, 1000);
Line *l1 = solid_geo->addSpline({p1, p2}, [](double x) { return (std::sqrt(std::pow(10.,2.0) - x*x)) + 70.; }, 1000);
Line *l2 = solid_geo->addLine({p2, p0});*/

Point *p0 = solid_geo->addPoint({0.15, 0.7, 0.0},1.0,false);
Point *p1 = solid_geo->addPoint({0.15, 0.6, 0.0});
Point *p2 = solid_geo->addPoint({0.15, 0.8, 0.0});

Line *l0 = solid_geo->addCircle({p1, p0, p2});
Line *l1 = solid_geo->addCircle({p2, p0, p1});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1});

solid_geo->transfiniteLine({l0}, 17);
solid_geo->transfiniteLine({l1}, 17);

solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);

solid_geo->setProjectileInterfaces({l0, l1});
solid_geo->setFallingSurface(s0);

//--------------------------------------------------------------------------------------------------------------------
// TARGET SOLID

// RAMP
Point *p3 = solid_geo->addPoint({-0.2, 0.0, 0.0});
Point *p4 = solid_geo->addPoint({0.3, 0.0, 0.0});
Point *p5 = solid_geo->addPoint({0.0, 0.2, 0.0});
Point *p6 = solid_geo->addPoint({-0.2, 0.2, 0.0});

Line *l3 = solid_geo->addLine({p3, p4});
Line *l4 = solid_geo->addLine({p4, p5});
Line *l5 = solid_geo->addLine({p5, p6});
Line *l6 = solid_geo->addLine({p6, p3});

Surface *s1 = solid_geo->addPlaneSurface({l3, l4, l5, l6});

solid_geo->transfiniteLine({l3}, 11);
solid_geo->transfiniteLine({l4}, 7);
solid_geo->transfiniteLine({l5}, 5);
solid_geo->transfiniteLine({l6}, 5);

solid_geo->addInitialCondition({s1}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s1}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);
solid_geo->addDirichletBoundaryCondition({s1}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);

solid_geo->setTargetInterfaces({l4, l5});

// WALL
Point *p7 = solid_geo->addPoint({0.4, 0.0, 0.0});
Point *p8 = solid_geo->addPoint({0.6, 0.0, 0.0});
Point *p9 = solid_geo->addPoint({0.6, 0.5, 0.0});
Point *p10 = solid_geo->addPoint({0.4, 0.5, 0.0});

Line *l7 = solid_geo->addLine({p7, p8});
Line *l8 = solid_geo->addLine({p8, p9});
Line *l9 = solid_geo->addLine({p9, p10});
Line *l10 = solid_geo->addLine({p10, p7});

Surface *s2 = solid_geo->addPlaneSurface({l7, l8, l9, l10});

solid_geo->transfiniteLine({l7}, 5);
solid_geo->transfiniteLine({l8}, 11);
solid_geo->transfiniteLine({l9}, 5);
solid_geo->transfiniteLine({l10}, 11);

solid_geo->addInitialCondition({s2}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s2}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);
solid_geo->addDirichletBoundaryCondition({s2}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);

solid_geo->setTargetInterfaces({l10});
//--------------------------------------------------------------------------------------------------------------------

//Material *mat = new ElasticSolid(NEO_HOOKEAN, 150., 0.4, 0.25);
Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 150., 0.4, 0.25);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, FUNG4, TWO_FIELDS, 1.0e-3);
//time_domain->setInContactIntegratorType(FUNG4);
//time_domain->setInReleaseIntegratorType(FUNG4);
time_domain->setInContactIntegratorType(FUNG3);
time_domain->setInReleaseIntegratorType(FUNG3);
time_domain->setFungParameter(2.);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain(solid_geo,time_domain);

solid_problem->applyMaterial({s0}, mat);
solid_problem->applyMaterial({s1}, mat);
solid_problem->applyMaterial({s2}, mat);
solid_problem->setForceType(CONSTANT);
solid_problem->generateMesh(T6, FRONT, "test", "", true, false);
solid_problem->buildFullContactProblem();
solid_problem->setNumberOfSteps(1000);
solid_problem->setExportFrequency(1);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, -9.81, 0.0);
solid_problem->showIterativeInformation(false);

solid_problem->addGraphic("disp-A", DISPLACEMENT, Y, "p1");
solid_problem->addGraphic("stress11-A", CAUCHY_STRESS, X, "p1");
solid_problem->addGraphic("stress22-A", CAUCHY_STRESS, Y, "p1");
solid_problem->addGraphic("stress12-A", CAUCHY_STRESS, Z, "p1");
solid_problem->solveContactProblem();

// Se houver algum sólido em queda, utilizar setFallingSurface()
//Se houver algum sólido fixo, utilizar setFixedSurface()