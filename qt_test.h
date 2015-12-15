#pragma once
#include "StdAfx.h"
#include "GALL_app.h"

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
