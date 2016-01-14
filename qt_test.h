#pragma once
#include "StdAfx.h"
#include "GALL_app.h"
#include "trainparameters.h"
#include "AMP_char.h"

#ifndef QT_TEST_H
#define QT_TEST_H

#include "ui_qt_test.h"

using namespace cv;

class qt_test : public QMainWindow
{
	Q_OBJECT

public:

	GALL_app gall_forest_app;
	cv::Mat currentImage;
	bool have_forest;
	//bool single_image_selected;

	vector<std::string> filepaths;
	int last_selected_result;

	// for retraining
	std::map<string, Results> positive;

	qt_test(QWidget *parent = 0);
	~qt_test();
	bool loadFile(const QString &fileName, Results* res);
	void DrawRect(cv::Rect rect, int class_, QColor color);
	void DrawRects(Results* res);
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);
	void GetSelectedImageResult(string& img_name, int& img_index, int& res_index);
	void GetSelectedImageResult(int& img_index, int& res_index);
	void GetSelectedImageResult(int& img_index);
private:
	Ui::qt_testClass ui;
	double scaleFactor;
	void DisplayPositiveFiles();
	void LoadRectsFromBWMasks();
	void AddPositiveRectToTree(QTreeWidgetItem* node, cv::Rect* rect, int class_);
	void AddPositiveFile(QString filename);
	void AddPositiveFile(string filename);
	// type: 0 - warning, 1 - information, 2 - error
	void ShowMessage(QString message, char type = 0)
	{
		switch (type)
		{
		case 0:
			QMessageBox::QMessageBox("Warning!", message, QMessageBox::Icon::Warning, QMessageBox::Ok,0,0,this).exec();
			break;
		case 2:
			QMessageBox::QMessageBox("Error!", message, QMessageBox::Icon::Critical, QMessageBox::Ok,0,0,this).exec();
			break;
		default:
			QMessageBox::QMessageBox("Information", message, QMessageBox::Icon::Information, QMessageBox::Ok,0,0,this).exec();
			break;
		}
	}

	void temp_AMP_test()
	{
		last_selected_result = -1;
		int img_index; //res_index not used
		GetSelectedImageResult(img_index);
		///////////////////////////////

		using namespace concurrency;
		cv::Mat image = cv::imread(filepaths[img_index].c_str(),CV_LOAD_IMAGE_COLOR);
		cv::Mat vImg(image.rows, image.cols, CV_8UC1);
		cvtColor( image, vImg, CV_RGB2GRAY );
		
		int time = clock();

		concurrency::extent<1> e((image.rows*image.cols+3)/4);
		array_view<unsigned int, 1> imgView (e, reinterpret_cast<unsigned int*>(vImg.data));

		int step = vImg.step1();

		concurrency::extent<2> e2(image.rows, image.cols);
		parallel_for_each(e2, [=](index<2>idx) restrict (amp)
		{
			unsigned int ch = read_uchar(imgView, idx, step);
			ch/=2;
			write_uchar(imgView, idx, step, ch);
			//imgView[idx] *= 0.25f;
		});

		imgView.synchronize();
		
		time = clock() - time;
		ShowMessage(QString("Time is %0").arg(time), 1);
		
		//imgView.convertTo(output, CV_8UC1);

		char buffer[200];
		sprintf_s(buffer,"%s/%s",(gall_forest_app.configpath + gall_forest_app.outpath).c_str(), "test.png");
		imwrite( buffer, vImg );
		
		image.release();
		//output.release();
		vImg.release();
	}
	void temp_Count_leaves()
	{
		gall_forest_app.loadForest();
		vector<int> num_of_centers (gall_forest_app.crForest.vTrees.size(),0);
		for(int i=0; i<(int)gall_forest_app.crForest.vTrees.size(); ++i) {
			for (int j = 0; j< gall_forest_app.crForest.vTrees[i]->num_leaf; j++)
				for (int k = 0; k<num_of_classes; k++)
					num_of_centers[i]+=gall_forest_app.crForest.vTrees[i]->leaf[j].vCenter[k].size();
		}
	}

	void qt_test::ResizeToBWMasks()
	{
		///////
		string outputfile = gall_forest_app.configpath+"/horses_rgb_rescaled/";
		string inputfile = gall_forest_app.configpath+"/trainpos/";
		//ofstream out(outputfile);
		//////
		string dirwhite = gall_forest_app.configpath+"/figure_ground/";
		string list_file = gall_forest_app.configpath+"/out.txt";
		const char * filename = (list_file).c_str();
		FILE * pFile = fopen (filename,"r");

		int N;
		fscanf(pFile, "%i", &N);
		

		for (int i = 0; i<N; i++)
		{
			char buffer[200];
			fscanf(pFile, "%s", &buffer);
			string file = buffer;
			cv::Mat bw;
			bw = imread(dirwhite+file, 0);
			bw.convertTo(bw, CV_8UC1);

			cv::Mat rgb;
			cv::Mat rgbr;
			rgb = imread(inputfile+file, CV_LOAD_IMAGE_COLOR);
			cv::resize(rgb, rgbr, bw.size());
			cv::imwrite(outputfile+file, rgbr);
		}
	}
public slots:
	void on_actionOpen_triggered();
	void on_actionLoad_config_file_triggered();
	void on_actionTrain_triggered();
	void on_actionLoad_test_images_triggered();
	void on_actionShow_leaves_triggered();
	void on_actionDetect_triggered();
	void on_actionZoom_in_2_triggered();
	void on_actionZoom_out_2_triggered();
	void on_actionBatch_detect_triggered();
	void on_actionTest_local_max_triggered();
	void on_actionMean_shift_triggered();
	void on_btnAddPositive_clicked();
	void on_btnAddNegative_clicked();
	void on_btnRefresh_clicked();
	void on_btnDropAllResults_clicked();
	void on_btnRemove_clicked();
	void on_treeResults_clicked();
	
};

#endif // QT_TEST_H
