#pragma once
#include "Node.h"
#include "BaseLineElement.h"
#include "BaseSurfaceElement.h"
#include "mesh_interface/Enums.h"


class NeumannBoundaryCondition
{
    protected:
        int index_;
        int ndofs_;
        double forces_[3];
        ForceType type_;

    public:

        NeumannBoundaryCondition(const int index, const int ndofs, const double valueX, const double valueY, const double valueZ, ForceType type = CONSERVATIVE);

        virtual ~NeumannBoundaryCondition() = 0;

        int getNumberOfDOFs() const;

        double getForce(const int& dof) const;

        ForceType getType() const;

        virtual void getNodalForce(const int& dimension, std::vector<DegreeOfFreedom*>& dofs, double*& values) const = 0;

        virtual void getPotentialEnergy(const int &dimension, const double &currentTime, double &potentialEnergy) const = 0;
};

class PointLoad : public NeumannBoundaryCondition
{
    private:
        Node* node_;
    
    public:

        PointLoad(const int index, const int ndofs, Node* const node, const double valueX, const double valueY, const double valueZ, ForceType type = CONSERVATIVE);

        ~PointLoad() override;

        void getNodalForce(const int& dimension, std::vector<DegreeOfFreedom*>& dofs, double*& values) const override;

        void getPotentialEnergy(const int &dimension, const double &currentTime, double &potentialEnergy) const override;
};

class LineLoad : public NeumannBoundaryCondition
{
    private:
        BaseLineElement* element_;

    public:

        LineLoad(const int index, const int ndofs, BaseLineElement* const element, const double valueX, const double valueY, const double valueZ);

        ~LineLoad() override;

        void getNodalForce(const int& dimension, std::vector<DegreeOfFreedom*>& dofs, double*& values) const override;

        void getPotentialEnergy(const int &dimension, const double &currentTime, double &potentialEnergy) const override;
};

class SurfaceLoad : public NeumannBoundaryCondition
{
    private:
        BaseSurfaceElement* element_;

    public:

        SurfaceLoad(const int index, const int ndofs, BaseSurfaceElement* const element, const double valueX, const double valueY, const double valueZ);

        ~SurfaceLoad() override;
        
        void getNodalForce(const int& dimension, std::vector<DegreeOfFreedom*>& dofs, double*& values) const override;

        void getPotentialEnergy(const int &dimension, const double &currentTime, double &potentialEnergy) const override;
};