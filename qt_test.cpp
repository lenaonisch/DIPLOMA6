#include "StdAfx.h"
#include "qt_test.h"

qt_test::qt_test(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.lblInput->setBackgroundRole(QPalette::Base);
	ui.lblInput->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	ui.lblInput->setScaledContents(true);

	//ui.scrollAreaInput->setBackgroundRole(QPalette::Dark);
	ui.scrollAreaInput->setWidget(ui.lblInput);
}

qt_test::~qt_test()
{

}

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


		ui.actionBatch_detect->setEnabled(true);
		ui.actionTrain->setEnabled(true);
		ui.actionShow_leaves->setEnabled(true);
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

void qt_test::on_actionBatch_detect_triggered()
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

void qt_test::on_actionDetect_triggered()
{
	
}

void qt_test::on_actionOpen_triggered()
{
	QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
//    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                      /* picturesLocations.isEmpty() ?*/ QDir::currentPath() /*: picturesLocations.last()*/);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");

	if (dialog.exec() == QDialog::Accepted && dialog.selectedFiles().length()>0)
	{
		if (loadFile(dialog.selectedFiles().first()))
		{
			currentImage = cv::imread(dialog.selectedFiles().first().toStdString());
			ui.actionDetect->setEnabled(true);
		}	
	}

	//QString fileName = QFileDialog::getOpenFileName(this,
 //        tr("Open XML File 1"), QDir::currentPath(), "All files (*.*);;Images (*.png *.xpm *.jpg)");

	//QFile file(fileName);
 //   if (!file.open(QIODevice::ReadOnly)) {
 //       return;
 //   }

	//QStringList stringList;
 //   QTextStream textStream(&file);

 //   while (!textStream.atEnd())
 //       stringList << textStream.readLine();

 //   file.close();

	////ui.lstOut->addItems(stringList);

}

bool qt_test::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
		ui.lblInput->setPixmap(QPixmap());
        ui.lblInput->adjustSize();
        return false;
    }
	
	ui.lblInput->setPixmap(QPixmap::fromImage(image));
	ui.lblInput->adjustSize();
	scaleFactor = 1.0;

    setWindowFilePath(fileName);
    return true;
}


void qt_test::scaleImage(double factor)
{
	Q_ASSERT(ui.lblInput->pixmap());
    scaleFactor *= factor;
    ui.lblInput->resize(scaleFactor * ui.lblInput->pixmap()->size());

    adjustScrollBar(ui.scrollAreaInput->horizontalScrollBar(), factor);
    adjustScrollBar(ui.scrollAreaInput->verticalScrollBar(), factor);
}

void qt_test::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void qt_test::on_actionZoom_in_2_triggered()
{
	scaleImage(1.25);
}
void qt_test::on_actionZoom_out_2_triggered()
{
	scaleImage(0.8);
}

void qt_test::on_actionTest_local_max_triggered()
{
	cv::Mat src(currentImage.rows, currentImage.cols, CV_8UC1);
	
	cv::cvtColor( currentImage, src, CV_RGB2GRAY );

	cv::Mat dst;
	
	int squire = ui.lineEditSquire->text().toInt();
	int threshold = ui.lineEditThreshold->text().toInt();
	vector<cv::Point> maxs;
	gall_forest_app.localMaxima(src, dst, squire, maxs);
	QString str = "";
	for (int i = 0;i<maxs.size();i++)
	{
		str +=maxs[i].x;
		str+=",";
		str+=maxs[i].y;
		str+= ";   ";
	}
	ui.plainTextEdit_Console->appendPlainText(str);
	cv::imshow("Max", dst);
}

void qt_test::on_actionMean_shift_triggered()
{

}