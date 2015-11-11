#include "StdAfx.h"
#include "GALL_app.h"

#ifndef QT_TEST_H
#define QT_TEST_H

//#include <QtWidgets/QMainWindow>
//#include <QFileDialog>
//#include <QTextStream>
//#include <QLabel>
//#include <QDebug>
#include "ui_qt_test.h"

class qt_test : public QMainWindow
{
	Q_OBJECT

public:

	GALL_app gall_forest_app;

	qt_test(QWidget *parent = 0);
	~qt_test();

private:
	Ui::qt_testClass ui;
public slots:
	void on_actionOpen_triggered();
	void on_actionLoad_config_file_triggered();
	void on_actionTrain_triggered();
	void on_actionShow_leaves_triggered();
	void on_actionDetect_triggered();
	//void on_btnRun_clicked();
};

#endif // QT_TEST_H
