#pragma once
#ifndef TESTPARAMETERS_H
#define TESTPARAMETERS_H

#include "ui_testparameters.h"
#include "miscellaneous.h"
#include <QDialog>

namespace Ui {
class TestParameters;
}

class TestParameters : public QDialog
{
    Q_OBJECT

public:
    explicit TestParameters(QWidget *parent = 0);
    ~TestParameters();

	TestParam testParameters(){
		return TestParam(
			ui->edtThreshold->value(), 
			ui->edtGaussKernel->value(),
			ui->edtOutScale->value(),
			ui->edtProbToCast->value(),
			ui->cbSaveHoughAsImage->isChecked());
	}

    int gaussKernel(){return ui->edtGaussKernel->value();}
    int threshold(){return ui->edtThreshold->value();}
    int outScale(){return ui->edtOutScale->value();}
    float probability(){return ui->edtProbToCast->value();}

private:
    Ui::TestParameters *ui;
public slots:
    void on_cbCastVoteIfProb_stateChanged();
};

#endif // TESTPARAMETERS_H
