// -----------------------------------------------------------------------------------------
int dimension = 2;

// -----------------------------------------------------------------------------------------
// PARTICULATE PROBLEM
// -----------------------------------------------------------------------------------------
// Materials
Material *particleMaterial = new RigidParticle(1.e9, 0.25, 29841.5518298, 0.0, 0.0); // ball with radius = 20 cm and mass = 1000 kg
Material *surfaceMaterial = new RigidWall();

ParticleDomain *particulate_problem = new ParticleDomain(dimension);
particulate_problem->readParticlePackageData("../../particlePackages/2d_single_particle.dat", particleMaterial);

// Friction parameters of each group formed by particle-particle or particle-wall pair
FrictionParameters *parameters = new FrictionParameters(particleMaterial, surfaceMaterial, 0.0, 0.0, 0.0);
particulate_problem->addFrictionParameters({parameters});

// Analysis parameters
particulate_problem->setErrorTolerance(1.e-9);
particulate_problem->setMaxNonlinearIterations(10);
particulate_problem->setNumberOfSteps(1000);
particulate_problem->setExportFrequency(1);
particulate_problem->setDeltat(1.e-4);
// particulate_problem->setSpectralRadius(1.0);
// particulate_problem->setGravity(0.0, -9.81, 0.0);
// particulate_problem->addInitialCondition({1.0, 0.0, 0.0}, VELOCITY);
particulate_problem->addInitialCondition({0.0, 0.0, 125.0}, ANGULAR_VELOCITY);

// Plot definition
OutputGraphic *px = new OutputGraphic(POSITION, X, 0);
OutputGraphic *py = new OutputGraphic(POSITION, Y, 0);
OutputGraphic *vx = new OutputGraphic(VELOCITY, X, 0);
OutputGraphic *vy = new OutputGraphic(VELOCITY, Y, 0);
OutputGraphic *rz = new OutputGraphic(ROTATION, Z, 0);
OutputGraphic *wz = new OutputGraphic(ANGULAR_VELOCITY, Z, 0);
OutputGraphic *wtz = new OutputGraphic(ANGULAR_ACCELERATION, Z, 0);

// OutputGraphic *u = new OutputGraphic(STRAIN_ENERGY, 0);
// OutputGraphic *k = new OutputGraphic(KINETIC_ENERGY, 0);
// OutputGraphic *p = new OutputGraphic(EXTERNAL_ENERGY, 0);
// OutputGraphic *t = new OutputGraphic(TOTAL_ENERGY, 0);

particulate_problem->addGraphic({px, py, vx, vy, rz, wz, wtz});

// Problem solution
// particulate_problem->solveStaggeredProblem();
particulate_problem->solveTransientProblem();

// Plot results
// Gnuplot *g0 = new Gnuplot();
// g0->plotSeparate(particulate_problem->getOutputGraphics());

Gnuplot *g1 = new Gnuplot();
// g1->plotTogether({px0, px1, px2});
g1->plotSeparate({px, py, vx, vy, rz, wz, wtz});
// g1->view();
// g1->plotSeparate({vx, vy});
// g1->view();
// g1->plotSeparate({tz});
// // g1->view();
// g1->plotSeparate({u, k, p, t}, "points");

// Example of a single plot or a set of predefined plots
/*Gnuplot *pos1 = new Gnuplot("plotGraph/velocities.png");
plot->setTitle("Velocities");
plot->setXLabel("Time");
plot->setYLabel("Velocity");
//plot->setRange("x", graph2->getXMin(), graph2->getXMax());
//plot->setRange("y", graph2->getYMin(), graph2->getYMax());
//plot->setXTics((graph2->getXMax()-graph2->getXMin())/5.);
//plot->setYTics((graph2->getYMax()-graph2->getYMin())/5.);
plot->addPlot(g4, "Node 0, Y direction", "lines", "red");
plot->addPlot(g5, "Node 1, Y direction", "lines", "blue");
plot->addPlot(g6, "Node 2, Y direction", "lines", "black");
plot->addPlot(g7, "Node 2, x direction", "lines", "gray");
plot->plot();
//plot->view();*/