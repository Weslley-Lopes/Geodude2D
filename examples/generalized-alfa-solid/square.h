Geometry *solid_geo = new Geometry(0);

Point *p0 = solid_geo->addPoint({0.0, 0.0, 0.0});
Point *p1 = solid_geo->addPoint({1.0, 0.0, 0.0});
Point *p2 = solid_geo->addPoint({1.0, .4, 0.0});
Point *p3 = solid_geo->addPoint({0.0, 0.4, 0.0});

Line *l0 = solid_geo->addLine({p0, p1});
Line *l1 = solid_geo->addLine({p1, p2});
Line *l2 = solid_geo->addLine({p2, p3});
Line *l3 = solid_geo->addLine({p3, p0});

Surface *s0 = solid_geo->addPlaneSurface({l0, l1, l2, l3});

solid_geo->transfiniteLine({l0}, 2);
solid_geo->transfiniteLine({l1}, 2);
solid_geo->transfiniteLine({l2}, 2);
solid_geo->transfiniteLine({l3}, 2);
//solid->transfiniteSurface({s0}, "Right", {p0, p1, p2, p3});

solid_geo->addDirichletBoundaryCondition({p0}, ALL, 0.0);
solid_geo->addDirichletBoundaryCondition({p3}, X, 0.0);
solid_geo->addNeumannBoundaryCondition({l1}, 5.e6, 0.0, 0.0);

Material *mat = new ElasticSolid(200.e9, 0.0, 0.0);

SolidDomain *solid_problem = new SolidDomain(solid_geo);

solid_problem->applyMaterial({s0}, mat);
solid_problem->generateMesh(T10, FRONT, "teste", "", true, false);
solid_problem->setNumberOfSteps(1);
solid_problem->setDeltat(1.e-4);
solid_problem->setLumpedMass(false);
solid_problem->setSpectralRadius(0.5);
solid_problem->setMaxNonlinearIterations(100);
solid_problem->setNonlinearTolerance(1.0e-6);
solid_problem->setGravity(0.0, 0.0, 0.0);
solid_problem->solveTransientProblem();