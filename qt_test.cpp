#include "StdAfx.h"
#include "qt_test.h"

qt_test::qt_test(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

qt_test::~qt_test()
{

}

//void qt_test::on_btnRun_clicked()
//{
//	QString fileName = QFileDialog::getOpenFileName(this,
//         tr("Open config"), QDir::currentPath(), "All files (*.*);;Images (*.png *.xpm *.jpg)");
//
//	QFile file(fileName);
//    if (!file.open(QIODevice::ReadOnly)) {
//        return;
//    }
//
//	int mode = 1;
//
//	//// Check argument
//	//if(argc<2) {
//	//	cout << "Usage: CRForest-Detector.exe mode [config.txt] [tree_offset]" << endl;
//	//	cout << "mode: 0 - train; 1 - show; 2 - detect" << endl;
//	//	cout << "tree_offset: output number for trees" << endl;
//	//	cout << "Load default: mode - 2" << endl; 
//	//} else
//	//	mode = atoi(argv[1]);
//	//
//	//off_tree = 0;	
//	//if(argc>3)
//	//	off_tree = atoi(argv[3]);
//
//	//// load configuration for dataset
//	//if(argc>2)
//	gall_forest_app.loadConfig(fileName.toLocal8Bit().constData(), mode);
//	//else
//	//	loadConfig("config.txt", mode);
//
//	//switch ( mode ) { 
//	//	case 0: 	
//	//		// train forest
//	//		run_train();
//	//		break;	
//
//	//	case 1: 
//	//				
//	//		// train forest
//	//		show();
//	//		break;	
//
//	//	default:
//
//	//		// detection
//	//		run_detect();
//	//		break;
//}

void qt_test::on_actionLoad_config_file_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open config"), QDir::currentPath(), "All files (*.*);;Images (*.png *.xpm *.jpg)");

	QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

	string filename = fileName.toLocal8Bit().constData();
	try
	{
		gall_forest_app.loadConfig(filename/*, mode*/);

		string directory;
		const size_t last_slash_idx = filename.rfind('/');
		if (std::string::npos != last_slash_idx)
		{
			directory = filename.substr(0, last_slash_idx);
		}
		gall_forest_app.configpath = directory;
		QMessageBox msg;
		QString str("Config file has been loaded successfully!");
		msg.setText(str);
		msg.exec();	
		//gall_forest_app.show();
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
		msg.setIcon(QMessageBox::Icon::Critical);
		msg.setText(str);
		msg.exec();	
	}
}


void qt_test::on_actionTrain_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open config"), QDir::currentPath(), "All files (*.*);;Images (*.png *.xpm *.jpg)");

	QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

	int mode = 1;

	string filename = fileName.toLocal8Bit().constData();
	try
	{
		gall_forest_app.loadConfig(filename/*, mode*/);

		string directory;
		const size_t last_slash_idx = filename.rfind('/');
		if (std::string::npos != last_slash_idx)
		{
			directory = filename.substr(0, last_slash_idx);
		}
		gall_forest_app.configpath = directory;
		gall_forest_app.run_train();
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
		msg.setIcon(QMessageBox::Icon::Critical);
		msg.setText(str);
		msg.exec();	
	}
}

void qt_test::on_actionShow_leaves_triggered()
{
	try
	{
		gall_forest_app.show();
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
		msg.setIcon(QMessageBox::Icon::Critical);
		msg.setText(str);
		msg.exec();	
	}
	//ui->plainTextEdit_Console
}

void qt_test::on_actionDetect_triggered()
{
	try
	{
		gall_forest_app.run_detect();
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
		msg.setIcon(QMessageBox::Icon::Critical);
		msg.setText(str);
		msg.exec();	
	}
}

void qt_test::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open XML File 1"), QDir::currentPath(), "All files (*.*);;Images (*.png *.xpm *.jpg)");

	QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

	QStringList stringList;
    QTextStream textStream(&file);

    while (!textStream.atEnd())
        stringList << textStream.readLine();

    file.close();

	ui.lstOut->addItems(stringList);

}