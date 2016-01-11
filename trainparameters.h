#ifndef TRAINPARAMETERS_H
#define TRAINPARAMETERS_H

#include "ui_trainparameters.h"
#include <QDialog>

namespace Ui {
class TrainParameters;
}

class TrainParameters : public QDialog
{
    Q_OBJECT

public:
    explicit TrainParameters(QWidget *parent = 0);
    ~TrainParameters();

	bool* bFluct() 
	{
		if (ui->cbUseFluct->isChecked())
		{
			bool* param = new bool[6];

			param[0] = ui->cbResize->isChecked();
			param[1] = ui->cbRotate->isChecked();
            param[2] = ui->cbMoveHor->isChecked();
            param[3] = ui->cbMoveVert->isChecked();
            param[4] = ui->cbFlipHor->isChecked();
            param[5] = ui->cbFlipVertic->isChecked();
			return param;
		}
		else
			return NULL;
	}
	bool bUseAllMarkedInGUI(){return ui->cbUseAllPositiveFromGUI->isChecked();}

	int trees(){return ui->edtTreeCount->value();}
	float trainPart(){return ui->edtTrainPercent->value()/100.0;}
	int binary_tests_iterations(){return (ui->edtBinaryTests->text()).toInt();}

private:
    Ui::TrainParameters *ui;
public slots:
	void on_cbUseFluct_stateChanged();
};

#endif // TRAINPARAMETERS_H
