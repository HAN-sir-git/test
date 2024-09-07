#include "cutpos.h"
#include "ui_cutpos.h"

CutPos::CutPos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CutPos)
{
    ui->setupUi(this);
}

CutPos::~CutPos()
{
    delete ui;
}
