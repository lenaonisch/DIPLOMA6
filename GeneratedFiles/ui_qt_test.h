/********************************************************************************
** Form generated from reading UI file 'qt_test.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_TEST_H
#define UI_QT_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include <mouselabel.h>

QT_BEGIN_NAMESPACE

class Ui_qt_testClass
{
public:
    QAction *actionOpen;
    QAction *actionTrain;
    QAction *actionLoad_config_file;
    QAction *actionDetect;
    QAction *actionBatch_detect;
    QAction *actionZoom_in_2;
    QAction *actionZoom_out_2;
    QAction *actionLoad_test_images;
    QAction *actionRefresh;
    QAction *actionAdd_positive;
    QAction *actionAdd_negative;
    QAction *actionRemove;
    QAction *actionDrop_All_results;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QComboBox *lstClasses;
    QPushButton *btnAddPositive;
    QPushButton *btnAddNegative;
    MouseLabel *lblInput;
    QScrollArea *scrollAreaInput;
    QWidget *scrollAreaWidgetContents_2;
    QTreeWidget *treeResults;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnRefresh;
    QPushButton *btnRemove;
    QPushButton *btnDropAllResults;
    QWidget *tab_2;
    QPlainTextEdit *plainTextEdit_Console;
    QLineEdit *lineEditSquire;
    QLineEdit *lineEditThreshold;
    QLabel *label;
    QLabel *label_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSetup;
    QMenu *menuView;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *qt_testClass)
    {
        if (qt_testClass->objectName().isEmpty())
            qt_testClass->setObjectName(QStringLiteral("qt_testClass"));
        qt_testClass->resize(605, 440);
        QIcon icon;
        icon.addFile(QStringLiteral(":/qt_test/GeneratedFiles/icon.bmp"), QSize(), QIcon::Normal, QIcon::Off);
        qt_testClass->setWindowIcon(icon);
        actionOpen = new QAction(qt_testClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/qt_test/GeneratedFiles/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionTrain = new QAction(qt_testClass);
        actionTrain->setObjectName(QStringLiteral("actionTrain"));
        actionTrain->setEnabled(false);
        actionLoad_config_file = new QAction(qt_testClass);
        actionLoad_config_file->setObjectName(QStringLiteral("actionLoad_config_file"));
        actionDetect = new QAction(qt_testClass);
        actionDetect->setObjectName(QStringLiteral("actionDetect"));
        actionDetect->setEnabled(false);
        actionBatch_detect = new QAction(qt_testClass);
        actionBatch_detect->setObjectName(QStringLiteral("actionBatch_detect"));
        actionBatch_detect->setEnabled(false);
        actionZoom_in_2 = new QAction(qt_testClass);
        actionZoom_in_2->setObjectName(QStringLiteral("actionZoom_in_2"));
        actionZoom_out_2 = new QAction(qt_testClass);
        actionZoom_out_2->setObjectName(QStringLiteral("actionZoom_out_2"));
        actionLoad_test_images = new QAction(qt_testClass);
        actionLoad_test_images->setObjectName(QStringLiteral("actionLoad_test_images"));
        actionLoad_test_images->setEnabled(false);
        actionRefresh = new QAction(qt_testClass);
        actionRefresh->setObjectName(QStringLiteral("actionRefresh"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/qt_test/GeneratedFiles/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRefresh->setIcon(icon2);
        actionAdd_positive = new QAction(qt_testClass);
        actionAdd_positive->setObjectName(QStringLiteral("actionAdd_positive"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/qt_test/GeneratedFiles/plus.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_positive->setIcon(icon3);
        actionAdd_negative = new QAction(qt_testClass);
        actionAdd_negative->setObjectName(QStringLiteral("actionAdd_negative"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/qt_test/GeneratedFiles/minus.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_negative->setIcon(icon4);
        actionRemove = new QAction(qt_testClass);
        actionRemove->setObjectName(QStringLiteral("actionRemove"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/qt_test/GeneratedFiles/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemove->setIcon(icon5);
        actionDrop_All_results = new QAction(qt_testClass);
        actionDrop_All_results->setObjectName(QStringLiteral("actionDrop_All_results"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/qt_test/GeneratedFiles/bin.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDrop_All_results->setIcon(icon6);
        centralWidget = new QWidget(qt_testClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tab->sizePolicy().hasHeightForWidth());
        tab->setSizePolicy(sizePolicy);
        tab->setLayoutDirection(Qt::LeftToRight);
        tab->setAutoFillBackground(false);
        gridLayout = new QGridLayout(tab);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lstClasses = new QComboBox(widget);
        lstClasses->setObjectName(QStringLiteral("lstClasses"));

        horizontalLayout->addWidget(lstClasses);

        btnAddPositive = new QPushButton(widget);
        btnAddPositive->setObjectName(QStringLiteral("btnAddPositive"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnAddPositive->sizePolicy().hasHeightForWidth());
        btnAddPositive->setSizePolicy(sizePolicy1);
        btnAddPositive->setIcon(icon3);

        horizontalLayout->addWidget(btnAddPositive);

        btnAddNegative = new QPushButton(widget);
        btnAddNegative->setObjectName(QStringLiteral("btnAddNegative"));
        btnAddNegative->setIcon(icon4);

        horizontalLayout->addWidget(btnAddNegative);


        gridLayout->addWidget(widget, 0, 0, 1, 1);

        lblInput = new MouseLabel(tab);
        lblInput->setObjectName(QStringLiteral("lblInput"));
        QSizePolicy sizePolicy2(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lblInput->sizePolicy().hasHeightForWidth());
        lblInput->setSizePolicy(sizePolicy2);
        lblInput->setScaledContents(true);

        gridLayout->addWidget(lblInput, 1, 1, 1, 1);

        scrollAreaInput = new QScrollArea(tab);
        scrollAreaInput->setObjectName(QStringLiteral("scrollAreaInput"));
        scrollAreaInput->setMouseTracking(true);
        scrollAreaInput->setFrameShape(QFrame::Box);
        scrollAreaInput->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setEnabled(true);
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 547, 303));
        scrollAreaInput->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollAreaInput, 1, 0, 1, 1);

        treeResults = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeResults->setHeaderItem(__qtreewidgetitem);
        treeResults->setObjectName(QStringLiteral("treeResults"));
        treeResults->setUniformRowHeights(true);
        treeResults->setHeaderHidden(true);

        gridLayout->addWidget(treeResults, 1, 2, 1, 1);

        widget_2 = new QWidget(tab);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btnRefresh = new QPushButton(widget_2);
        btnRefresh->setObjectName(QStringLiteral("btnRefresh"));
        btnRefresh->setIcon(icon2);

        horizontalLayout_3->addWidget(btnRefresh);

        btnRemove = new QPushButton(widget_2);
        btnRemove->setObjectName(QStringLiteral("btnRemove"));
        btnRemove->setIcon(icon5);

        horizontalLayout_3->addWidget(btnRemove);

        btnDropAllResults = new QPushButton(widget_2);
        btnDropAllResults->setObjectName(QStringLiteral("btnDropAllResults"));
        btnDropAllResults->setIcon(icon6);

        horizontalLayout_3->addWidget(btnDropAllResults);


        gridLayout->addWidget(widget_2, 0, 2, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        plainTextEdit_Console = new QPlainTextEdit(tab_2);
        plainTextEdit_Console->setObjectName(QStringLiteral("plainTextEdit_Console"));
        plainTextEdit_Console->setGeometry(QRect(9, 9, 256, 192));
        lineEditSquire = new QLineEdit(tab_2);
        lineEditSquire->setObjectName(QStringLiteral("lineEditSquire"));
        lineEditSquire->setGeometry(QRect(360, 30, 113, 20));
        lineEditThreshold = new QLineEdit(tab_2);
        lineEditThreshold->setObjectName(QStringLiteral("lineEditThreshold"));
        lineEditThreshold->setGeometry(QRect(360, 70, 113, 20));
        label = new QLabel(tab_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(300, 30, 47, 13));
        label_2 = new QLabel(tab_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(300, 70, 47, 13));
        tabWidget->addTab(tab_2, QString());

        horizontalLayout_2->addWidget(tabWidget);

        qt_testClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(qt_testClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 605, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuSetup = new QMenu(menuBar);
        menuSetup->setObjectName(QStringLiteral("menuSetup"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        qt_testClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(qt_testClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        qt_testClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuSetup->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionDetect);
        menuFile->addAction(actionBatch_detect);
        menuFile->addSeparator();
        menuFile->addAction(actionAdd_positive);
        menuFile->addAction(actionAdd_negative);
        menuFile->addAction(actionRemove);
        menuFile->addAction(actionDrop_All_results);
        menuSetup->addAction(actionLoad_config_file);
        menuSetup->addAction(actionTrain);
        menuSetup->addAction(actionLoad_test_images);
        menuView->addAction(actionZoom_in_2);
        menuView->addAction(actionZoom_out_2);
        menuView->addAction(actionRefresh);

        retranslateUi(qt_testClass);
        QObject::connect(actionAdd_positive, SIGNAL(triggered()), btnAddPositive, SLOT(click()));
        QObject::connect(actionAdd_negative, SIGNAL(triggered()), btnAddNegative, SLOT(click()));
        QObject::connect(actionDrop_All_results, SIGNAL(triggered()), btnDropAllResults, SLOT(click()));
        QObject::connect(actionRemove, SIGNAL(triggered()), btnRemove, SLOT(click()));
        QObject::connect(actionRefresh, SIGNAL(triggered()), btnRefresh, SLOT(click()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(qt_testClass);
    } // setupUi

    void retranslateUi(QMainWindow *qt_testClass)
    {
        qt_testClass->setWindowTitle(QApplication::translate("qt_testClass", "qt_test", 0));
#ifndef QT_NO_TOOLTIP
        qt_testClass->setToolTip(QApplication::translate("qt_testClass", "Delete single item", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setText(QApplication::translate("qt_testClass", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("qt_testClass", "Ctrl+O", 0));
        actionTrain->setText(QApplication::translate("qt_testClass", "Train", 0));
        actionLoad_config_file->setText(QApplication::translate("qt_testClass", "Load config file", 0));
        actionLoad_config_file->setShortcut(QApplication::translate("qt_testClass", "Ctrl+Q", 0));
        actionDetect->setText(QApplication::translate("qt_testClass", "Detect", 0));
        actionDetect->setShortcut(QApplication::translate("qt_testClass", "Ctrl+D", 0));
        actionBatch_detect->setText(QApplication::translate("qt_testClass", "Batch detect", 0));
        actionBatch_detect->setShortcut(QApplication::translate("qt_testClass", "Ctrl+B", 0));
        actionZoom_in_2->setText(QApplication::translate("qt_testClass", "Zoom in", 0));
        actionZoom_in_2->setShortcut(QApplication::translate("qt_testClass", "Ctrl++", 0));
        actionZoom_out_2->setText(QApplication::translate("qt_testClass", "Zoom out", 0));
        actionZoom_out_2->setShortcut(QApplication::translate("qt_testClass", "Ctrl+-", 0));
        actionLoad_test_images->setText(QApplication::translate("qt_testClass", "Load test images", 0));
        actionRefresh->setText(QApplication::translate("qt_testClass", "Refresh", 0));
        actionAdd_positive->setText(QApplication::translate("qt_testClass", "Add positive", 0));
        actionAdd_negative->setText(QApplication::translate("qt_testClass", "Add negative", 0));
        actionRemove->setText(QApplication::translate("qt_testClass", "Remove", 0));
        actionDrop_All_results->setText(QApplication::translate("qt_testClass", "Drop All results", 0));
#ifndef QT_NO_TOOLTIP
        btnAddPositive->setToolTip(QApplication::translate("qt_testClass", "Add positive example", 0));
#endif // QT_NO_TOOLTIP
        btnAddPositive->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnAddNegative->setToolTip(QApplication::translate("qt_testClass", "Add negative example", 0));
#endif // QT_NO_TOOLTIP
        btnAddNegative->setText(QString());
        lblInput->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnRefresh->setToolTip(QApplication::translate("qt_testClass", "Reload all", 0));
#endif // QT_NO_TOOLTIP
        btnRefresh->setText(QString());
        btnRemove->setText(QString());
        btnRemove->setShortcut(QApplication::translate("qt_testClass", "Ctrl+D", 0));
#ifndef QT_NO_TOOLTIP
        btnDropAllResults->setToolTip(QApplication::translate("qt_testClass", "Delete all results", 0));
#endif // QT_NO_TOOLTIP
        btnDropAllResults->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("qt_testClass", "General", 0));
        lineEditSquire->setText(QApplication::translate("qt_testClass", "11", 0));
        lineEditThreshold->setText(QApplication::translate("qt_testClass", "50", 0));
        label->setText(QApplication::translate("qt_testClass", "Squire", 0));
        label_2->setText(QApplication::translate("qt_testClass", "Threshold", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("qt_testClass", "Information", 0));
        menuFile->setTitle(QApplication::translate("qt_testClass", "File", 0));
        menuSetup->setTitle(QApplication::translate("qt_testClass", "Setup", 0));
        menuView->setTitle(QApplication::translate("qt_testClass", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class qt_testClass: public Ui_qt_testClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_TEST_H
