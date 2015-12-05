#include "StdAfx.h"
#include "qt_test.h"

qt_test::qt_test(QWidget *parent)
	: QMainWindow(parent)
{
	have_forest = false;
	single_image_selected = false;
	last_selected_result = -1;
	ui.setupUi(this);

	ui.lblInput->setBackgroundRole(QPalette::Base);
	ui.lblInput->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	ui.lblInput->setScaledContents(true);

	//ui.scrollAreaInput->setBackgroundRole(QPalette::Dark);
	ui.scrollAreaInput->setWidget(ui.lblInput);
	results.clear();
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

		QMessageBox msg;
		QString str("Config file has been loaded successfully!");
		msg.setText(str);
		msg.exec();	


		ui.actionBatch_detect->setEnabled(true);
		ui.actionTrain->setEnabled(true);
		ui.actionShow_leaves->setEnabled(true);
		if (single_image_selected)
			ui.actionDetect->setEnabled(true);

		results.clear();
		have_forest = false;
		//gall_forest_app.show();
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
		msg.setText(str);
		msg.exec();	
	}
}

void qt_test::DisplayPositiveFiles()
{
	ui.treeResults->clear();
	QList<QTreeWidgetItem *> items;
	for (int i = 0; i < filepaths.size(); ++i)
	{
		QTreeWidgetItem* next = new QTreeWidgetItem(QStringList(QString(gall_forest_app.getFilename(filepaths[i]).c_str())));
		Results* res = &results[i];
		for (int j = 0;j < res->classes.size(); j++)
		{
			Rect* rect = &res->rects[j];
			next->addChild(new QTreeWidgetItem(QStringList(QString("class %1: %2, %3->%4, %5").arg(res->classes[j]).arg(rect->x).arg(rect->y).arg(rect->width).arg(rect->height))));
		}
		items.append(next);
	}
	ui.treeResults->insertTopLevelItems(0, items);
}

void qt_test::on_actionTrain_triggered()
{
	try
	{
		gall_forest_app.run_train();
		have_forest = true;
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
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
		msg.setText(str);
		msg.exec();	
	}
	//ui->plainTextEdit_Console
}

void qt_test::on_actionBatch_detect_triggered()
{
	try
	{
		last_selected_result = -1;
		filepaths.clear();
		results.clear();
		gall_forest_app.run_detect(have_forest, filepaths, results);
		DisplayPositiveFiles();
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
		msg.setText(str);
		msg.exec();	
	}
}

void qt_test::on_actionDetect_triggered()
{
	try
	{
		last_selected_result = -1;
		results.clear();
		gall_forest_app.run_detect(have_forest, filepaths[0], results);
		DisplayPositiveFiles();
	}
	catch (exception& e)
	{
		QMessageBox msg;
		QString str(e.what());
		msg.setText(str);
		msg.exec();	
	}
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
    dialog.selectMimeTypeFilter("image/jpeg");

	if (dialog.exec() == QDialog::Accepted && dialog.selectedFiles().length()>0)
	{
		if (loadFile(dialog.selectedFiles().first(), 0))
		{
			filepaths.clear();
			filepaths.push_back(dialog.selectedFiles().first().toStdString());
			currentImage = cv::imread(filepaths[0]);
			ui.treeResults->clear();
			last_selected_result = -1;
			if  (!gall_forest_app.configpath.empty())
			{
				ui.actionDetect->setEnabled(true);
			}
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

bool qt_test::loadFile(const QString &fileName, Results* res)
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
	
	single_image_selected = true;
	QPixmap pix = QPixmap::fromImage(image);
	if (res != NULL)
	{
		QPainter *paint = new QPainter(&pix);

		paint->setPen(*(new QColor("darkGreen")));
		for (int i = 0; i < res->rects.size(); i++)
		{
			QRect rect(res->rects[i].x, res->rects[i].y, res->rects[i].width, res->rects[i].height);
			paint->drawRect(rect);
			paint->drawText(res->rects[i].x, res->rects[i].y, QString(gall_forest_app.classes[res->classes[i]].c_str()));
		}
	}

	ui.lblInput->setPixmap(pix);
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
	//int squire = ui.lineEditSquire->text().toInt();
	int threshold = ui.lineEditThreshold->text().toInt();
	//ui.plainTextEdit_Console->appendPlainText(str);
	Size size;
	size.height = 48;
	size.width = 100;
	CRForestDetector crdet;
	vector<MaxPoint> locations;
	cv::Mat tmp;
	cvtColor( currentImage, tmp, CV_RGB2GRAY );
	tmp.convertTo(tmp, CV_8UC1);
	crdet.localMaxima(tmp, size, locations, 0, threshold);

	/////////
	
	QPixmap* pix_ = new QPixmap(filepaths[0].c_str());
	
	QPainter *paint = new QPainter(pix_);

	paint->setPen(*(new QColor("red")));
	for (int i = 0; i < locations.size(); i++)
	{
		paint->drawEllipse(locations[i].point.x, locations[i].point.y, 3,3);
		//paint->drawPoint(locations[i].point.x, locations[i].point.y);
		
	}
	

	ui.lblInput->setPixmap(*pix_);
	ui.lblInput->adjustSize();
	/////////
}

void qt_test::on_btnAddPositive_clicked()
{
	
}

void qt_test::on_actionMean_shift_triggered()
{

}

void qt_test::on_treeResults_clicked()
{
	QString text;
	QTreeWidgetItem * parent = ui.treeResults->currentItem()->parent();
	if (parent != 0)
		text = parent->text(0);
	else
	{
		text = ui.treeResults->currentItem()->text(0);
		QModelIndex model = ui.treeResults->currentIndex();
		int a = model.row();
		if (a != last_selected_result)
		{
			last_selected_result = a;
			loadFile(QString((gall_forest_app.configpath+gall_forest_app.impath+"\\").c_str())+text, &results[a]);
		}
	}
	
}

void qt_test::LoadRectsFromBWMasks()
{
	///////
	string outputfile = gall_forest_app.configpath+"/horses_rects.txt";
	ofstream out(outputfile);
	//////
	string dirwhite = gall_forest_app.configpath+"/white/";
	string list_file = dirwhite+"out.txt";
	const char * filename = (list_file).c_str();
	FILE * pFile = fopen (filename,"r");

	int N;
	fscanf(pFile, "%i", &N);
	out<<N<<endl;

	for (int i = 0; i<N; i++)
	{
		char buffer[200];
		fscanf(pFile, "%s", &buffer);
		string file = buffer;
		cv::Mat bw;
		bw = imread(dirwhite+file, 0);
		bw.convertTo(bw, CV_8UC1);

		int a,b,c,d;
		bool break_ = false;
		for (int y = 0; y<bw.rows;y++){
			for (int x = 0; x<bw.cols;x++){
				if (bw.at<uchar>(y,x) > 0){
					b = y;
					break_ = true;
					break;
				}
			}
			if (break_)
				break;
		}
		break_ = false;
		for (int x = 0; x<bw.cols;x++){
			for (int y = 0; y<bw.rows;y++){
				if (bw.at<uchar>(y,x) > 0){
					a = x;
					break_ = true;
					break;
				}
			}
			if (break_)
				break;
		}
		/////////////
		break_ = false;
		for (int y = bw.rows-1; y>0;y--){
			for (int x = 0; x<bw.cols;x++){
				if (bw.at<uchar>(y,x) > 0){
					d = y;
					break_ = true;
					break;
				}
			}
			if (break_)
				break;
		}

		break_ = false;
		for (int x = bw.cols-1; x>0;x--){
			for (int y = 0; y<bw.rows;y++){
				if (bw.at<uchar>(y,x) > 0){
					c = x;
					break_ = true;
					break;
				}
			}
			if (break_)
				break;
		}


		out<< file << '\t' << a << " " << b << " " << c << " " << d << " " << "0" << endl;
	}
	fclose(pFile);
	out.close();
}