//============================================================
// Cylindrical shell with dynamic snap through - Siqueira (2019)
//============================================================
ContactGeometry *solid_geo = new ContactGeometry(0);

Point *p0 = solid_geo->addPoint({-2.475, 4.28682575, 0.0});
Point *p1 = solid_geo->addPoint({2.475, 4.28682575,0.0});
Point *p2 = solid_geo->addPoint({2.525, 4.37342829, 0.0});
Point *p3 = solid_geo->addPoint({-2.525, 4.37342829, 0.0});
Point *p4 = solid_geo->addPoint({0.0, 5.05, 0.0}); // Load point

Line *l0 = solid_geo->addSpline({p0, p1}, [](double x) { return (std::sqrt(std::pow(4.95,2.0) - x*x)); }, 1000);
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addSpline({p2, p4}, [](double x) { return (std::sqrt(std::pow(5.05,2.0) - x*x)); }, 1000);
Line *l3 = solid_geo->addSpline({p4, p3}, [](double x) { return (std::sqrt(std::pow(5.05,2.0) - x*x)); }, 1000);
Line *l4 = solid_geo->addLine({p3, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l3, l4});

solid_geo->transfiniteLine({l0}, 21);
solid_geo->transfiniteLine({l1}, 2);
solid_geo->transfiniteLine({l2}, 11);
solid_geo->transfiniteLine({l3}, 11);
solid_geo->transfiniteLine({l4}, 2);
//solid->transfiniteSurface({s0}, "Right", {p5, p6});

solid_geo->addDirichletBoundaryCondition({p0}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);
solid_geo->addDirichletBoundaryCondition({p1}, Variable::ALL_VARIABLES, ConstrainedDOF::ALL, 0.0);
solid_geo->addNeumannBoundaryCondition({p4}, 0.0, -5.e7, 0.0);
solid_geo->addInitialCondition({s0}, Variable::DISPLACEMENT, ConstrainedDOF::ALL, 0.0);
solid_geo->addInitialCondition({s0}, Variable::VELOCITY, ConstrainedDOF::ALL, 0.0);

Material *mat = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 200.e9, 0.25, 1.e4);
mat->setPlaneAnalysis(PLANE_STRAIN);

TimeDomain *time_domain = new TimeDomain(0, HERMITE, FUNG4, TWO_FIELDS, 6.25e-5);

SpaceTimeDomain *solid_problem = new SpaceTimeDomain(solid_geo,time_domain);

solid_problem->applyMaterial({s0}, mat);
solid_problem->setForceType(RAMP);
solid_problem->setForceTimeDuration(0.2);
solid_problem->generateMesh(T6, FRONT, "teste", "", true, false);
solid_problem->setNumberOfSteps(4800);
solid_problem->setExportFrequency(4);
solid_problem->setLumpedMass(false);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, 0.0, 0.0);

solid_problem->addGraphic("disp-A", DISPLACEMENT, Y, "p4");
solid_problem->showIterativeInformation(false);
solid_problem->solveTransientProblem();