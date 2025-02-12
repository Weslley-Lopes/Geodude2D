// -----------------------------------------------------------------------------------------
int dimension = 2;

// -----------------------------------------------------------------------------------------
// PARTICULATE PROBLEM
// -----------------------------------------------------------------------------------------
// New discrete element problem
ParticleDomain *particulate_problem = new ParticleDomain(dimension);

// Materials
Material *particleMaterial = new RigidParticle(1.e9, 0.0, 2387.32414638, 0.0, 0.0); // ball with radius = 10 cm and mass = 10 kg
Material *surfaceMaterial = new RigidWall();

// Reading particle package and applying material
particulate_problem->readParticlePackageData("../../particlePackages/campello_4.2_2d.dat", particleMaterial);

// Rigid wall
ContactSurface *surface = new ContactSurface(X, 0.101, {-1.0, 0.0, 0.0}, surfaceMaterial);
particulate_problem->addContactSurface({surface});

// Friction parameters of each group formed by particle-particle or particle-wall pair
FrictionParameters *parameters = new FrictionParameters(particleMaterial, surfaceMaterial, 0.0, 0.0, 0.0);
particulate_problem->addFrictionParameters({parameters});

// Analysis parameters
particulate_problem->setErrorTolerance(1.e-9);
particulate_problem->setMaxNonlinearIterations(100);
particulate_problem->setNumberOfSteps(100);
particulate_problem->setExportFrequency(1);
particulate_problem->setDeltat(1.e-4);
// particulate_problem->setSpectralRadius(0.5);
// particulate_problem->setGravity(0.0, 0.0, 0.0);
particulate_problem->addInitialCondition({1.0, 0.0, 0.0}, VELOCITY);
// particulate_problem->addInitialCondition({0.0, 0.0, 125.0}, ANGULAR_VELOCITY);

// Plot definition
OutputGraphic *px = new OutputGraphic(POSITION, X, 0);
OutputGraphic *py = new OutputGraphic(POSITION, Y, 0);
OutputGraphic *vx = new OutputGraphic(VELOCITY, X, 0);
OutputGraphic *vy = new OutputGraphic(VELOCITY, Y, 0);
OutputGraphic *rz = new OutputGraphic(ROTATION, Z, 0);
OutputGraphic *wz = new OutputGraphic(ANGULAR_VELOCITY, Z, 0);
OutputGraphic *wtz = new OutputGraphic(ANGULAR_ACCELERATION, Z, 0);
OutputGraphic *f = new OutputGraphic(CONTACT_FORCE, X, 0);

OutputGraphic *k = new OutputGraphic(KINETIC_ENERGY, 0);
OutputGraphic *p = new OutputGraphic(EXTERNAL_ENERGY, 0);
OutputGraphic *c = new OutputGraphic(CONTACT_ENERGY, 0);
OutputGraphic *t = new OutputGraphic(TOTAL_ENERGY, 0);

particulate_problem->addGraphic({px, py, vx, vy, rz, wz, wtz, f, k, p, c, t});

// Problem solution
// particulate_problem->solveStaggeredProblem();
particulate_problem->solveTransientProblem();

// Plot results
// Gnuplot *g0 = new Gnuplot();
// g0->plotSeparate(particulate_problem->getOutputGraphics());

Gnuplot *g1 = new Gnuplot();
// g1->plotTogether({px0, px1, px2});
g1->plotSeparate({px, py, vx, vy, rz, wz, wtz, f});
g1->view();
g1->plotTogether({k, p, c, t});
g1->view();

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