#pragma once

#include <vector>
#include <algorithm>
#include "globals.h"
#include "ray.h"
#include "aabb.h"
#include "intersection.h"
#include "mempool.h"

// kd-Tree
template<class T>
class KDTree
{
protected:
	typedef std::vector<int> idxarray;

	// inner classes
	class object
	{
	public:
		T* ptr;
		unsigned int lastId;
	public:
		object() : ptr(0), lastId(0) { }
		object(T* p) : ptr(p), lastId(0) { }
	};

	// root node of tree
	class node
	{
	public:
		node() { }
		~node() { }

		// constructor for new nodes
		node(object* objArray, const idxarray& objIdx, MemPool<object*>& memPool, AABB bb)
		{
			int n = (int)objIdx.size();
			if (n > 1)
			{
				objs = memPool.allocate(n);
				for (int i = 0; i < n; i++) objs[i] = &objArray[objIdx[i]];
			}
			else
			{
				obj = &objArray[objIdx[0]];
			}

			boundingBox = bb;

			num = (LEAF | n);
		}

		node(int axis, float t)
		{
			flags = (axis << AXISBITS);	// Splittingebene 0..2
			split = t;					// Position der Splittingebene
		}

		bool isLeaf() const { return (flags & LEAF) == LEAF; }
		int getAxis() const { return (flags >> AXISBITS); }		// Achse
		float getSplit() const { return split; }				// Position der Splitting Ebene
		void setFarChild(int i) { farChild = (flags & LEAF) | i; }
		int getFarChild() const { return (farChild & MASK); }

		int getNum() const { return (num & MASK); }				// Anzahl der Objekte im Knoten
		object& getObj(int i) const { return *objs[i]; }

		object& getObj() const { return *obj; }

	protected:
		static const int AXISBITS = 30;					///< Number of bit position to shift in order to access AXISBITS.
		static const unsigned int LEAF = 0xC0000000;	///< Masks out the flag bits (axis=0..2 or leaf=3).
		static const unsigned int MASK = 0x3FFFFFFF;	///< Masks out the number part (number of objs or idx to far child).

		union	// first dword
		{
			unsigned int flags;
			unsigned int num;
			unsigned int farChild;
		};
		union	// second dword
		{
			float split;
			object** objs;
			object* obj;
		};

	public:
		AABB boundingBox;				// Bounding Box vom Knoten
	};


	// Bounding Box Ecke
	class edge
	{
	public:
		float t;							// Position
		int idx;							// Index vom Objekt
		enum type_t { START, END } type;
	public:
		edge() { }
		edge(float _t, int _idx, type_t _type) : t(_t), idx(_idx), type(_type) { }
		bool operator<(const edge& e) const { return t == e.t ? (int)type < (int)e.type : t < e.t; }
	};


	class nodeTodo
	{
	public:
		int idx;			///< Index of node.
		float tmin;			///< Ray min time.
		float tmax;			///< Ray max time.
	public:
		nodeTodo() { }
		nodeTodo(int _idx, float _tmin, float _tmax) : idx(_idx), tmin(_tmin), tmax(_tmax) { }
	};



public:
	KDTree();
	virtual ~KDTree();

	// create new kdTree based on given data
	void buildTree(const std::vector<T*>& objs, int maxDepth = 0, int maxObjs = 1);

	// Intersect Methoden
	bool intersect(const Ray& ray);
	bool intersect(const Ray& ray, Intersection& is);

protected:
	void buildTree(const idxarray& objs, const AABB& bb, int depth, int badRefines);
	void classifyObjs(const idxarray& objs, idxarray& objsBelow, idxarray& objsAbove, int axis, int offset);
	float bestSplit(int numObjs, int axis, const AABB& bb, int& offset) const;
	void setupEdges(const idxarray& objs, int axis);
	void computeBBox(AABB& world, AABB* boxes) const;

protected:
	unsigned int mCurrentRayId;			// Ray ID

	int mNumObjs;						// Anzahl der Objekte im Tree
	object* mObjs;						// Objekte
	
	int mMaxDepth;						//Tiefe
	int mMaxObjs;						//max. Anzahl von Objekten im Blatt
public:
	std::vector<node> mNodes;			//Knoten

	AABB mWorldBox;						//Bounding Box vom Tree
protected:
	AABB* mBoxes;						//Bounding Box Array

	edge* mEdges[3];					//Edges Array
	idxarray* mObjIdx[2];
	std::vector<nodeTodo> mNodesTodo;

	MemPool<object*> mMemPool;
};


static const float EMPTY_BONUS = 0.1f;
static const float INTERSECTION_COST = 80.0f;
static const float TRAVERSAL_COST = 1.0f;

template<class T>
KDTree<T>::KDTree()
	: mObjs(0)
	, mNumObjs(0)
	, mCurrentRayId(0)
{
}


template<class T>
KDTree<T>::~KDTree()
{
	if (mObjs) delete[] mObjs;
}

template<class T>
bool KDTree<T>::intersect(const Ray& ray)
{
	// Intersection test the ray against the tree bounding box.
	float tmin, tmax;
	if (!mWorldBox.intersect(ray, tmin, tmax)) return false;

	// Prepare for tree traversal.
	unsigned int rayId = ++mCurrentRayId;	// get new ray id
	Vector invDir = 1.0f / ray.dir;			// compute reciprocal ray direction
	int current = 0;						// current node
	mNodesTodo.resize(0);					// clear node todo list, this shouldn't clear previously reserved mem
	bool hit = false;

	// Traverse the tree until the ray exists or hits something.
	while (true)
	{
		if (ray.maxT < tmin) break;
		node& currentNode = mNodes[current];

		if (!currentNode.isLeaf())		// process interior node
		{
			// Compute splitting plane intersection time.
			int axis = currentNode.getAxis();
			float split = currentNode.getSplit();
			float orig = ray.orig(axis);
			float t = (split - orig) * invDir(axis);

			// Choose traversal order based on ray origin.
			int first, second;
			if (orig <= split)
			{
				first = current + 1;
				second = currentNode.getFarChild();
			}
			else
			{
				first = currentNode.getFarChild();
				second = current + 1;
			}

			// Check if we need to traverse both children of just one side.
			if (t > tmax || t < 0.0f)	// first only
				current = first;
			else if (t < tmin)			// second only
				current = second;
			else					// both, put second in todo list
			{
				mNodesTodo.push_back(nodeTodo(second, t, tmax));
				current = first;
				tmax = t;
			}
		}
		else		// process leaf node
		{
			// Process leaf node by checking for intersections with objects in leaf.
			int numObjs = currentNode.getNum();
			if (numObjs == 1)
			{
				// Just one object in node
				object& obj = currentNode.getObj();
				if (obj.lastId != rayId)		// check mailbox id so we are not testing the same object twice
				{
					obj.lastId = rayId;
					if (obj.ptr->intersect(ray))
					{
						hit = true;
						break;				// return true on first intersection
					}
				}
			}
			else
			{
				// More than one object, loop over objects
				for (int i = 0; i < numObjs; i++)
				{
					object& obj = currentNode.getObj(i);
					if (obj.lastId != rayId)		// check mailbox id so we are not testing the same object twice
					{
						obj.lastId = rayId;
						if (obj.ptr->intersect(ray))
						{
							hit = true;
							break;				// return true on first intersection
						}
					}
				}
			}

			// Check todo list for next node to process. If empty, the ray has passed
			// through the tree without hitting anything.
			if (!mNodesTodo.empty())
			{
				current = mNodesTodo.back().idx;
				tmin = mNodesTodo.back().tmin;
				tmax = mNodesTodo.back().tmax;
				mNodesTodo.pop_back();
			}
			else break;
		}
	}

	return hit;
}


template<class T>
bool KDTree<T>::intersect(const Ray& ray, Intersection& is)
{
	// Intersection test the ray against the tree bounding box.
	float tmin, tmax;
	if (!mWorldBox.intersect(ray, tmin, tmax)) return false;

	// Prepare for tree traversal.
	unsigned int rayId = ++mCurrentRayId;	// get new ray id
	Vector invDir = 1.0f / ray.dir;			// compute reciprocal ray direction
	int current = 0;						// current node
	mNodesTodo.resize(0);					// clear node todo list, this shouldn't clear previously reserved mem
	bool hit = false;
	Ray localRay = ray;

	// Traverse the tree until the ray exists or the closest hit is found.
	while (true)
	{
		if (localRay.maxT < tmin) break;
		node& currentNode = mNodes[current];

		if (!currentNode.isLeaf())		// process interior node
		{
			// Compute splitting plane intersection time.
			int axis = currentNode.getAxis();
			float split = currentNode.getSplit();
			float orig = ray.orig(axis);
			float t = (split - orig) * invDir(axis);

			// Choose traversal order based on ray origin.
			int first, second;
			if (orig <= split)
			{
				first = current + 1;
				second = currentNode.getFarChild();
			}
			else
			{
				first = currentNode.getFarChild();
				second = current + 1;
			}

			// Check if we need to traverse both children of just one side.
			if (t > tmax || t < 0.0f)	// first only (ray end before plane or turned away from plane)
				current = first;
			else if (t < tmin)			// second only (ray starts after plane)
				current = second;
			else					// both, put second in todo list
			{
				mNodesTodo.push_back(nodeTodo(second, t, tmax));
				current = first;
				tmax = t;
			}
		}
		else		// process leaf node
		{
			// Process leaf node by checking for intersections with objects in leaf.
			int numObjs = currentNode.getNum();
			if (numObjs == 1)
			{
				// Just one object in node
				object& obj = currentNode.getObj();
				if (obj.lastId != rayId)		// check mailbox id so we are not testing the same object twice
				{
					obj.lastId = rayId;
					Intersection isect;
					if (obj.ptr->intersect(localRay, isect))
					{
						is = isect;
						localRay.maxT = is.mHitTime;	// shorten ray
						hit = true;
					}
				}
			}
			else
			{
				// More than one object, loop over objects
				for (int i = 0; i < numObjs; i++)
				{
					object& obj = currentNode.getObj(i);
					if (obj.lastId != rayId)		// check mailbox id so we are not testing the same object twice
					{
						obj.lastId = rayId;
						Intersection isect;
						if (obj.ptr->intersect(localRay, isect))
						{
							is = isect;
							localRay.maxT = is.mHitTime;	// shorten ray
							hit = true;
						}
					}
				}
			}

			// Check todo list for next node to process. If empty, the ray has passed
			// through the tree without hitting anything.
			if (!mNodesTodo.empty())
			{
				current = mNodesTodo.back().idx;
				tmin = mNodesTodo.back().tmin;
				tmax = mNodesTodo.back().tmax;
				mNodesTodo.pop_back();
			}
			else break;		// no more nodes in the todo list

			tmax = localRay.maxT < tmax ? localRay.maxT : tmax;	// decrease tmax to closest hit so far
		}
	}

	return hit;
}


/** Builds a tree containing the given objects.
*	The maxDepth attribute specifies the maximum depth of the tree,
*	or if it is set to 0 the depth is automatically deducted from the
*	number of objects using the formula on p204 in PBRT (Pharr&Humphreys).
*/
template<class T>
void KDTree<T>::buildTree(const std::vector<T*>& objs, int maxDepth, int maxObjs)
{
	mNumObjs = (int)objs.size();
	if (mObjs) delete[] mObjs;
	mObjs = new object[mNumObjs];
	for (int i = 0; i < mNumObjs; i++) mObjs[i].ptr = objs[i];

	//Maximale Tiefe setzen
	mMaxObjs = maxObjs;
	mMaxDepth = maxDepth > 0 ? maxDepth : (8 + (int)ROUND(1.3f*std::log((float)mNumObjs)));

	mNodes.clear();
	mNodes.reserve(mNumObjs);	//Speicher für Nodes anlegen
	mMemPool.init(std::max(mNumObjs / 10, 1000));

	//Alle Bounding Boxen zusammenfügen zur KDTree Bounding Box
	mBoxes = new AABB[mNumObjs];
	computeBBox(mWorldBox, mBoxes);

	for (int i = 0; i < 3; i++) mEdges[i] = new edge[2 * mNumObjs];
	for (int i = 0; i < 2; i++) mObjIdx[i] = new idxarray[mMaxDepth];

	// Recursively build tree
	idxarray objIdx;
	objIdx.resize(mNumObjs);
	for (int i = 0; i < mNumObjs; i++) objIdx[i] = i;	// initial object indices are all objs from 0...numObjs-1

	buildTree(objIdx, mWorldBox, 0, 0);

	// Delete temp memory
	delete[] mBoxes;
	for (int i = 0; i < 3; i++) delete[] mEdges[i];
	for (int i = 0; i < 2; i++) delete[] mObjIdx[i];

	// Initialize traversal data.
	mCurrentRayId = 0;
	mNodesTodo.reserve(mMaxDepth);
}


/** Recursive function building the tree.
*	The depth parameter starts at maxDepth and is decreased until it reaches 0,
*	where the is terminated.
*/
template<class T>
void KDTree<T>::buildTree(const idxarray& objs, const AABB& bb, int depth, int badRefines)
{
	int numObjs = (int)objs.size();

	if (numObjs < 1)
		return;

	// Check if termination criteria are fulfilled (small enough number of objects or max depth),
	// if so setup leaf node and return.
	if (numObjs <= mMaxObjs || depth == mMaxDepth)
	{
		//		mNodes.push_back(node(mObjs,objs));
		mNodes.push_back(node(mObjs, objs, mMemPool, bb));
		return;
	}

	float oldCost = INTERSECTION_COST * (float)numObjs;		// cost of not splitting the node
	float bestCost = INF;
	int bestAxis = -1, bestOffset = -1, offset;

	// Try all three axes, but start with the one with largest extent.
	// As soon as a valid splitting axis is found, we break the loop.
	int axis = bb.getLargestAxis();
	for (int i = 0; i < 3; i++)
	{
		setupEdges(objs, axis);
		float cost = bestSplit(numObjs, axis, bb, offset);
		if (cost < bestCost)
		{
			bestCost = cost;
			bestAxis = axis;
			bestOffset = offset;
			break;
		}
		axis = (axis + 1) % 3;	// try next axis
	}
	if (bestCost > oldCost) badRefines++;	// track number of bad refines along this path down the tree	

	// Decide if it's better to create a leaf instead of splitting.
	if (bestAxis == -1 || badRefines == 3 || (bestCost > 4.0f*oldCost && numObjs < 16))
	{
		//		mNodes.push_back(node(mObjs,objs));
		mNodes.push_back(node(mObjs, objs, mMemPool, bb));
		return;
	}

	// Split the node at the best splitting plane.
	idxarray& objsBelow = mObjIdx[0][depth];
	idxarray& objsAbove = mObjIdx[1][depth];
	classifyObjs(objs, objsBelow, objsAbove, bestAxis, bestOffset);

	// Setup interior node and bounding boxes for the two children.
	float t = mEdges[bestAxis][bestOffset].t;	// location of splitting plane
	int currentNode = (int)mNodes.size();
	mNodes.push_back(node(bestAxis, t));

	AABB bbBelow = bb, bbAbove = bb;
	bbBelow.mMax(bestAxis) = bbAbove.mMin(bestAxis) = t;

	// Recursively build the two children nodes.
	buildTree(objsBelow, bbBelow, depth + 1, badRefines);
	mNodes[currentNode].setFarChild((int)mNodes.size());	// set index of the far child to the next free position in the nodes array.
	buildTree(objsAbove, bbAbove, depth + 1, badRefines);
}


/** Classify an array of objects with respect to the given splitting plane,
*	and put them in the bins for below or above, or in both if they intersect
*	the splitting plane. The edge array is assumed to be setup.
*/
template<class T>
void KDTree<T>::classifyObjs(const idxarray& objs, idxarray& objsBelow, idxarray& objsAbove, int axis, int offset)
{
	// First, we reset the output arrays but reserve memory for the maximum
	// number of objects that can be placed in each (that is all input objects).
	int numObjs = (int)objs.size();
	objsBelow.resize(0);		// should not clear previously allocated memory
	objsBelow.reserve(numObjs);
	objsAbove.resize(0);		// should not clear previously allocated memory
	objsAbove.reserve(numObjs);
	edge* e = mEdges[axis];

	// We loop over the edges on each side of the edge that is used as splitting plane.
	// By using the start/end flag, we can efficiently classify each object.
	for (int i = 0; i < offset; i++)
		if (e[i].type == edge::START) objsBelow.push_back(e[i].idx);
	for (int i = offset + 1; i < 2 * numObjs; i++)
		if (e[i].type == edge::END) objsAbove.push_back(e[i].idx);
}


/** Computes the location of the best splitting plane based on the
*	edges of the objects along the given axis. The cost for splitting
*	at the
*/
template<class T>
float KDTree<T>::bestSplit(int numObjs, int axis, const AABB& bb, int& offset) const
{
	int nBelow = 0;						// number of objs below plane
	int nAbove = numObjs;				// number of objs above plane

	int axis1 = (axis + 1) % 3;				// the 1st other axis
	int axis2 = (axis + 2) % 3;				// the 2nd other axis
	float d1 = bb.mMax(axis1) - bb.mMin(axis1);	// length of aabb along axis1
	float d2 = bb.mMax(axis2) - bb.mMin(axis2);	// length of aabb along axis2
	float A = d1 * d2;					// area of side of box
	float B = d1 + d2;					// sum of box side lengths

	float invTotalSA = 1.0f / bb.getArea();		// inverse surface area of bounding box
	float bestCost = INF;
	int bestOffset = 0;

	// Loop over the object edges and evaluate the cost for each candidate splitting plane.
	for (int i = 0; i < 2 * numObjs; i++)
	{
		const edge& e = mEdges[axis][i];
		if (e.type == edge::END) nAbove--;
		float t = e.t, tmin = bb.mMin(axis), tmax = bb.mMax(axis);

		if (t > tmin && t < tmax)
		{
			// The probability of ray intersecting box below/above splitting plane
			// is given by the respective surface area / total surface area.
			float pBelow = 2 * (A + (t - tmin)*B) * invTotalSA;
			float pAbove = 2 * (A + (tmax - t)*B) * invTotalSA;

			// Compute empty bonus.
			float eb = (nBelow == 0 || nAbove == 0) ? EMPTY_BONUS : 0.0f;

			// Compute cost.
			float cost = TRAVERSAL_COST + (1.0f - eb) * INTERSECTION_COST * (pBelow*nBelow + pAbove * nAbove);
			if (cost < bestCost)
			{
				bestCost = cost;
				bestOffset = i;
			}

		}
		if (e.type == edge::START) nBelow++;
	}

	offset = bestOffset;
	return bestCost;
}



/** Setup the list of bounding box edges for the given objects along the specified axis.
*	The edge list is used for determining the optimal splitting plane.
*/
template<class T>
void KDTree<T>::setupEdges(const idxarray& objs, int axis)
{
	// Setup start/end edges for the objects in the objs list.
	int n = (int)objs.size();
	for (int i = 0; i < n; i++)
	{
		int idx = objs[i];
		const AABB& bb = mBoxes[idx];
		mEdges[axis][2 * i] = edge(bb.mMin(axis), idx, edge::START);
		mEdges[axis][2 * i + 1] = edge(bb.mMax(axis), idx, edge::END);
	}
	// Sort the list of edges.
	std::sort(&mEdges[axis][0], &mEdges[axis][2 * n]);
}



/** Helper function for computing the bounding boxes of all objects,
*	and a world box enclosing all objects.
*/
template<class T>
void KDTree<T>::computeBBox(AABB& world, AABB* boxes) const
{
	AABB worldBox, box;
	for (int i = 0; i < mNumObjs; i++)
	{
		mObjs[i].ptr->getAABB(box);
		boxes[i] = box;
		worldBox.include(box);
	}
	world = worldBox;
}