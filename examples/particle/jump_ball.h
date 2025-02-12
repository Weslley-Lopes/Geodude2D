// -----------------------------------------------------------------------------------------
int dimension = 2;

// -----------------------------------------------------------------------------------------
// PARTICULATE PROBLEM
// -----------------------------------------------------------------------------------------
// New discrete element problem
ParticleDomain *discrete_problem = new ParticleDomain(dimension);

// Materials
Material *particleMaterial = new RigidParticle(1.e9, 0.0, 2387.32414638, 0.0, 0.0); // ball with radius = 10 cm and mass = 10 kg
Material *surfaceMaterial = new RigidWall();

// Reading particle package and applying material
discrete_problem->readParticlePackageData("../../particlePackages/campello_4.2_2d.dat", particleMaterial);

// Rigid wall
ContactSurface *surface = new ContactSurface(Y, -0.1, {0.0, 1.0, 0.0}, surfaceMaterial);
discrete_problem->addContactSurface({surface});

// Friction parameters of each group formed by particle-particle or particle-wall pair
FrictionParameters *parameters = new FrictionParameters(particleMaterial, surfaceMaterial, 0.0, 0.0, 0.0);
discrete_problem->addFrictionParameters({parameters});

// Analysis parameters
discrete_problem->setErrorTolerance(1.e-9);
discrete_problem->setMaxNonlinearIterations(20);
discrete_problem->setNumberOfSteps(1000);
discrete_problem->setExportFrequency(1);
discrete_problem->setDeltat(1.e-4);
discrete_problem->setGravity(0.0, -9.81, 0.0);

// Plot definition
OutputGraphic *px = new OutputGraphic(POSITION, X, 0);
OutputGraphic *py = new OutputGraphic(POSITION, Y, 0);
OutputGraphic *vx = new OutputGraphic(VELOCITY, X, 0);
OutputGraphic *vy = new OutputGraphic(VELOCITY, Y, 0);
OutputGraphic *rz = new OutputGraphic(ROTATION, Z, 0);
OutputGraphic *wz = new OutputGraphic(ANGULAR_VELOCITY, Z, 0);
OutputGraphic *wtz = new OutputGraphic(ANGULAR_ACCELERATION, Z, 0);
OutputGraphic *k = new OutputGraphic(KINETIC_ENERGY, 0);
OutputGraphic *p = new OutputGraphic(EXTERNAL_ENERGY, 0);
OutputGraphic *c = new OutputGraphic(CONTACT_ENERGY, 0);
OutputGraphic *t = new OutputGraphic(TOTAL_ENERGY, 0);
OutputGraphic *f = new OutputGraphic(CONTACT_FORCE, Y, 0);
// OutputGraphic *vy2 = new OutputGraphic(VELOCITY, Y, 0, 2);
discrete_problem->addGraphic({px, py, vx, vy, k, p, c, t, f});

// Problem solution
// discrete_problem->solveStaggeredProblem();
discrete_problem->solveTransientProblem();

Gnuplot *g1 = new Gnuplot();
g1->plotSeparate({px, py, vx, vy, rz, wz, wtz, f});
g1->view();
g1->plotTogether({k, p, c, t});
g1->view();