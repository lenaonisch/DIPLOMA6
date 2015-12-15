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
	}

	// Set/Get functions
	void SetTrees(int n) {vTrees.resize(n);}
	int GetSize() const {return vTrees.size();}
	unsigned int GetDepth() const {return vTrees[0]->GetDepth();}
	//unsigned int GetNumCenter() const {return vTrees[0]->GetNumCenter();}
	
	// Regression 
	void regression(std::vector<const LeafNode*>& result, uchar** ptFCh, int stepImg) const;

	// Training
	void trainForest(int min_s, int max_d, CvRNG* pRNG, const CRPatch& TrData, int samples, const char* filename, unsigned int offset);

	// IO functions
	void saveForest(const char* filename, unsigned int offset = 0);
	void loadForest(const char* filename);
	void show(int w, int h) const {vTrees[0]->showLeaves(w,h);}

	// Trees
	std::vector<CRTree*> vTrees;
};

inline void CRForest::regression(std::vector<const LeafNode*>& result, uchar** ptFCh, int stepImg) const {
	result.resize( vTrees.size() );
	for(int i=0; i<(int)vTrees.size(); ++i) {
		result[i] = vTrees[i]->regression(ptFCh, stepImg);
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
	for(unsigned int i=0; i<vTrees.size(); ++i) {
		sprintf_s(buffer,"%s%03d.txt",filename,i);
		vTrees[i] = new CRTree(buffer);
	}
}
