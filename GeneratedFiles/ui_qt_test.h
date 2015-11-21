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
    QAction *actionLoad_config;
    QAction *actionTrain;
    QAction *actionShow;
    QAction *actionShow_leaves;
    QAction *actionLoad_config_file;
    QAction *actionDetect;
    QAction *actionBatch_detect;
    QAction *actionZoom_in;
    QAction *actionZoom_out;
    QAction *actionZoom_in_2;
    QAction *actionZoom_out_2;
    QAction *actionTest_local_max;
    QAction *actionMean_shift;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QScrollArea *scrollAreaInput;
    QWidget *scrollAreaWidgetContents_2;
    MouseLabel *lblInput;
    QTreeWidget *treeNegative;
    QTreeWidget *treeResults;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAddPositive;
    QPushButton *btnMarkNegative;
    QPushButton *btnClearAllSelections;
    QLabel *lblPositive;
    QLabel *lblNegative;
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
        actionOpen = new QAction(qt_testClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionLoad_config = new QAction(qt_testClass);
        actionLoad_config->setObjectName(QStringLiteral("actionLoad_config"));
        actionTrain = new QAction(qt_testClass);
        actionTrain->setObjectName(QStringLiteral("actionTrain"));
        actionTrain->setEnabled(false);
        actionShow = new QAction(qt_testClass);
        actionShow->setObjectName(QStringLiteral("actionShow"));
        actionShow_leaves = new QAction(qt_testClass);
        actionShow_leaves->setObjectName(QStringLiteral("actionShow_leaves"));
        actionShow_leaves->setEnabled(false);
        actionLoad_config_file = new QAction(qt_testClass);
        actionLoad_config_file->setObjectName(QStringLiteral("actionLoad_config_file"));
        actionDetect = new QAction(qt_testClass);
        actionDetect->setObjectName(QStringLiteral("actionDetect"));
        actionDetect->setEnabled(false);
        actionBatch_detect = new QAction(qt_testClass);
        actionBatch_detect->setObjectName(QStringLiteral("actionBatch_detect"));
        actionBatch_detect->setEnabled(false);
        actionZoom_in = new QAction(qt_testClass);
        actionZoom_in->setObjectName(QStringLiteral("actionZoom_in"));
        actionZoom_in->setEnabled(false);
        actionZoom_out = new QAction(qt_testClass);
        actionZoom_out->setObjectName(QStringLiteral("actionZoom_out"));
        actionZoom_out->setEnabled(false);
        actionZoom_in_2 = new QAction(qt_testClass);
        actionZoom_in_2->setObjectName(QStringLiteral("actionZoom_in_2"));
        actionZoom_out_2 = new QAction(qt_testClass);
        actionZoom_out_2->setObjectName(QStringLiteral("actionZoom_out_2"));
        actionTest_local_max = new QAction(qt_testClass);
        actionTest_local_max->setObjectName(QStringLiteral("actionTest_local_max"));
        actionMean_shift = new QAction(qt_testClass);
        actionMean_shift->setObjectName(QStringLiteral("actionMean_shift"));
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

        lblInput = new MouseLabel(tab);
        lblInput->setObjectName(QStringLiteral("lblInput"));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lblInput->sizePolicy().hasHeightForWidth());
        lblInput->setSizePolicy(sizePolicy1);
        lblInput->setScaledContents(true);

        gridLayout->addWidget(lblInput, 1, 1, 1, 1);

        treeNegative = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeNegative->setHeaderItem(__qtreewidgetitem);
        treeNegative->setObjectName(QStringLiteral("treeNegative"));
        treeNegative->setUniformRowHeights(true);
        treeNegative->setHeaderHidden(true);
        treeNegative->header()->setVisible(false);

        gridLayout->addWidget(treeNegative, 1, 4, 1, 1);

        treeResults = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        treeResults->setHeaderItem(__qtreewidgetitem1);
        treeResults->setObjectName(QStringLiteral("treeResults"));
        treeResults->setUniformRowHeights(true);
        treeResults->setHeaderHidden(true);

        gridLayout->addWidget(treeResults, 1, 3, 1, 1);

        widget = new QWidget(tab);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnAddPositive = new QPushButton(widget);
        btnAddPositive->setObjectName(QStringLiteral("btnAddPositive"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(btnAddPositive->sizePolicy().hasHeightForWidth());
        btnAddPositive->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(btnAddPositive);

        btnMarkNegative = new QPushButton(widget);
        btnMarkNegative->setObjectName(QStringLiteral("btnMarkNegative"));

        horizontalLayout->addWidget(btnMarkNegative);

        btnClearAllSelections = new QPushButton(widget);
        btnClearAllSelections->setObjectName(QStringLiteral("btnClearAllSelections"));

        horizontalLayout->addWidget(btnClearAllSelections);


        gridLayout->addWidget(widget, 0, 0, 1, 1);

        lblPositive = new QLabel(tab);
        lblPositive->setObjectName(QStringLiteral("lblPositive"));

        gridLayout->addWidget(lblPositive, 0, 3, 1, 1);

        lblNegative = new QLabel(tab);
        lblNegative->setObjectName(QStringLiteral("lblNegative"));

        gridLayout->addWidget(lblNegative, 0, 4, 1, 1);

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
        menuFile->addAction(actionTest_local_max);
        menuFile->addAction(actionMean_shift);
        menuSetup->addAction(actionLoad_config_file);
        menuSetup->addAction(actionTrain);
        menuSetup->addAction(actionShow_leaves);
        menuView->addAction(actionZoom_in_2);
        menuView->addAction(actionZoom_out_2);

        retranslateUi(qt_testClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(qt_testClass);
    } // setupUi

    void retranslateUi(QMainWindow *qt_testClass)
    {
        qt_testClass->setWindowTitle(QApplication::translate("qt_testClass", "qt_test", 0));
        actionOpen->setText(QApplication::translate("qt_testClass", "Open", 0));
        actionOpen->setShortcut(QApplication::translate("qt_testClass", "Ctrl+O", 0));
        actionLoad_config->setText(QApplication::translate("qt_testClass", "Load config", 0));
        actionTrain->setText(QApplication::translate("qt_testClass", "Train", 0));
        actionShow->setText(QApplication::translate("qt_testClass", "Show", 0));
        actionShow_leaves->setText(QApplication::translate("qt_testClass", "Show leaves", 0));
        actionLoad_config_file->setText(QApplication::translate("qt_testClass", "Load config file", 0));
        actionDetect->setText(QApplication::translate("qt_testClass", "Detect", 0));
        actionBatch_detect->setText(QApplication::translate("qt_testClass", "Batch detect", 0));
        actionZoom_in->setText(QApplication::translate("qt_testClass", "Zoom in", 0));
        actionZoom_out->setText(QApplication::translate("qt_testClass", "Zoom out", 0));
        actionZoom_in_2->setText(QApplication::translate("qt_testClass", "Zoom in", 0));
        actionZoom_in_2->setShortcut(QApplication::translate("qt_testClass", "Ctrl++", 0));
        actionZoom_out_2->setText(QApplication::translate("qt_testClass", "Zoom out", 0));
        actionZoom_out_2->setShortcut(QApplication::translate("qt_testClass", "Ctrl+-", 0));
        actionTest_local_max->setText(QApplication::translate("qt_testClass", "test local max", 0));
        actionMean_shift->setText(QApplication::translate("qt_testClass", "mean shift", 0));
        lblInput->setText(QString());
        btnAddPositive->setText(QApplication::translate("qt_testClass", "+", 0));
        btnMarkNegative->setText(QApplication::translate("qt_testClass", "-", 0));
        btnClearAllSelections->setText(QApplication::translate("qt_testClass", "Clear All", 0));
        lblPositive->setText(QApplication::translate("qt_testClass", "Positive", 0));
        lblNegative->setText(QApplication::translate("qt_testClass", "Negative", 0));
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
