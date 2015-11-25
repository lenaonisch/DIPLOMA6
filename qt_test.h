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

	vector<Results> results;
	vector<std::string> filenames;

	qt_test(QWidget *parent = 0);
	~qt_test();
	bool loadFile(const QString &fileName, Results* res);
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);

private:
	Ui::qt_testClass ui;
	double scaleFactor;
	void DisplayPositiveFiles();
public slots:
	void on_actionOpen_triggered();
	void on_actionLoad_config_file_triggered();
	void on_actionTrain_triggered();
	void on_actionShow_leaves_triggered();
	void on_actionDetect_triggered();
	void on_actionZoom_in_2_triggered();
	void on_actionZoom_out_2_triggered();
	void on_actionBatch_detect_triggered();
	void on_actionTest_local_max_triggered();
	void on_actionMean_shift_triggered();
	void on_btnAddPositive_clicked();
	void on_treeResults_clicked();
	
};

#endif // QT_TEST_H
