#include "StdAfx.h"
#include "trainparameters.h"
#include "ui_trainparameters.h"

TrainParameters::TrainParameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainParameters)
{
    ui->setupUi(this);
	ui->cbUseAllPositiveFromGUI->hide();
}

TrainParameters::~TrainParameters()
{
    delete ui;
}

void TrainParameters::on_cbUseFluct_stateChanged()
{
	ui->grFluctuation->setEnabled(ui->cbUseFluct->isChecked());
}

