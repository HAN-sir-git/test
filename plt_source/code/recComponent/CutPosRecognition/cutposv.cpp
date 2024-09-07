#include "cutposv.h"
#include "ui_cutposv.h"

CutPosV::CutPosV(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CutPosV)
{
    ui->setupUi(this);
}

CutPosV::~CutPosV()
{
    delete ui;
}
