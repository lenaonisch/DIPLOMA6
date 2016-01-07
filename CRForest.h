/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

#pragma once
#include "stdafx.h"
#include "CRTree.h"


class CRForest {
public:
	// Constructors
	CRForest(int trees = 0) {
		vTrees.resize(trees);
	}

	CRForest& operator=(CRForest& right) { // copy/move constructor is called to construct arg
		if (this == &right) {
            return *this;
        }
        
		vTrees = right.vTrees;

		return *this;
	}
	~CRForest() {
		for(std::vector<CRTree*>::iterator it = vTrees.begin(); it != vTrees.end(); ++it) delete *it;
		vTrees.clear();
		amp_treetable.release();
		amp_treepointer.release();
		amp_leafs.release();
		amp_leafpointer.release();
	}

	// Set/Get functions
	void SetTrees(int n) {vTrees.resize(n);}
	int GetSize() const {return vTrees.size();}
	unsigned int GetDepth() const {return vTrees[0]->GetDepth();}
	//unsigned int GetNumCenter() const {return vTrees[0]->GetNumCenter();}
	
	// Regression 
	void regression(std::vector<const LeafNode*>& result, uchar** ptFCh, int stepImg) const;
	void regression(std::vector<const LeafNode*>& result, uchar* ptFCh, int stepImg, int channels) const;

	// Training
	void trainForest(int min_s, int max_d, CvRNG* pRNG, const CRPatch& TrData, int samples, const char* filename, unsigned int offset);

	// IO functions
	void saveForest(const char* filename, unsigned int offset = 0);
	void loadForest(const char* filename);
	void show(int w, int h) const {vTrees[0]->showLeaves(w,h);}

	// Trees
	std::vector<CRTree*> vTrees;
	

	cv::Mat amp_treetable; //size: num_of_trees * max.size of treetable
	int max_treetable_len;
	int amp_treetable_step1;
	cv::Mat amp_treepointer;
	int amp_treepointer_step1;

	cv::Mat amp_leafs;
	int amp_leafs_step;
	int max_num_leaf;
	cv::Mat amp_leafpointer;
	int amp_leafpointer_step;
};

inline void CRForest::regression(std::vector<const LeafNode*>& result, uchar** ptFCh, int stepImg) const{
	result.resize( vTrees.size() );
	for(int i=0; i<(int)vTrees.size(); ++i) {
		result[i] = vTrees[i]->regression(ptFCh, stepImg);
	}
}

inline void CRForest::regression(std::vector<const LeafNode*>& result, uchar* ptFCh, int stepImg, int channels) const{
	result.resize( vTrees.size() );
	for(int i=0; i<(int)vTrees.size(); ++i) {
		result[i] = vTrees[i]->regression(ptFCh, stepImg, channels);
	}
}

//Training
inline void CRForest::trainForest(int min_s, int max_d, CvRNG* pRNG, const CRPatch& TrData, int samples, const char* filename, unsigned int offset) {
	char buffer[200];
	for(int i=0; i < (int)vTrees.size(); ++i) {
		vTrees[i] = new CRTree(min_s, max_d, pRNG);
		vTrees[i]->growTree(TrData, samples);

		//and save
		sprintf_s(buffer,"%s%03d.txt",filename,i+offset);
		vTrees[i]->saveTree(buffer);
	}
}

// IO Functions
inline void CRForest::saveForest(const char* filename, unsigned int offset) {
	char buffer[200];
	for(unsigned int i=0; i<vTrees.size(); ++i) {
		sprintf_s(buffer,"%s%03d.txt",filename,i+offset);
		vTrees[i]->saveTree(buffer);
	}
}

inline void CRForest::loadForest(const char* filename) {
	char buffer[200];
	max_treetable_len = 0;
	max_num_leaf = 0;

	uint trees = vTrees.size();
	for(unsigned int i=0; i<trees; ++i) {
		sprintf_s(buffer,"%s%03d.txt",filename,i);
		vTrees[i] = new CRTree(buffer);
		if (vTrees[i]->treetable_count > max_treetable_len)
			max_treetable_len = vTrees[i]->treetable_count;
		if (vTrees[i]->num_leaf > max_num_leaf)
			max_num_leaf = vTrees[i]->num_leaf;
	}

	amp_treetable.create (trees, max_treetable_len*7, CV_32SC1);
	amp_treetable_step1 = amp_treetable.step1();
	amp_treepointer.create (trees, vTrees[0]->num_nodes, CV_32SC1);
	amp_treepointer_step1 = amp_treepointer.step1();

	amp_leafs.create (trees, 2 * vTrees[0]->center_count + max_num_leaf * num_of_classes * 3, CV_32SC1); 
	amp_leafs_step = amp_leafs.step1(); 
	amp_leafpointer.create (trees, max_num_leaf, CV_32SC1);
	amp_leafpointer_step = amp_leafpointer.step1();
	for(unsigned int i=0; i < vTrees.size(); ++i)
	{
		vTrees[i]->ConvertTreeForPointers(i, amp_treetable, amp_treepointer, amp_leafs, amp_leafpointer);
	}
}
