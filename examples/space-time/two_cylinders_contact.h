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

Point *p0 = solid_geo->addPoint({0.0, 1.6, 0.0},1.0,false);
Point *p1 = solid_geo->addPoint({0.0, 1.1, 0.0});
Point *p2 = solid_geo->addPoint({0.0, 2.1, 0.0});

Line *l0 = solid_geo->addCircle({p1, p0, p2});
Line *l1 = solid_geo->addLine({p2, p0});
Line *l2 = solid_geo->addLine({p0, p1});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2});

solid_geo->transfiniteLine({l0}, 17);
solid_geo->transfiniteLine({l1}, 5);
solid_geo->transfiniteLine({l2}, 5);

solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, PerturbedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, PerturbedDOF::ALL, 0.0);
//solid_geo->addInitialCondition({s0}, Variable::VELOCITY, PerturbedDOF::Y, -2.0);
solid_geo->addDirichletBoundaryCondition({l1}, X, 0.0);
solid_geo->addDirichletBoundaryCondition({l2}, X, 0.0);

solid_geo->setProjectileInterfaces({l0});
//--------------------------------------------------------------------------------------------------------------------
// TARGET SOLID

Point *p3 = solid_geo->addPoint({0.0, 0.50, 0.0},1.0,false);
Point *p4 = solid_geo->addPoint({0.0, 0.00, 0.0});
Point *p5 = solid_geo->addPoint({0.0, 1.00, 0.0});

Line *l3 = solid_geo->addCircle({p4, p3, p5});
Line *l4 = solid_geo->addLine({p5, p3});
Line *l5 = solid_geo->addLine({p3, p4});

Surface *s1 = solid_geo->addPlaneSurface({l3, l4, l5});

solid_geo->transfiniteLine({l3}, 17);
solid_geo->transfiniteLine({l4}, 5);
solid_geo->transfiniteLine({l5}, 5);

solid_geo->addInitialCondition({s1}, Variable::DISPLACEMENT, PerturbedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s1}, Variable::VELOCITY, PerturbedDOF::ALL, 0.0);
//solid_geo->addInitialCondition({s1}, Variable::VELOCITY, PerturbedDOF::Y, 4.0);
solid_geo->addDirichletBoundaryCondition({s1}, ALL, 0.0);
//solid_geo->addDirichletBoundaryCondition({l4}, X, 0.0);
//solid_geo->addDirichletBoundaryCondition({l5}, X, 0.0);

solid_geo->setTargetInterfaces({l3});
//--------------------------------------------------------------------------------------------------------------------

Material *mat1 = new ElasticSolid(NEO_HOOKEAN, 43.9535, 0.465116, 0.25);
Material *mat2 = new ElasticSolid(NEO_HOOKEAN, 43.9535, 0.465116, 0.25);
//Material *mat1 = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 2000, 0.2, 0.02);
//Material *mat2 = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 1000, 0.4, 0.01);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, P2, TWO_FIELDS, 1.0e-4);
time_domain->setInContactIntegratorType(FUNG3);
time_domain->setInReleaseIntegratorType(FUNG3);
time_domain->setFungParameter(1000.);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain(solid_geo,time_domain);

solid_problem->applyMaterial({s0}, mat1);
solid_problem->applyMaterial({s1}, mat2);
solid_problem->setForceType(CONSTANT);
solid_problem->generateMesh(T6, FRONT, "test", "", true, false);
solid_problem->buildFullContactProblem();
solid_problem->setNumberOfSteps(5000);
solid_problem->setExportFrequency(2);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, -9.81, 0.0);
solid_problem->showIterativeInformation(false);
solid_problem->solveContactProblem();

// Se houver algum sólido em queda livre, utilizar setFallingSurface()
// Se houver algum sólido fixo, enquanto o outro estiver em movimento, utilizar setFixedSurface()