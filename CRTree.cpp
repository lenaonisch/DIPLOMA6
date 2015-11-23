/* 
// Author: Juergen Gall, BIWI, ETH Zurich
// Email: gall@vision.ee.ethz.ch
*/

#pragma once
#include "stdafx.h"
#include "CRTree.h"

using namespace std;

/////////////////////// Constructors /////////////////////////////

// Read tree from file
CRTree::CRTree(const char* filename) {
	cout << "Load Tree " << filename << endl;

	int dummy;

	ifstream in(filename);
	if(in.is_open()) 
	{
		// allocate memory for tree table
		in >> max_depth;
		num_nodes = (int)pow(2.0,int(max_depth+1))-1;
		// num_nodes x 7 matrix as vector
		treetable = new int[num_nodes * 7];
		int* ptT = &treetable[0];
		
		// allocate memory for leafs
		in >> num_leaf;
		leaf = new LeafNode[num_leaf];

		// number of classes!!! 
		in >> num_classes;

		// read tree nodes
		for(unsigned int n=0; n<num_nodes; ++n) 
		{
			in >> dummy; in >> dummy;
			for(unsigned int i=0; i<7; ++i, ++ptT) {
				in >> *ptT;
			}
		}

		// read tree leafs
		LeafNode* ptLN = &leaf[0];
		char symbol;
		for(unsigned int l=0; l<num_leaf; ++l, ++ptLN) 
		{
			in >> dummy; // sequence number
			ptLN->pfg.resize(num_classes);
			ptLN->vCenter.resize(num_classes);
			ptLN->vSize.resize(num_classes);
			for (int p = 0; p < num_classes; p++) //  probabilities
				in >> ptLN->pfg[p];
				
			// read centers and rectangle's sizes. For each patch there are 4 numbers!
			for (int p = 0; p < num_classes; p++)
			{
				in >> symbol; // "|" symbol
				in >> dummy;
				ptLN->vCenter[p].resize(dummy);
				ptLN->vSize[p].resize(dummy);
				for(int i=0; i<dummy; ++i)
				{
					in >> symbol;
					in >> ptLN->vCenter[p][i].x;
					in >> ptLN->vCenter[p][i].y;
					in >> ptLN->vSize[p][i].width;
					in >> ptLN->vSize[p][i].height;
					in >> symbol; 
				}
			}
		}

	} else {
		cerr << "Could not read tree: " << filename << endl;
	}

	in.close();

}


/////////////////////// IO Function /////////////////////////////

bool CRTree::saveTree(const char* filename) const {
	cout << "Save Tree " << filename << endl;

	bool done = false;

	ofstream out(filename);
	if(out.is_open()) {

		out << max_depth << " " << num_leaf << " " << num_classes << endl;

		// save tree nodes
		int* ptT = &treetable[0];
		int depth = 0;
		unsigned int step = 2;
		for(unsigned int n=0; n<num_nodes; ++n) {
			// get depth from node
			if(n==step-1) {
				++depth;
				step *= 2;
			}

			out << n << " " << depth << " ";
			for(unsigned int i=0; i<7; ++i, ++ptT) {
				out << *ptT << " ";
			}
			out << endl;
		}
		out << endl;

		// save tree leafs
		LeafNode* ptLN = &leaf[0];
		for(unsigned int l=0; l<num_leaf; ++l, ++ptLN) 
		{
			out << l << " ";
			// write probabilities
			for (int p = 0; p<num_classes; p++)
				out << ptLN->pfg[p] << " ";

			// write centers and rectangle's sizes. For each patch there are 4 numbers!
			for (int p = 0; p<num_classes; p++)
			{
				out <<  "| " << ptLN->vCenter[p].size() << " ";
				
				for(unsigned int k=0; k<ptLN->vCenter[p].size(); ++k) 
				{
					out << "[" << ptLN->vCenter[p][k].x << " " << ptLN->vCenter[p][k].y << " ";
					out << ptLN->vSize[p][k].width << " " << ptLN->vSize[p][k].height << "] ";
				}
			}
			out << endl; // end of leaf
		}

		out.close();

		done = true;
	}


	return done;
}

/////////////////////// Training Function /////////////////////////////

// Start grow tree
void CRTree::growTree(const CRPatch& TrData, int samples) {
	// Get ratio positive patches/negative patches
	patch_size.height = TrData.vLPatches[0][0].vPatch[0].rows;
	patch_size.width = TrData.vLPatches[0][0].vPatch[0].cols;
	channels = TrData.vLPatches[0][0].vPatch.size();

	vector<float> vRatio(num_classes+1);
	vRatio[0] = 1;
	vector<vector<const PatchFeature*> > TrainSet( TrData.vLPatches.size() );
	float n0 = TrData.vLPatches[0].size();
	for(unsigned int l=0; l<TrainSet.size(); l++) {
		TrainSet[l].resize(TrData.vLPatches[l].size());
		
		vRatio[l] = TrainSet[l].size()/n0;
		
		for(unsigned int i=0; i < TrainSet[l].size(); i++) {
			TrainSet[l][i] = &TrData.vLPatches[l][i];
		}
	}

	// Grow tree
	grow(TrainSet, 0, 0, samples, vRatio );

	//after tree was constructed we can count average size for classes
}

// Called by growTree
void CRTree::grow(const vector<vector<const PatchFeature*> >& TrainSet, int node, unsigned int depth, int samples, vector<float> vRatio)
{
	int number_of_patches = 0;
	int class_left = 0;
	for (int i = 0; i <= num_classes ; i++)
	{
		if (TrainSet[i].size() > 0)
		{
			class_left++;
			number_of_patches += TrainSet[i].size();
		}
	}
	if(depth < max_depth && class_left > 1) // until max depth is not reached or patches of more than one classes left
	{	
		vector<vector<const PatchFeature*> > SetA;
		vector<vector<const PatchFeature*> > SetB;
		int test[6];

		// Set measure mode for split: 0 - classification, 1 - regression
		unsigned int measure_mode = 1;
		if( float(TrainSet[num_classes].size()) / float(number_of_patches) > 0.05 
						&& depth < max_depth-2 )
			measure_mode = cvRandInt( cvRNG ) % 2;
	
		// Find optimal test
		if( optimizeTest(SetA, SetB, TrainSet, test, samples, measure_mode) ) {
	
			// Store binary test for current node
			int* ptT = &treetable[node*7];
			ptT[0] = -1; ++ptT; 
			for(int t=0; t<6; ++t)
				ptT[t] = test[t];

			double countA = 0;
			double countB = 0;
			for(unsigned int l = 0; l < TrainSet.size(); l++) {
				countA += SetA[l].size(); countB += SetB[l].size();
			}

			// Go left
			// If enough patches are left continue growing else stop
			if(countA > min_samples) {
				grow(SetA, 2*node+1, depth+1, samples, vRatio);
			} else {
				makeLeaf(SetA, vRatio, 2*node+1);
			}

			// Go right
			// If enough patches are left continue growing else stop
			if(countB > min_samples) {
				grow(SetB, 2*node+2, depth+1, samples, vRatio);
			} else {
				makeLeaf(SetB, vRatio, 2*node+2);
			}

		} else {

			// Could not find split (only invalid one leave split)
			makeLeaf(TrainSet, vRatio, node);
	
		}	

	} else {

		// Only negative patches are left or maximum depth is reached
		makeLeaf(TrainSet, vRatio, node);
	
	}
}

// Create leaf node from patches 
void CRTree::makeLeaf(const std::vector<std::vector<const PatchFeature*> >& TrainSet, vector<float> vRatio, int node) {
	// Get pointer
	treetable[node*7] = num_leaf;
	LeafNode* ptL = &leaf[num_leaf];

	// Store data
	vector<float> scaled_pb(num_classes, 0);
	ptL->pfg.resize(num_classes, 0);
	ptL->vCenter.resize(num_classes);
	ptL->vSize.resize(num_classes);
	float sum = 0;
	for (int i = 0; i < num_classes; i++)
	{
		scaled_pb[i] = TrainSet[i].size()/vRatio[i];
		sum += scaled_pb[i];
	}
	for (int i = 0; i < num_classes; i++)
	{
		if (sum != 0)
			ptL->pfg[i] = scaled_pb[i] / sum;
		ptL->vCenter[i].resize( TrainSet[i].size() );
		ptL->vSize[i].resize( TrainSet[i].size() );
		for(unsigned int k = 0; k<TrainSet[i].size(); k++)
		{
			ptL->vCenter[i][k] = TrainSet[i][k]->center;
			ptL->vSize[i][k].width = TrainSet[i][k]->size.width;
			ptL->vSize[i][k].height = TrainSet[i][k]->size.height;
		}
	}

	// Increase leaf counter
	++num_leaf;
}

bool CRTree::optimizeTest  (vector<vector<const PatchFeature*> >& SetA, 
							vector<vector<const PatchFeature*> >& SetB, 
							const vector<vector<const PatchFeature*> >& TrainSet, 
							int* test, unsigned int iter, unsigned int measure_mode) {
	
	bool found = false;

	// temporary data for split into Set A and Set B
	vector<vector<const PatchFeature*> > tmpA(TrainSet.size());
	vector<vector<const PatchFeature*> > tmpB(TrainSet.size());

	// temporary data for finding best test
	vector<vector<IntIndex> > valSet(TrainSet.size());
	double tmpDist;
	// maximize!!!!
	double bestDist = -DBL_MAX; 
	int tmpTest[6];

	// Find best test of ITER iterations
	for(unsigned int i = 0; i < iter; ++i) {

		// reset temporary data for split
		for(unsigned int l = 0; l < TrainSet.size(); l++) {
			tmpA[l].clear();
			tmpB[l].clear(); 
		}

		// generate binary test without threshold
		generateTest(&tmpTest[0], patch_size.width, patch_size.height, channels);

		// compute value for each patch
		evaluateTest(valSet, &tmpTest[0], TrainSet);

		// find min/max values for threshold
		int vmin = INT_MAX;
		int vmax = INT_MIN;
		for(unsigned int l = 0; l < TrainSet.size(); l++)
		{
			if(valSet[l].size()>0)
			{
				if(vmin>valSet[l].front().val)  vmin = valSet[l].front().val;
				if(vmax<valSet[l].back().val )  vmax = valSet[l].back().val;
			}
		}
		int d = vmax-vmin;

		if(d>0) {

			// Find best threshold
			for(unsigned int j=0; j<10; ++j) { 

				// Generate some random thresholds
				int tr = (cvRandInt( cvRNG ) % (d)) + vmin; 

				// Split training data into two sets A,B accroding to threshold t 
				split(tmpA, tmpB, TrainSet, valSet, tr);

				// Do not allow empty set split (all patches end up in set A or B)
				int ACount = 0, BCount;
				for (int z = 0; z <= num_classes; z++)
				{
					ACount += tmpA[z].size();
					BCount += tmpB[z].size();
				}
				if( ACount >0 && BCount > 0 ) {

					// Measure quality of split with measure_mode 0 - classification, 1 - regression
					tmpDist = measureSet(tmpA, tmpB, measure_mode);

					// Take binary test with best split
					if(tmpDist>bestDist)
					{
						found = true;
						bestDist = tmpDist;
						for(int t=0; t<5;++t) test[t] = tmpTest[t];
						test[5] = tr;
						SetA = tmpA;
						SetB = tmpB;
					}
				}
			} // end for j
		}
	} // end iter

	// return true if a valid test has been found
	// test is invalid if only splits with an empty set A or B has been created
	return found;
}

void CRTree::evaluateTest(std::vector<std::vector<IntIndex> >& valSet, const int* test, const std::vector<std::vector<const PatchFeature*> >& TrainSet) {
	for(unsigned int l=0;l<TrainSet.size();++l) {
		valSet[l].resize(TrainSet[l].size());
		for(unsigned int i=0;i<TrainSet[l].size();++i) {

			// pointer to channel
			cv::Mat ptC = TrainSet[l][i]->vPatch[test[4]];
			// get pixel values 
			int step = ptC.step1();
			int size = ptC.type();
			int p1 = ptC.at<uchar>(test[1], test[0]);
			int p2 = ptC.at<uchar>(test[3], test[2]);
		
			valSet[l][i].val = p1 - p2;
			valSet[l][i].index = i;			
		}
		sort( valSet[l].begin(), valSet[l].end() );
	}
}

void CRTree::split(vector<vector<const PatchFeature*> >& SetA, vector<vector<const PatchFeature*> >& SetB, const vector<vector<const PatchFeature*> >& TrainSet, const vector<vector<IntIndex> >& valSet, int t) {
	for(unsigned int l = 0; l<TrainSet.size(); ++l) {
		// search largest value such that val<t 
		vector<IntIndex>::const_iterator it = valSet[l].begin();
		while(it!=valSet[l].end() && it->val<t) {
			++it;
		}

		SetA[l].resize(it-valSet[l].begin());
		SetB[l].resize(TrainSet[l].size()-SetA[l].size());

		it = valSet[l].begin();
		for(unsigned int i=0; i<SetA[l].size(); ++i, ++it) {
			SetA[l][i] = TrainSet[l][it->index];
		}
		
		it = valSet[l].begin()+SetA[l].size();
		for(unsigned int i=0; i<SetB[l].size(); ++i, ++it) {
			SetB[l][i] = TrainSet[l][it->index];
		}

	}
}

double CRTree::distMean(const std::vector<const PatchFeature*>& SetA, const std::vector<const PatchFeature*>& SetB)
{
	/////////////////// Set A /////////////////////////
	double meanAx = 0;
	double meanAy = 0;
	double distA = 0; // суммы квадратов
	if (SetA.size() > 0)
	{
		for(vector<const PatchFeature*>::const_iterator it = SetA.begin(); it != SetA.end(); ++it) 
		{	
			meanAx += (*it)->center.x;
			meanAy += (*it)->center.y;
		}
			
		meanAx /= SetA.size();
		meanAy /= SetA.size();
		

		for(std::vector<const PatchFeature*>::const_iterator it = SetA.begin(); it != SetA.end(); ++it)
		{	
			double tmp = (*it)->center.x - meanAx;
			distA += tmp*tmp;
			tmp = (*it)->center.y - meanAy;
			distA += tmp*tmp;
		}
	}

	/////////////////// Set B /////////////////////////
	double meanBx = 0;
	double meanBy = 0;
	double distB = 0; // суммы квадратов
	if (SetB.size() > 0)
	{
		for(vector<const PatchFeature*>::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
			meanBx += (*it)->center.x;
			meanBy += (*it)->center.y;
		}

		meanBx /= SetB.size();
		meanBy /= SetB.size();

		for(std::vector<const PatchFeature*>::const_iterator it = SetB.begin(); it != SetB.end(); ++it)
		{
			double tmp = (*it)->center.x - meanBx;
			distB += tmp*tmp;
			tmp = (*it)->center.y - meanBy;
			distB += tmp*tmp;
		}
	}

	double minDist = DBL_MAX;

	distA += distB;
	if (distA < minDist)
		minDist = distA;

	return minDist/double( SetA.size() + SetB.size() ); 
}

double CRTree::InfGain(const vector<vector<const PatchFeature*> >& SetA, const vector<vector<const PatchFeature*> >& SetB) {

	// get size of set A
	double sizeA = 0;
	for(vector<vector<const PatchFeature*> >::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
		sizeA += it->size();
	}

	// negative entropy: sum_i p_i*log(p_i)
	double n_entropyA = 0;
	for(vector<vector<const PatchFeature*> >::const_iterator it = SetA.begin(); it != SetA.end(); ++it) {
		double p = double( it->size() ) / sizeA;
		if (p > 0) 
			n_entropyA += p*log(p); 
	}

	// get size of set B
	double sizeB = 0;
	for(vector<vector<const PatchFeature*> >::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
		sizeB += it->size();
	}

	// negative entropy: sum_i p_i*log(p_i)
	double n_entropyB = 0;
	for(vector<vector<const PatchFeature*> >::const_iterator it = SetB.begin(); it != SetB.end(); ++it) {
		double p = double( it->size() ) / sizeB;
		if(p>0) n_entropyB += p*log(p); 
	}

	return (sizeA * n_entropyA + sizeB * n_entropyB) / (sizeA + sizeB); 
}

/////////////////////// IO functions /////////////////////////////

void LeafNode::show(int delay, int width, int height) {
	char buffer[200];

	if(vCenter.size()>0) {
		vector<IplImage*> iShow(vCenter[0].size());
		for(unsigned int c = 0; c<iShow.size(); ++c) {
			iShow[c] = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U , 1 );
			cvSetZero( iShow[c] );
			for(unsigned int i = 0; i<vCenter.size(); ++i) {
				int y = height/2+vCenter[i][c].y;
				int x = width/2+vCenter[i][c].x;

				if(x>=0 && y>=0 && x<width && y<height)
					cvSetReal2D( iShow[c],  y,  x, 255 );
			}
			sprintf_s(buffer,"Leaf%d",c);
			cvNamedWindow(buffer,1);
			cvShowImage(buffer, iShow[c]);
		}
		
		cvWaitKey(delay);
		
		for(unsigned int c = 0; c<iShow.size(); ++c) {
			sprintf_s(buffer,"Leaf%d",c);
			cvDestroyWindow(buffer);
			cvReleaseImage(&iShow[c]);
		}
	}
}
