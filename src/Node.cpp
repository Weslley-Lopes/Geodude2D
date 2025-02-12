#include "Node.h"
#include <cmath>

Node::Node(const unsigned int &index,
           const std::vector<DegreeOfFreedom *> &degreesOfFreedom) : index_(index),
                                                                     permutedIndex_(index),
                                                                     rank_(0),
                                                                     cloudArea_(0.0),
                                                                     isBoundary_(false),
                                                                     isFreeSurface_(false),
                                                                     isConstrained_(false),
                                                                     isBlocked_(false),
                                                                     isIsolated_(false),
                                                                     isInterface_(false),
                                                                     isInContact_(false),
                                                                     isProjectile_(false),
                                                                     isTarget_(false),
                                                                     wasFreeSurface_(false),
                                                                     wasIsolated_(false),
                                                                     isToRemove_(false),
                                                                     isNewEntity_(true),
                                                                     material_(nullptr),
                                                                     cauchyStress_(new double[degreesOfFreedom.size() * (degreesOfFreedom.size() + 1) / 2]),
                                                                     interfaceNode_(nullptr),
                                                                     degreesOfFreedom_(degreesOfFreedom),
                                                                     contactForce_(new double[2]) // 2D problems
{
    const unsigned int dimension = degreesOfFreedom.size();
    for (unsigned int i = 0; i < dimension * (dimension + 1) / 2; i++)
        cauchyStress_[i] = 0.0;
    for (unsigned int i = 0; i < 2; i++)
        contactForce_[i] = 0.0;
    degreesOfFreedom_.reserve(dimension + 1);
    neighborNodes_.reserve(10);
    neighborElements_.reserve(10);
}

Node::~Node()
{
    for (DegreeOfFreedom *dof : degreesOfFreedom_)
        delete dof;

    delete[] cauchyStress_;
    delete[] contactForce_;
}

bool Node::operator==(const Node &node) const
{
    return index_ == node.index_;
}

bool Node::operator<(const Node &node) const
{
    return index_ < node.index_;
}

void Node::setIndex(const unsigned int &index)
{
    index_ = index;
}

void Node::setPermutedIndex(const unsigned int &index)
{
    permutedIndex_ = index;
}

void Node::setRank(const unsigned int &rank)
{
    rank_ = rank;
}

void Node::setMaterial(Material *material)
{
    material_ = material;
}

void Node::setDegreesOfFreedom(const std::vector<DegreeOfFreedom *> &degreesOfFreedom)
{
    degreesOfFreedom_ = degreesOfFreedom;
}

void Node::setCauchyStress(const unsigned int &size,
                           double *&cauchyStress)
{
    for (unsigned int i = 0; i < size; i++)
        cauchyStress_[i] = cauchyStress[i];
}

void Node::setContactForce1(const unsigned int &size,
                           double *&contactForce)
{
    for (unsigned int i = 0; i < size; i++)
        contactForce_[i] = contactForce[i];
}

void Node::clearCauchyStress(const unsigned int &size)
{
    for (unsigned int i = 0; i < size; i++)
        cauchyStress_[i] = 0.0;
}

void Node::clearContactForce(const unsigned int &size)
{
    for (unsigned int i = 0; i < size; i++)
        contactForce_[i] = 0.0;
}

void Node::incrementCauchyStress(const unsigned int &size,
                                 double *&cauchyStress)
{
    for (unsigned int i = 0; i < size; i++)
        cauchyStress_[i] += cauchyStress[i];
}

void Node::setCloudArea(const double &cloudArea)
{
    cloudArea_ = cloudArea;
}

void Node::setBoundary(const bool &isBoundary)
{
    isBoundary_ = isBoundary;
}

void Node::setFreeSurface(const bool &isFreeSurface)
{
    isFreeSurface_ = isFreeSurface;
}

void Node::setConstrain(const bool &isConstrained)
{
    isConstrained_ = isConstrained;
}

void Node::setBlocked(const bool &isBlocked)
{
    isBlocked_ = isBlocked;
}

void Node::setIsolated(const bool &isIsolated)
{
    isIsolated_ = isIsolated;
}

void Node::setInterface(const bool &isInterface)
{
    isInterface_ = isInterface;
}

void Node::setInContact(const bool &isInContact)
{
    isInContact_ = isInContact;
}

void Node::setProjectile(const bool &isProjectile)
{
    isProjectile_ = isProjectile;
}

void Node::setTarget(const bool &isTarget)
{
    isTarget_ = isTarget;
}

void Node::setInterfaceNode(Node *interfaceNode)
{
    interfaceNode_ = interfaceNode;
}

void Node::setPreviouslyFreeSurface(const bool &wasFreeSurface)
{
    wasFreeSurface_ = wasFreeSurface;
}

void Node::setPreviouslyIsolated(const bool &wasIsolated)
{
    wasIsolated_ = wasIsolated;
}

void Node::setToRemove(const bool &isToRemove)
{
    isToRemove_ = isToRemove;
}

void Node::setNewEntity(const bool &isNewEntity)
{
    isNewEntity_ = isNewEntity;
}

unsigned int Node::getIndex() const
{
    return index_;
}

unsigned int Node::getPermutedIndex() const
{
    return permutedIndex_;
}

unsigned int Node::getRank() const
{
    return rank_;
}

Material *Node::getMaterial() const
{
    return material_;
}

const std::vector<DegreeOfFreedom *> &Node::getDegreesOfFreedom() const
{
    return degreesOfFreedom_;
}

double *Node::getCauchyStress() const
{
    return cauchyStress_;
}

double *Node::getContactForce1() const
{
    return contactForce_;
}

unsigned int Node::getNumberOfDegreesOfFreedom() const
{
    return degreesOfFreedom_.size();
}

DegreeOfFreedom *Node::getDegreeOfFreedom(const unsigned int &index) const
{
    return degreesOfFreedom_[index];
}

double Node::getCloudArea() const
{
    return cloudArea_;
}

bool Node::isBoundary() const
{
    return isBoundary_;
}

bool Node::isFreeSurface() const
{
    return isFreeSurface_;
}

bool Node::isConstrained() const
{
    return isConstrained_;
}

bool Node::isBlocked() const
{
    return isBlocked_;
}

bool Node::isIsolated() const
{
    return isIsolated_;
}

bool Node::isInterface() const
{
    return isInterface_;
}

bool Node::isInContact() const
{
    return isInContact_;
}

bool Node::isProjectile() const
{
    return isProjectile_;
}

bool Node::isTarget() const
{
    return isTarget_;
}

bool Node::wasIsolated() const
{
    return wasIsolated_;
}

bool Node::wasFreeSurface() const
{
    return wasFreeSurface_;
}

bool Node::isToRemove() const
{
    return isToRemove_;
}

bool Node::isNewEntity() const
{
    return isNewEntity_;
}

const std::vector<Node *> &Node::getNeighborNodes() const
{
    return neighborNodes_;
}

const std::vector<Element *> &Node::getNeighborElements() const
{
    return neighborElements_;
}

const std::vector<Particle *> &Node::getNeighborParticles() const
{
    return neighborParticles_;
}

Node *Node::getInterfaceNode() const
{
    return interfaceNode_;
}

void Node::addDegreeOfFreedom(DegreeOfFreedom *dof)
{
    degreesOfFreedom_.emplace_back(dof);
}

void Node::addNeighborNode(Node *node)
{
    neighborNodes_.push_back(node);
}

void Node::addNeighborElement(Element *el)
{
    neighborElements_.push_back(el);
}

void Node::addNeighborParticle(Particle *particle)
{
    neighborParticles_.push_back(particle);
}

void Node::removeDegreeOfFreedom(const unsigned int &index)
{
    degreesOfFreedom_.erase(degreesOfFreedom_.begin() + index);
}

void Node::clearNeighborNodes()
{
    neighborNodes_.clear();
    neighborNodes_.shrink_to_fit();
    neighborNodes_.reserve(10);
}

void Node::clearNeighborElements()
{
    neighborElements_.clear();
    neighborElements_.shrink_to_fit();
    neighborElements_.reserve(10);
}

double Node::distanceToNode(const Node &node,
                            const unsigned int &dimension)
{
    double distance = 0.0;
    for (unsigned int i = 0; i < dimension; i++)
    {
        distance += (degreesOfFreedom_[i]->getCurrentValue() - node.getDegreeOfFreedom(i)->getCurrentValue()) *
                    (degreesOfFreedom_[i]->getCurrentValue() - node.getDegreeOfFreedom(i)->getCurrentValue());
    }
    return sqrt(distance);
}

double Node::squareDistanceToNode(const Node &node,
                                  const unsigned int &dimension)
{
    double distance = 0.0;
    for (unsigned int i = 0; i < dimension; i++)
    {
        distance += (degreesOfFreedom_[i]->getCurrentValue() - node.getDegreeOfFreedom(i)->getCurrentValue()) *
                    (degreesOfFreedom_[i]->getCurrentValue() - node.getDegreeOfFreedom(i)->getCurrentValue());
    }
    return distance;
}

void Node::searchNodesInRadius(const double &radius, const int &dimension, int &numberOfFoundNodes, std::vector<Node *> &foundNodes, std::vector<double> &distances)
{
    foundNodes.reserve(neighborNodes_.size());
    distances.reserve(neighborNodes_.size());
    numberOfFoundNodes = 1;

    auto begin = neighborNodes_.begin() + 1;
    auto end = neighborNodes_.end();
    for (auto &nnode = begin; nnode != end; nnode++)
    {
        double distance = distanceToNode(*(*nnode), dimension);
        if (distance < radius)
        {
            numberOfFoundNodes++;
            foundNodes.push_back(*nnode);
            distances.push_back(distance);
        }
    }
}