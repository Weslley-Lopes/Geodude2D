// -----------------------------------------------------------------------------------------
int dimension = 2;

// -----------------------------------------------------------------------------------------
// PARTICULATE PROBLEM
// -----------------------------------------------------------------------------------------
// Materials
Material *particleMaterial = new RigidParticle(1.e9, 0.25, 29841.5518298, 0.0, 0.0); // ball with radius = 20 cm and mass = 1000 kg
Material *surfaceMaterial = new RigidWall();

ParticleDomain *particulate_problem = new ParticleDomain(dimension);
particulate_problem->readParticlePackageData("../../particlePackages/2d_two_particles.dat", particleMaterial);

// Friction parameters of each group formed by particle-particle or particle-wall pair
FrictionParameters *parameters = new FrictionParameters(particleMaterial, particleMaterial, 0.0, 0.0, 0.0);
particulate_problem->addFrictionParameters({parameters});

// Analysis parameters
particulate_problem->setErrorTolerance(1.e-9);
particulate_problem->setMaxNonlinearIterations(10);
particulate_problem->setNumberOfSteps(1500);
particulate_problem->setExportFrequency(10);
particulate_problem->setDeltat(1.e-4);
particulate_problem->setSpectralRadius(1.0);
// particulate_problem->setGravity(0.0, 0.0, 0.0);
particulate_problem->addInitialCondition({1.0, 0.0, 0.0}, VELOCITY, 0);
particulate_problem->addInitialCondition({-1.0, 0.0, 0.0}, VELOCITY, 1);
// particulate_problem->addInitialCondition({0.0, 0.0, 125.0}, ANGULAR_VELOCITY);

// Plot definition
OutputGraphic *p0x0 = new OutputGraphic(POSITION, X, 0, 0);
OutputGraphic *p0x1 = new OutputGraphic(POSITION, X, 0, 1);
OutputGraphic *p0x2 = new OutputGraphic(POSITION, X, 0, 2);
OutputGraphic *p0y0 = new OutputGraphic(POSITION, Y, 0, 0);
OutputGraphic *p0y1 = new OutputGraphic(POSITION, Y, 0, 1);
OutputGraphic *p0y2 = new OutputGraphic(POSITION, Y, 0, 2);
OutputGraphic *v0x0 = new OutputGraphic(VELOCITY, X, 0, 0);
OutputGraphic *v0x1 = new OutputGraphic(VELOCITY, X, 0, 1);
OutputGraphic *v0x2 = new OutputGraphic(VELOCITY, X, 0, 2);
OutputGraphic *v0y0 = new OutputGraphic(VELOCITY, Y, 0, 0);
OutputGraphic *v0y1 = new OutputGraphic(VELOCITY, Y, 0, 1);
OutputGraphic *v0y2 = new OutputGraphic(VELOCITY, Y, 0, 2);
OutputGraphic *u0 = new OutputGraphic(STRAIN_ENERGY, 0);
OutputGraphic *k0 = new OutputGraphic(KINETIC_ENERGY, 0);
OutputGraphic *p0 = new OutputGraphic(EXTERNAL_ENERGY, 0);
OutputGraphic *c0 = new OutputGraphic(CONTACT_ENERGY, 0);
OutputGraphic *t0 = new OutputGraphic(TOTAL_ENERGY, 0);

OutputGraphic *p1x0 = new OutputGraphic(POSITION, X, 1, 0);
OutputGraphic *p1x1 = new OutputGraphic(POSITION, X, 1, 1);
OutputGraphic *p1x2 = new OutputGraphic(POSITION, X, 1, 2);
OutputGraphic *p1y0 = new OutputGraphic(POSITION, Y, 1, 0);
OutputGraphic *p1y1 = new OutputGraphic(POSITION, Y, 1, 1);
OutputGraphic *p1y2 = new OutputGraphic(POSITION, Y, 1, 2);
OutputGraphic *v1x0 = new OutputGraphic(VELOCITY, X, 1, 0);
OutputGraphic *v1x1 = new OutputGraphic(VELOCITY, X, 1, 1);
OutputGraphic *v1x2 = new OutputGraphic(VELOCITY, X, 1, 2);
OutputGraphic *v1y0 = new OutputGraphic(VELOCITY, Y, 1, 0);
OutputGraphic *v1y1 = new OutputGraphic(VELOCITY, Y, 1, 1);
OutputGraphic *v1y2 = new OutputGraphic(VELOCITY, Y, 1, 2);
OutputGraphic *u1 = new OutputGraphic(STRAIN_ENERGY, 1);
OutputGraphic *k1 = new OutputGraphic(KINETIC_ENERGY, 1);
OutputGraphic *p1 = new OutputGraphic(EXTERNAL_ENERGY, 1);
OutputGraphic *c1 = new OutputGraphic(CONTACT_ENERGY, 1);
OutputGraphic *t1 = new OutputGraphic(TOTAL_ENERGY, 1);

particulate_problem->addGraphic({p0x0, p0x1, p0x2, p0y0, p0y1, p0y2, v0x0, v0x1, v0x2, v0y0, v0y1, v0y2, u0, k0, p0, c0, t0});
particulate_problem->addGraphic({p1x0, p1x1, p1x2, p1y0, p1y1, p1y2, v1x0, v1x1, v1x2, v1y0, v1y1, v1y2, u1, k1, p1, c1, t1});

// Problem solution
// particulate_problem->solveStaggeredProblem();
particulate_problem->solveTransientProblem();

// Plot results
// Gnuplot *g0 = new Gnuplot();
// g0->plotSeparate(particulate_problem->getOutputGraphics());

Gnuplot *g0 = new Gnuplot();
g0->plotTogether({p0x0, p0x1, p0x2});
// g0->view();
g0->plotTogether({p0y0, p0y1, p0y2});
// g0->view();
g0->plotTogether({v0x0, v0x1, v0x2});
// g0->view();
g0->plotTogether({v0y0, v0y1, v0y2});
// g0->view();
g0->plotTogether({u0, k0, p0, c0, t0}, "points");
g0->view();

Gnuplot *g1 = new Gnuplot();
g1->plotTogether({p1x0, p1x1, p1x2});
// g1->view();
g1->plotTogether({p1y0, p1y1, p1y2});
// g1->view();
g1->plotTogether({v1x0, v1x1, v1x2});
// g1->view();
g1->plotTogether({v1y0, v1y1, v1y2});
// g1->view();
g1->plotTogether({u1, k1, p1, c1, t1}, "points");
g1->view();