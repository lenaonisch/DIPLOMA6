/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

#pragma once
#include "stdafx.h"
#define sprintf_s sprintf 

#include "CRPatch.h"

using namespace std;

// Auxilary structure
struct IntIndex {
	int val;
	unsigned int index;
	bool operator<(const IntIndex& a) const { return val<a.val; }
};

// Structure for the leafs
struct LeafNode {
	// Constructors
	LeafNode() {}

	// IO functions
	void show(int delay, int width, int height); 

	// Probabilities for different classes of objects  pfg[0] - for 1st class, pfg[1] - for 2nd class etc.
	vector<float> pfg; 
	// Vector for average ratios of training objects for different classes.
	vector<float> vRatio;
	// Vectors from object center to training patches
	vector<vector<cv::Point> > vCenter;	
};

class CRTree {
public:
	// Constructors
	CRTree(const char* filename);
	// cp - number of classes!!!
	CRTree(int min_s, int max_d, CvRNG* pRNG) : min_samples(min_s), max_depth(max_d), num_leaf(0), cvRNG(pRNG){
		num_nodes = (int)pow(2.0,int(max_depth+1))-1;
		// num_nodes x 7 matrix as vector
		treetable = new int[num_nodes * 7];
		for(unsigned int i=0; i<num_nodes * 7; ++i) 
			treetable[i] = 0;
		// allocate memory for leafs
		leaf = new LeafNode[(int)pow(2.0,int(max_depth))];
		center_count = vector<int>(num_of_classes, 0);
		treetable_count = 0;
	}

	CRTree(){}
	CRTree operator=(CRTree& right)
	{
		if (this == &right) {
            return *this;
        }
        
		int* treetable = right.treetable;
		min_samples = right.min_samples;
		max_depth = right.max_depth;
		num_nodes = right.num_nodes;
		num_leaf = right.num_leaf;
		patch_size = right.patch_size;
		channels = right.channels;
		leaf = right.leaf;
		cvRNG = right.cvRNG;

		return *this;
	}
	~CRTree() {delete[] leaf; delete[] treetable;}

	// Set/Get functions
	unsigned int GetDepth() const {return max_depth;}
	unsigned int GetNumClasses() const {return num_of_classes;}

	// Regression
	const LeafNode* regression(uchar** ptFCh, int stepImg) const;
	const LeafNode* regression(uchar* ptFCh, int stepImg, int channels) const;

	// Training
	void growTree(const CRPatch& TrData, int samples);

	// IO functions
	bool saveTree(const char* filename) const;
	void showLeaves(int width, int height) const {
		for(unsigned int l=0; l<num_leaf; ++l)
			leaf[l].show(5000, width, height);
	}

	//int* amp_treetable;
	//unsigned int* amp_treepointer;
	int treetable_count;

	//int* amp_leafs;
	//unsigned int* amp_leafpointer;
	vector<int> center_count; // each value in vector is for separate class ob objects

	void ConvertTreeForPointers(int row, cv::Mat& treetable, vector<cv::Mat>& leafs, vector<cv::Mat>& leafpointer);
private: 

	// Private functions for training
	void grow(const vector<vector<const PatchFeature*> >& TrainSet, int node, unsigned int depth, int samples, vector<float> percentage);
	void makeLeaf(const vector<vector<const PatchFeature*> >& TrainSet, vector<float> percentage, int node);
	bool optimizeTest(vector<vector<const PatchFeature*> >& SetA, vector<vector<const PatchFeature*> >& SetB, const vector<vector<const PatchFeature*> >& TrainSet, int* test, unsigned int iter, unsigned int mode);
	void generateTest(int* test, unsigned int max_w, unsigned int max_h, unsigned int max_c);
	void evaluateTest(vector<vector<IntIndex> >& valSet, const int* test, const vector<vector<const PatchFeature*> >& TrainSet);
	void split(vector<vector<const PatchFeature*> >& SetA, vector<vector<const PatchFeature*> >& SetB, const vector<vector<const PatchFeature*> >& TrainSet, const vector<vector<IntIndex> >& valSet, int t);
	double measureSet(const vector<vector<const PatchFeature*> >& SetA, const vector<vector<const PatchFeature*> >& SetB, unsigned int mode) {
	  if (mode==0) 
		  return InfGain(SetA, SetB); 
	  else 
		  return -distMean(SetA, SetB);
	}
	double distMean(const vector<vector<const PatchFeature*>>& SetA, const vector<vector<const PatchFeature*>>& SetB)
	{
		double sum = 0;
		for (int i = 0; i < num_of_classes; i++)
		{
			sum += distMean(SetA[i],SetB[i]);
		}
		return sum;
	}
	double distMean(const vector<const PatchFeature*>& SetA, const vector<const PatchFeature*>& SetB);
	double InfGain(const vector<vector<const PatchFeature*> >& SetA, const vector<vector<const PatchFeature*> >& SetB);


	// Data structure

	// tree table
	// 2^(max_depth+1)-1 x 7 matrix as vector
	// column: leafindex x1 y1 x2 y2 channel thres
	// if node is not a leaf, leaf=-1
	int* treetable;

	// stop growing when number of patches is less than min_samples
	unsigned int min_samples;

	// depth of the tree: 0-max_depth
	unsigned int max_depth;
public:
	// number of nodes: 2^(max_depth+1)-1
	unsigned int num_nodes;

	// number of leafs
	unsigned int num_leaf;

	cv::Size patch_size;
	unsigned int channels; // 32

	//leafs as vector
	LeafNode* leaf;

	CvRNG *cvRNG;
};

inline const LeafNode* CRTree::regression(uchar** ptFCh, int stepImg) const{
	// pointer to current node
	const int* pnode = &treetable[0];
	int node = 0;

	// Go through tree until one arrives at a leaf, i.e. pnode[0]>=0)
	while(pnode[0]==-1) {
		// binary test 0 - left, 1 - right
		// Note that x, y are changed since the patches are given as matrix and not as image 
		// p1 - p2 < t -> left is equal to (p1 - p2 >= t) == false
		
		// pointer to channel
		uchar* ptC = ptFCh[pnode[5]];
		// get pixel values 
		int p1 = *(ptC+pnode[1]+pnode[2]*stepImg);
		int p2 = *(ptC+pnode[3]+pnode[4]*stepImg);
		// test
		bool test = ( p1 - p2 ) >= pnode[6];

		// next node: 2*node_id + 1 + test
		// increment node/pointer by node_id + 1 + test
		int incr = node+1+test;
		node += incr; //after this operation node contains node_id
		pnode += incr*7;
	}

	// return leaf
	return &leaf[pnode[0]];
}

inline const LeafNode* CRTree::regression(uchar* ptFCh, int stepImg, int channels) const{
	// pointer to current node
	const int* pnode = &treetable[0];
	int node = 0;

	// Go through tree until one arrives at a leaf, i.e. pnode[0]>=0)
	while(pnode[0]==-1) {
		// binary test 0 - left, 1 - right
		// Note that x, y are changed since the patches are given as matrix and not as image 
		// p1 - p2 < t -> left is equal to (p1 - p2 >= t) == false
		
		// pointer to channel
		//uchar* ptC = ptFCh[pnode[5]];
		// get pixel values 
		int p1 = *(ptFCh+pnode[1]*channels+pnode[2]*stepImg+pnode[5]); //pnode[1] - x - col, pnode[2] - y - row
		int p2 = *(ptFCh+pnode[3]*channels+pnode[4]*stepImg+pnode[5]);
		// test
		bool test = ( p1 - p2 ) >= pnode[6];

		// next node: 2*node_id + 1 + test
		// increment node/pointer by node_id + 1 + test
		int incr = node+1+test;
		node += incr; //after this operation node contains node_id
		pnode += incr*7;
	}

	// return leaf
	return &leaf[pnode[0]];
}

inline void CRTree::generateTest(int* test, unsigned int max_w, unsigned int max_h, unsigned int max_c) {
	test[0] = cvRandInt( cvRNG ) % max_w;
	test[1] = cvRandInt( cvRNG ) % max_h;
	test[2] = cvRandInt( cvRNG ) % max_w;
	test[3] = cvRandInt( cvRNG ) % max_h;
	test[4] = cvRandInt( cvRNG ) % max_c;
}
