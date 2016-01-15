/********************************************************************************
** Form generated from reading UI file 'testparameters.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTPARAMETERS_H
#define UI_TESTPARAMETERS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_TestParameters
{
public:
    QDialogButtonBox *buttonBox;
    QSpinBox *edtOutScale;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *edtGaussKernel;
    QCheckBox *cbCastVoteIfProb;
    QLabel *label_4;
    QDoubleSpinBox *edtProbToCast;
    QSpinBox *edtThreshold;
    QCheckBox *cbSaveHoughAsImage;

    void setupUi(QDialog *TestParameters)
    {
        if (TestParameters->objectName().isEmpty())
            TestParameters->setObjectName(QStringLiteral("TestParameters"));
        TestParameters->resize(208, 235);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TestParameters->sizePolicy().hasHeightForWidth());
        TestParameters->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(TestParameters);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(10, 200, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        edtOutScale = new QSpinBox(TestParameters);
        edtOutScale->setObjectName(QStringLiteral("edtOutScale"));
        edtOutScale->setGeometry(QRect(130, 50, 61, 22));
        edtOutScale->setMinimum(10);
        edtOutScale->setMaximum(300);
        edtOutScale->setValue(100);
        label = new QLabel(TestParameters);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 50, 71, 16));
        label_2 = new QLabel(TestParameters);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 80, 101, 16));
        edtGaussKernel = new QSpinBox(TestParameters);
        edtGaussKernel->setObjectName(QStringLiteral("edtGaussKernel"));
        edtGaussKernel->setGeometry(QRect(130, 80, 61, 22));
        edtGaussKernel->setMinimum(3);
        edtGaussKernel->setMaximum(15);
        edtGaussKernel->setSingleStep(1);
        edtGaussKernel->setValue(3);
        cbCastVoteIfProb = new QCheckBox(TestParameters);
        cbCastVoteIfProb->setObjectName(QStringLiteral("cbCastVoteIfProb"));
        cbCastVoteIfProb->setGeometry(QRect(20, 110, 101, 31));
        cbCastVoteIfProb->setChecked(false);
        label_4 = new QLabel(TestParameters);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 20, 91, 16));
        edtProbToCast = new QDoubleSpinBox(TestParameters);
        edtProbToCast->setObjectName(QStringLiteral("edtProbToCast"));
        edtProbToCast->setEnabled(false);
        edtProbToCast->setGeometry(QRect(130, 120, 62, 22));
        edtProbToCast->setSingleStep(0.01);
        edtThreshold = new QSpinBox(TestParameters);
        edtThreshold->setObjectName(QStringLiteral("edtThreshold"));
        edtThreshold->setGeometry(QRect(130, 20, 61, 22));
        edtThreshold->setMinimum(50);
        edtThreshold->setMaximum(255);
        edtThreshold->setValue(150);
        cbSaveHoughAsImage = new QCheckBox(TestParameters);
        cbSaveHoughAsImage->setObjectName(QStringLiteral("cbSaveHoughAsImage"));
        cbSaveHoughAsImage->setGeometry(QRect(20, 150, 171, 17));

        retranslateUi(TestParameters);
        QObject::connect(buttonBox, SIGNAL(accepted()), TestParameters, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TestParameters, SLOT(reject()));

        QMetaObject::connectSlotsByName(TestParameters);
    } // setupUi

    void retranslateUi(QDialog *TestParameters)
    {
        TestParameters->setWindowTitle(QApplication::translate("TestParameters", "Dialog", 0));
        label->setText(QApplication::translate("TestParameters", "Output scale", 0));
        label_2->setText(QApplication::translate("TestParameters", "Gaussian kernel size", 0));
#ifndef QT_NO_TOOLTIP
        edtGaussKernel->setToolTip(QApplication::translate("TestParameters", "% of positive examples that is used for training. Rest is used for validation", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        cbCastVoteIfProb->setToolTip(QApplication::translate("TestParameters", "Use all positive examples that were marked in GUI or detected automatically", 0));
#endif // QT_NO_TOOLTIP
        cbCastVoteIfProb->setText(QApplication::translate("TestParameters", "Cast vote\n"
"if probability >", 0));
        label_4->setText(QApplication::translate("TestParameters", "Threshold for max", 0));
        cbSaveHoughAsImage->setText(QApplication::translate("TestParameters", "Save Hough spaces as images", 0));
    } // retranslateUi

};

namespace Ui {
    class TestParameters: public Ui_TestParameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTPARAMETERS_H
