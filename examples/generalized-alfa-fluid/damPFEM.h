Geometry *dam_geo = new Geometry(0);

Point *p0 = dam_geo->addPoint({0.0  , 0.0  , 0.0});
Point *p1 = dam_geo->addPoint({0.146, 0.0  , 0.0});
Point *p2 = dam_geo->addPoint({0.146, 0.292, 0.0});
Point *p3 = dam_geo->addPoint({0.0  , 0.292, 0.0});
Point *p4 = dam_geo->addPoint({0.0  , 0.584, 0.0});
Point *p5 = dam_geo->addPoint({0.584, 0.0  , 0.0});
Point *p6 = dam_geo->addPoint({0.584, 0.584, 0.0});

Line *l0 = dam_geo->addLine({p0, p1});
Line *l1 = dam_geo->addLine({p1, p2});
Line *l2 = dam_geo->addLine({p2, p3});
Line *l3 = dam_geo->addLine({p3, p0});
Line *l4 = dam_geo->addLine({p1, p5});
Line *l5 = dam_geo->addLine({p5, p6});
Line *l6 = dam_geo->addLine({p3, p4});

Surface *s0 = dam_geo->addPlaneSurface({l0, l1, l2, l3});

int h = 24;
dam_geo->transfiniteLine({l0}, h+1);
dam_geo->transfiniteLine({l1}, 2*h+1);
dam_geo->transfiniteLine({l2}, h+1);
dam_geo->transfiniteLine({l3}, 2*h+1);
dam_geo->transfiniteLine({l4}, 3*h+1);
dam_geo->transfiniteLine({l5}, 4*h+1);
dam_geo->transfiniteLine({l6}, 2*h+1);
//dam_geo->transfiniteSurface({s0});

dam_geo->addDirichletBoundaryCondition({l0}, ALL, 0.0);
dam_geo->addDirichletBoundaryCondition({l3}, ALL, 0.0);
dam_geo->addDirichletBoundaryCondition({l4}, ALL, 0.0);
dam_geo->addDirichletBoundaryCondition({l5}, ALL, 0.0);
dam_geo->addDirichletBoundaryCondition({l6}, ALL, 0.0);

Material *mat = new NewtonianFluid(0.001, 1000.0);

FluidDomain *dam_problem = new FluidDomain(dam_geo);

dam_problem->applyMaterial({s0}, mat);
dam_problem->generateMesh(T3, FRONT, "", "", false, false);
dam_problem->setNumberOfSteps(1000);
dam_problem->setDeltat(0.001);
dam_problem->setMaxNonlinearIterations(6);
dam_problem->setNonlinearTolerance(1.0e-6);
dam_problem->setGravity(0.0, -9.81, 0.0);
dam_problem->setSpectralRadius(0.8);
dam_problem->setMeshLength(0.146/double(h));
dam_problem->solvePFEMProblem();