// -----------------------------------------------------------------------------------------
int dimension = 3;

// -----------------------------------------------------------------------------------------
// PARTICLE PACKAGE GEOMETRY
// -----------------------------------------------------------------------------------------
Geometry *boundaryGeometry = new Geometry(0);


Point *p0 = boundaryGeometry->addPoint({-0.5, -0.5, 0.0});
Point *p1 = boundaryGeometry->addPoint({0.5, -0.5, 0.0});
Point *p2 = boundaryGeometry->addPoint({0.5, 0.5, 0.0});
Point *p3 = boundaryGeometry->addPoint({-0.5, 0.5, 0.0});
Line *l0 = boundaryGeometry->addLine({p0, p1});
Line *l1 = boundaryGeometry->addLine({p1, p2});
Line *l2 = boundaryGeometry->addLine({p2, p3});
Line *l3 = boundaryGeometry->addLine({p3, p0});

boundaryGeometry->transfiniteLine({l0}, 1);
boundaryGeometry->transfiniteLine({l1}, 1);
boundaryGeometry->transfiniteLine({l2}, 1);
boundaryGeometry->transfiniteLine({l3}, 1);

Surface *s0 = boundaryGeometry->addPlaneSurface({l0, l1, l2, l3});

boundaryGeometry->transfiniteLine({l0}, 1);
boundaryGeometry->transfiniteLine({l1}, 1);
boundaryGeometry->transfiniteLine({l2}, 1);
boundaryGeometry->transfiniteLine({l3}, 1);

// -----------------------------------------------------------------------------------------
// PARTICULATE PROBLEM
// -----------------------------------------------------------------------------------------
// Materials
Material *particleMaterial = new RigidParticle(1.e10, 0.25, 29841.5518298, 0.0, 0.0);
Material *boundaryMaterial = new ElasticSolid(SAINT_VENANT_KIRCHHOFF, 90.e6, 0.0, 7.93e-6);

ParticleDomain *particulate_problem = new ParticleDomain(boundaryGeometry, dimension);
particulate_problem->readParticlePackageData("particlePackages/single.dat", particleMaterial);

// Friction parameters of each group formed by particle-particle or particle-wall pair
FrictionParameters *parameters1 = new FrictionParameters(particleMaterial, boundaryMaterial, 0.0, 0.0, 0.0);
FrictionParameters *parameters2 = new FrictionParameters(particleMaterial, particleMaterial, 0.0, 0.0, 0.0);
particulate_problem->addFrictionParameters({parameters1, parameters2});

// Rigid walls mesh
particulate_problem->applyMaterial({s0}, boundaryMaterial);
//particulate_problem->generateMesh(T3, FRONT, "boundary", "", false, false);

// Target surfaces
//particulate_problem->addTargetSurface({s0});

// Analysis parameters
particulate_problem->setPositionErrorTolerance(1.e-9);
particulate_problem->setRotationErrorTolerance(1.e-9);
//particulate_problem->setVelocityErrorTolerance(1.e-6);
//particulate_problem->setAngularVelocityErrorTolerance(1.e-6);
particulate_problem->setMaxNonlinearIterations(100);
particulate_problem->setGravity(0.0, 0.0, -9.81);
particulate_problem->setNumberOfSteps(50000);
particulate_problem->setExportFrequency(10);
particulate_problem->setSpectralRadius(1.0);
particulate_problem->setDeltat(1.e-4);

// Problem solution
particulate_problem->solveStaggeredProblem();
// particulate_problem->solveTransientProblem();

// Plot results
Gnuplot *gp1 = new Gnuplot("plotGraph/positions1.png");
gp1->setTitle("Node 1 Positions");
gp1->setXLabel("Time");
gp1->setYLabel("Position");
gp1->setRange("x", 0, 0.5);
gp1->setRange("y", -1.5, 0.06);
gp1->setXTics(0.1);
gp1->setYTics(0.25);
gp1->addPlot("plotData/xPosNode1.dat", "Position in x", "lines", "red");
gp1->addPlot("plotData/yPosNode1.dat", "Position in y", "lines", "blue");
gp1->addPlot("plotData/zPosNode1.dat", "Position in z", "lines", "black");
gp1->plot();
gp1->view();

Gnuplot *gp2 = new Gnuplot("plotGraph/velocities1.png");
gp2->setTitle("Node 1 Velocities");
gp2->setXLabel("Time");
gp2->setYLabel("Velocity");
gp2->setRange("x", 0, 0.5);
gp2->setRange("y", -5.0, 1.0);
gp2->setXTics(0.1);
gp2->setYTics(1.0);
gp2->addPlot("plotData/xVelNode1.dat", "Velocity in x", "lines", "red");
gp2->addPlot("plotData/yVelNode1.dat", "Velocity in y", "lines", "blue");
gp2->addPlot("plotData/zVelNode1.dat", "Velocity in z", "lines", "black");
gp2->plot();
gp2->view();

Gnuplot *gp3 = new Gnuplot("plotGraph/positions2.png");
gp3->setTitle("Node 2 Positions");
gp3->setXLabel("Time");
gp3->setYLabel("Position");
gp3->setRange("x", 0, 5);
gp3->setRange("y", -1.5, 0.2);
gp3->setXTics(1);
gp3->setYTics(0.25);
gp3->addPlot("plotData/xPosNode2.dat", "Position in x", "lines", "red");
gp3->addPlot("plotData/yPosNode2.dat", "Position in y", "lines", "blue");
gp3->addPlot("plotData/zPosNode2.dat", "Position in z", "lines", "black");
gp3->plot();
gp3->view();

Gnuplot *gp4 = new Gnuplot("plotGraph/velocities2.png");
gp4->setTitle("Node 2 Velocities");
gp4->setXLabel("Time");
gp4->setYLabel("Velocity");
gp4->setRange("x", 0, 5);
gp4->setRange("y", -5.0, 1.5);
gp4->setXTics(1);
gp4->setYTics(1);
gp4->addPlot("plotData/xVelNode2.dat", "Velocity in x", "lines", "red");
gp4->addPlot("plotData/yVelNode2.dat", "Velocity in y", "lines", "blue");
gp4->addPlot("plotData/zVelNode2.dat", "Velocity in z", "lines", "black");
gp4->plot();
gp4->view();
