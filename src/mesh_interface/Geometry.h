#pragma once

#include "PlaneSurface.h"
#include "GeometricDirichlet.h"
#include "GeometricNeumann.h"
#include "GeometricInitial.h"
#include <unordered_map>
#include <functional>

class Geometry
{
public:
	Geometry();

	Geometry(const int &index);

	~Geometry();

	int getIndex();

	int getNumberOfPoints();

	int getNumberOfLines();

	int getNumberOfLineLoops();

	int getNumberOfSurfaces();

	int getNumberOfSurfaceLoops();

	int getNumberOfVolumes();

	// int getNumberOfBoundaryConditions(const std::string& type);
	int getNumberOfBoundaryConditions();

	Point *getPoint(const std::string &name);

	Line *getLine(const std::string &name);

	LineLoop *getLineLoop(const std::string &name);

	Surface *getSurface(const std::string &name);

	const std::unordered_map<std::string, Line *> &getLines();

	const std::unordered_map<std::string, Surface *> &getSurfaces();

	const std::vector<GeometricDirichlet *> &getDirichletBoundaryConditions();

	const std::vector<GeometricNeumann *> &getNeumannBoundaryConditions();

	const std::vector<GeometricInitial *> &getInitialConditions();

	std::string getGmshCode();

	Point *addPoint(std::vector<double> coordinates, const double &lcar = 1.0, const bool &discretization = true);

	Line *addLine(std::vector<Point *> points, const bool &discretization = true);

	Circle *addCircle(std::vector<Point *> points, const bool &discretization = true);

	Spline *addSpline(std::vector<Point *> points, std::function<double(double)> function, const int &ndiv, const bool &discretization = true);

	LineLoop *addLineLoop(std::vector<Line *> lines);

	Surface *addSurface(LineLoop *lineLoop);

	Surface *addSurface(std::vector<Line *> lines);

	PlaneSurface *addPlaneSurface(LineLoop *lineLoop);

	PlaneSurface *addPlaneSurface(std::vector<Line *> lines);

	void appendGmshCode(std::string text);

	void transfiniteLine(std::vector<Line *> lines, const int &divisions, const double &progression = 1);

	void transfiniteSurface(std::vector<Surface *> surfaces, std::string oientation = "Left", std::vector<Point *> points = std::vector<Point *>());

	void addDirichletBoundaryCondition(const std::vector<Point *> &points, const Variable variable, const ConstrainedDOF dof, const double value);

	void addDirichletBoundaryCondition(const std::vector<Line *> &lines, const Variable variable, const ConstrainedDOF dof, const double value);

	void addDirichletBoundaryCondition(const std::vector<Surface *> &surfaces, const Variable variable, const ConstrainedDOF dof, const double value);

	void addNeumannBoundaryCondition(const std::vector<Point *> &points, const double valueX, const double valueY, const double valueZ, ForceType type = CONSERTATIVE);

	void addNeumannBoundaryCondition(const std::vector<Line *> &lines, const double valueX, const double valueY, const double valueZ);

	void addNeumannBoundaryCondition(const std::vector<Surface *> &surfaces, const double valueX, const double valueY, const double valueZ);

	void addInitialCondition(const std::vector<Point *> &points, const Variable variable, const ConstrainedDOF dof, const double value);

	void addInitialCondition(const std::vector<Line *> &lines, const Variable variable, const ConstrainedDOF dof, const double value);

	void addInitialCondition(const std::vector<Surface *> &surfaces, const Variable variable, const ConstrainedDOF dof, const double value);

private:
	int index_;
	std::unordered_map<std::string, Point *> points_;
	std::unordered_map<std::string, Line *> lines_;
	std::unordered_map<std::string, LineLoop *> lineLoops_;
	std::unordered_map<std::string, Surface *> surfaces_;
	std::vector<GeometricDirichlet *> dirichlet_;
	std::vector<GeometricNeumann *> neumann_;
	std::vector<GeometricInitial *> initial_;
	std::string gmshCode_;

public:
	friend class FluidDomain;
	friend class SolidDomain;
	friend class ParticleDomain;
};