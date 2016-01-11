/********************************************************************************
** Form generated from reading UI file 'trainparameters.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINPARAMETERS_H
#define UI_TRAINPARAMETERS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_TrainParameters
{
public:
    QDialogButtonBox *buttonBox;
    QSpinBox *edtTreeCount;
    QLabel *label;
    QCheckBox *cbUseFluct;
    QGroupBox *grFluctuation;
    QCheckBox *cbMoveHor;
    QCheckBox *cbResize;
    QCheckBox *cbRotate;
    QCheckBox *cbFlipHor;
    QCheckBox *cbFlipVertic;
    QCheckBox *cbMoveVert;
    QLabel *label_2;
    QSpinBox *edtTrainPercent;
    QLabel *label_3;
    QLineEdit *edtBinaryTests;
    QCheckBox *cbUseAllPositiveFromGUI;

    void setupUi(QDialog *TrainParameters)
    {
        if (TrainParameters->objectName().isEmpty())
            TrainParameters->setObjectName(QStringLiteral("TrainParameters"));
        TrainParameters->resize(400, 232);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TrainParameters->sizePolicy().hasHeightForWidth());
        TrainParameters->setSizePolicy(sizePolicy);
        buttonBox = new QDialogButtonBox(TrainParameters);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 200, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        edtTreeCount = new QSpinBox(TrainParameters);
        edtTreeCount->setObjectName(QStringLiteral("edtTreeCount"));
        edtTreeCount->setGeometry(QRect(110, 20, 51, 22));
        edtTreeCount->setMinimum(1);
        edtTreeCount->setMaximum(10);
        edtTreeCount->setValue(5);
        label = new QLabel(TrainParameters);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 20, 51, 16));
        cbUseFluct = new QCheckBox(TrainParameters);
        cbUseFluct->setObjectName(QStringLiteral("cbUseFluct"));
        cbUseFluct->setGeometry(QRect(231, 10, 111, 17));
        grFluctuation = new QGroupBox(TrainParameters);
        grFluctuation->setObjectName(QStringLiteral("grFluctuation"));
        grFluctuation->setEnabled(false);
        grFluctuation->setGeometry(QRect(210, 30, 171, 151));
        cbMoveHor = new QCheckBox(grFluctuation);
        cbMoveHor->setObjectName(QStringLiteral("cbMoveHor"));
        cbMoveHor->setGeometry(QRect(20, 60, 121, 17));
        cbMoveHor->setChecked(true);
        cbResize = new QCheckBox(grFluctuation);
        cbResize->setObjectName(QStringLiteral("cbResize"));
        cbResize->setGeometry(QRect(20, 20, 70, 17));
        cbResize->setChecked(true);
        cbRotate = new QCheckBox(grFluctuation);
        cbRotate->setObjectName(QStringLiteral("cbRotate"));
        cbRotate->setGeometry(QRect(20, 40, 70, 17));
        cbRotate->setChecked(true);
        cbFlipHor = new QCheckBox(grFluctuation);
        cbFlipHor->setObjectName(QStringLiteral("cbFlipHor"));
        cbFlipHor->setGeometry(QRect(20, 100, 111, 17));
        cbFlipHor->setChecked(true);
        cbFlipVertic = new QCheckBox(grFluctuation);
        cbFlipVertic->setObjectName(QStringLiteral("cbFlipVertic"));
        cbFlipVertic->setGeometry(QRect(20, 120, 111, 17));
        cbFlipVertic->setChecked(false);
        cbMoveVert = new QCheckBox(grFluctuation);
        cbMoveVert->setObjectName(QStringLiteral("cbMoveVert"));
        cbMoveVert->setGeometry(QRect(20, 80, 121, 17));
        cbMoveVert->setChecked(true);
        label_2 = new QLabel(TrainParameters);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 60, 71, 16));
        edtTrainPercent = new QSpinBox(TrainParameters);
        edtTrainPercent->setObjectName(QStringLiteral("edtTrainPercent"));
        edtTrainPercent->setGeometry(QRect(110, 60, 51, 22));
        edtTrainPercent->setMinimum(30);
        edtTrainPercent->setMaximum(100);
        edtTrainPercent->setSingleStep(5);
        edtTrainPercent->setValue(60);
        label_3 = new QLabel(TrainParameters);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 90, 91, 31));
        edtBinaryTests = new QLineEdit(TrainParameters);
        edtBinaryTests->setObjectName(QStringLiteral("edtBinaryTests"));
        edtBinaryTests->setGeometry(QRect(110, 100, 51, 20));
        cbUseAllPositiveFromGUI = new QCheckBox(TrainParameters);
        cbUseAllPositiveFromGUI->setObjectName(QStringLiteral("cbUseAllPositiveFromGUI"));
        cbUseAllPositiveFromGUI->setGeometry(QRect(20, 140, 141, 17));
        cbUseAllPositiveFromGUI->setChecked(true);

        retranslateUi(TrainParameters);
        QObject::connect(buttonBox, SIGNAL(accepted()), TrainParameters, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TrainParameters, SLOT(reject()));

        QMetaObject::connectSlotsByName(TrainParameters);
    } // setupUi

    void retranslateUi(QDialog *TrainParameters)
    {
        TrainParameters->setWindowTitle(QApplication::translate("TrainParameters", "Dialog", 0));
        label->setText(QApplication::translate("TrainParameters", "Tree count", 0));
        cbUseFluct->setText(QApplication::translate("TrainParameters", "Use fluctuations", 0));
        grFluctuation->setTitle(QApplication::translate("TrainParameters", "Fluctuations", 0));
        cbMoveHor->setText(QApplication::translate("TrainParameters", "Move horizontally", 0));
        cbResize->setText(QApplication::translate("TrainParameters", "Resize", 0));
        cbRotate->setText(QApplication::translate("TrainParameters", "Rotate", 0));
        cbFlipHor->setText(QApplication::translate("TrainParameters", "Flip horizontally", 0));
        cbFlipVertic->setText(QApplication::translate("TrainParameters", "Flip vertically", 0));
        cbMoveVert->setText(QApplication::translate("TrainParameters", "Move vertically", 0));
        label_2->setText(QApplication::translate("TrainParameters", "Train percent", 0));
#ifndef QT_NO_TOOLTIP
        edtTrainPercent->setToolTip(QApplication::translate("TrainParameters", "% of positive examples that is used for training. Rest is used for validation", 0));
#endif // QT_NO_TOOLTIP
        label_3->setText(QApplication::translate("TrainParameters", "<html><head/><body>Number of binary<br>tests generated</body></html>", 0));
        edtBinaryTests->setText(QApplication::translate("TrainParameters", "1500", 0));
#ifndef QT_NO_TOOLTIP
        cbUseAllPositiveFromGUI->setToolTip(QApplication::translate("TrainParameters", "Use all positive examples that were marked in GUI or detected automatically", 0));
#endif // QT_NO_TOOLTIP
        cbUseAllPositiveFromGUI->setText(QApplication::translate("TrainParameters", "Use all marked positives", 0));
    } // retranslateUi

};

namespace Ui {
    class TrainParameters: public Ui_TrainParameters {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINPARAMETERS_H
