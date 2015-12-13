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
	ui.btnAddPositive->setEnabled(false);
	ui.btnMarkNegative->setEnabled(false);

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

		QMessageBox msg;
		QString str("Config file has been loaded successfully!");
		msg.setText(str);
		msg.exec();	


		ui.actionBatch_detect->setEnabled(true);
		ui.actionTrain->setEnabled(true);
		ui.actionLoad_test_images->setEnabled(true);
		if (single_image_selected)
			ui.actionDetect->setEnabled(true);

		ui.btnAddPositive->setEnabled(true);
		ui.btnMarkNegative->setEnabled(true);

		have_forest = false;

		ui.lstClasses->clear();
		for (int i = 0; i < gall_forest_app.classes.size(); i++)
			ui.lstClasses->addItem(QString("class %1: %2").arg(i).arg(gall_forest_app.classes[i].c_str()));
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

void qt_test::AddPositiveRectToTree(QTreeWidgetItem* node, cv::Rect* rect, int class_)
{
	QString text = QString("%1: %2, %3->%4, %5").arg(gall_forest_app.classes[class_].c_str()).arg(rect->x).arg(rect->y).arg(rect->width).arg(rect->height);
	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(text));
	item->setToolTip(0, text);
	node->addChild(item);
}

void qt_test::AddPositiveFile(QString filename)
{
	string fileName_std = filename.toStdString();
	QTreeWidgetItem* next = new QTreeWidgetItem(QStringList(QString(gall_forest_app.getFilename(fileName_std).c_str())));
	next->setToolTip(0, filename);
	ui.treeResults->addTopLevelItem(next);
}

void qt_test::AddPositiveFile(string filename)
{
	QTreeWidgetItem* next = new QTreeWidgetItem(QStringList(QString(gall_forest_app.getFilename(filename).c_str())));
	next->setToolTip(0, QString(filename.c_str()));
	ui.treeResults->addTopLevelItem(next);
}

void qt_test::DisplayPositiveFiles()
{
	ui.treeResults->clear();
	QList<QTreeWidgetItem *> items;
	for (int i = 0; i < filepaths.size(); ++i)
	{
		QTreeWidgetItem* next = new QTreeWidgetItem(QStringList(QString(gall_forest_app.getFilename(filepaths[i]).c_str())));
		next->setToolTip(0, QString(filepaths[i].c_str()));
		Results* res = &positive[filepaths[i]];
		for (int j = 0;j < res->classes.size(); j++)
		{
			AddPositiveRectToTree(next, &res->rects[j], res->classes[j]);
		}
		items.append(next);
	}
	ui.treeResults->addTopLevelItems(items);
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
		gall_forest_app.run_detect(have_forest, positive);
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
		int img_index; //res_index not used
		GetSelectedImageResult(img_index);
		if (img_index < 0)
			img_index = filepaths.size()-1;
		if (positive[filepaths[img_index]].processed)
		{
			QMessageBox msg;
			QString str("Image has been already processed!");
			msg.setText(str);
			msg.exec();	
			return;
		}
		gall_forest_app.run_detect(have_forest, filepaths[img_index], positive[filepaths[img_index]]);
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
			string str = dialog.selectedFiles().first().toStdString();
			filepaths.push_back(str);
			AddPositiveFile(str);

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
			paint->drawText(res->rects[i].x-2, res->rects[i].y-2, QString(gall_forest_app.classes[res->classes[i]].c_str()));
		}
	}

	ui.lblInput->setPixmap(pix);
	ui.lblInput->adjustSize();
	scaleFactor = 1.0;

    setWindowFilePath(fileName);
    return true;
}

void qt_test::DrawRect(cv::Rect rect, int class_, QColor* color)
{
	QPixmap pixmap = *(ui.lblInput->pixmap());
	QPainter painter(&pixmap);              

	painter.setPen(*color);
	QRect qrect(rect.x, rect.y, rect.width, rect.height);
	painter.drawRect(qrect);
	painter.drawText(rect.x-2, rect.y-2, QString(gall_forest_app.classes[class_].c_str()));

	ui.lblInput->setPixmap(pixmap);     
}

void qt_test::DrawRects(Results* res)
{
	QPixmap pixmap = *(ui.lblInput->pixmap());
	QPainter painter(&pixmap);              

	if (res != NULL)
	{
		for (int i = 0; i < res->rects.size(); i++)
		{
			painter.setPen(res->colors[i]);
			QRect qrect(res->rects[i].x, res->rects[i].y, res->rects[i].width, res->rects[i].height);
			painter.drawRect(qrect);
			painter.drawText(qrect.x()-2, qrect.y()-2, QString(gall_forest_app.classes[res->classes[i]].c_str()));
		}
		ui.lblInput->setPixmap(pixmap); 
	}
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
	MouseLabel *lbl = ui.lblInput;
	if (lbl->dx > 0 && lbl->dy > 0)
	{
		int img_index;
		GetSelectedImageResult(img_index);
		int class_num = ui.lstClasses->currentIndex();
		img_index = (img_index >= 0) ? img_index: filepaths.size()-1;
		string filename_ = filepaths[img_index];
		cv::Rect rect(lbl->x, lbl->y, lbl->dx, lbl->dy);
		Results& res = positive[filename_];
		res.classes.push_back(class_num);
		res.rects.push_back(rect);
		QTreeWidgetItem* treeItem = ui.treeResults->topLevelItem(img_index);
		AddPositiveRectToTree(treeItem, &rect, class_num);
	}
	else
	{
		QMessageBox msg;
		QString str("Nothing selected!");
		msg.setText(str);
		msg.exec();	
	}
}

void qt_test::on_btnAddNegative_clicked()
{ 
	if (ui.lblInput->x > 0 && ui.lblInput->y > 0)
	{
		int img_index, 
			res_index; 
		GetSelectedImageResult(img_index, res_index);

	}
	else
	{

	}
}

void qt_test::on_actionMean_shift_triggered()
{}

void qt_test::GetSelectedImageResult(int& img_index)
{
	img_index = -1;
	if (!ui.treeResults->currentItem())
	{
		return;
	}
	QTreeWidgetItem * parent = ui.treeResults->currentItem()->parent();
	
	if (parent != 0)
	{
		img_index = ui.treeResults->indexOfTopLevelItem(parent);
	}
	else
	{
		QModelIndex model = ui.treeResults->currentIndex();
		img_index = model.row();
	}
}

void qt_test::GetSelectedImageResult(int& img_index, int& res_index)
{
	res_index = -1;
	img_index = -1;
	if (!ui.treeResults->currentItem())
	{
		return;
	}
	QTreeWidgetItem * parent = ui.treeResults->currentItem()->parent();
	
	if (parent != 0)
	{
		res_index = ui.treeResults->currentIndex().row(); // index of result
		img_index = ui.treeResults->indexOfTopLevelItem(parent);
	}
	else
	{
		QModelIndex model = ui.treeResults->currentIndex();
		img_index = model.row();
	}
}

void qt_test::GetSelectedImageResult(string& img_name, int& img_index, int& res_index)
{
	QTreeWidgetItem * parent = ui.treeResults->currentItem()->parent();
	res_index = -1;
	img_index;
	if (parent != 0)
	{
		res_index = ui.treeResults->currentIndex().row(); // index of result
		img_name = parent->text(0).toStdString();
		img_index = ui.treeResults->indexOfTopLevelItem(parent);
	}
	else
	{
		img_name = ui.treeResults->currentItem()->text(0).toStdString();
		QModelIndex model = ui.treeResults->currentIndex();
		img_index = model.row();
	}
}

void qt_test::on_treeResults_clicked()
{
	
	int img_index, res_index;
	string text;
	GetSelectedImageResult(text, img_index, res_index);
	if (img_index != last_selected_result)
	{
		last_selected_result = img_index;
		loadFile(QString((filepaths[img_index]).c_str()), &positive[filepaths[img_index]]);
		if (res_index >= 0)
		{
			Results res = positive[filepaths[img_index]];
			DrawRect(res.rects[res_index], res.classes[res_index], new QColor("violet"));
		}
	}
	else
	{
		if (res_index >= 0)
		{
			Results res = positive[filepaths[img_index]];
			res.colors = vector<QColor> (res.classes.size(), QColor("darkGreen"));
			res.colors[res_index] = QColor("violet");
			DrawRects(&res);
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
	std::fclose(pFile);
	out.close();
}

void qt_test::on_actionLoad_test_images_triggered()
{
	cout<<"sdfsfsf";
	try
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Clean", "Remove previously downloaded files?",
			QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
		if (reply == QMessageBox::Yes) {
			filepaths.clear();
			gall_forest_app.loadImFile(filepaths);
		} else if (reply == QMessageBox::No){
			vector<string> text_files;
			gall_forest_app.loadImFile(text_files);
			filepaths.insert(filepaths.end(), text_files.begin(), text_files.end());
		}
		else return;
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