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

Point *p0 = solid_geo->addPoint({0.0, 0.6, 0.0});
Point *p1 = solid_geo->addPoint({0.1, 0.6, 0.0});
Point *p2 = solid_geo->addPoint({0.1, 0.8, 0.0});
Point *p7 = solid_geo->addPoint({0.0, 0.8, 0.0});

Line *l0 = solid_geo->addLine({p0, p1});
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addLine({p2, p7});
Line *l7 = solid_geo->addLine({p7, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l7});

solid_geo->transfiniteLine({l0}, 5);
solid_geo->transfiniteLine({l1}, 9);
solid_geo->transfiniteLine({l2}, 5);
solid_geo->transfiniteLine({l7}, 9);

solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, PerturbedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, PerturbedDOF::ALL, 0.0);
solid_geo->addDirichletBoundaryCondition({l1}, X, 0.0);
solid_geo->addDirichletBoundaryCondition({l2}, X, 0.0);

solid_geo->setProjectileInterfaces({l0});
solid_geo->setFallingSurface(s0);

//--------------------------------------------------------------------------------------------------------------------
// TARGET SOLID

Point *p3 = solid_geo->addPoint({0.0, 0.0, 0.0});
Point *p4 = solid_geo->addPoint({0.2, 0.0, 0.0});
Point *p5 = solid_geo->addPoint({0.2, 0.2, 0.0});
Point *p6 = solid_geo->addPoint({0.0, 0.2, 0.0});

Line *l3 = solid_geo->addLine({p3, p4});
Line *l4 = solid_geo->addLine({p4, p5});
Line *l5 = solid_geo->addLine({p5, p6});
Line *l6 = solid_geo->addLine({p6, p3});

Surface *s1 = solid_geo->addPlaneSurface({l3, l4, l5, l6});

solid_geo->transfiniteLine({l3}, 5);
solid_geo->transfiniteLine({l4}, 5);
solid_geo->transfiniteLine({l5}, 5);
solid_geo->transfiniteLine({l6}, 5);

solid_geo->addInitialCondition({s1}, Variable::DISPLACEMENT, PerturbedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s1}, Variable::VELOCITY, PerturbedDOF::ALL, 0.0);
solid_geo->addDirichletBoundaryCondition({l3}, ALL, 0.0);
solid_geo->addDirichletBoundaryCondition({l6}, X, 0.0);

solid_geo->setTargetInterfaces({l5});
solid_geo->setFixedSurface(s1);
//--------------------------------------------------------------------------------------------------------------------

Material *mat = new ElasticSolid(NEO_HOOKEAN, 43.9535, 0.465116, 0.25);
//Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 43.9535, 0.465116, 0.25);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, P2, TWO_FIELDS, 1.0e-5);
time_domain->setInContactIntegratorType(P2);
time_domain->setInReleaseIntegratorType(P2);
time_domain->setFungParameter(2.0);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain(solid_geo,time_domain);

solid_problem->applyMaterial({s0}, mat);
solid_problem->applyMaterial({s1}, mat);
solid_problem->setForceType(CONSTANT);
solid_problem->generateMesh(T6, FRONT, "teste", "", true, false);
solid_problem->buildFullContactProblem();
solid_problem->setNumberOfSteps(400000);
solid_problem->setExportFrequency(200);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, -9.81, 0.0);
solid_problem->showIterativeInformation(false);
solid_problem->solveContactProblem();

// Se houver algum sólido em queda, utilizar setFallingSurface()
// Se houver algum sólido fixo, utilizar setFixedSurface()