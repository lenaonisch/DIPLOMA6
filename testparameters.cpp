#include "StdAfx.h"
#include "testparameters.h"
#include "ui_testparameters.h"

TestParameters::TestParameters(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestParameters)
{
    ui->setupUi(this);
}

TestParameters::~TestParameters()
{
    delete ui;
}

void TestParameters::on_cbCastVoteIfProb_stateChanged()
{
	if (!ui->cbCastVoteIfProb->isChecked())
		ui->edtProbToCast->setValue(0);
    ui->edtProbToCast->setEnabled(ui->cbCastVoteIfProb->isChecked());
}

