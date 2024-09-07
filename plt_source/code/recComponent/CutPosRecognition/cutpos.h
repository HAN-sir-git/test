#ifndef CUTPOS_H
#define CUTPOS_H

#include <QWidget>

namespace Ui {
class CutPos;
}

class CutPos : public QWidget
{
    Q_OBJECT

public:
    explicit CutPos(QWidget *parent = nullptr);
    ~CutPos();

private:
    Ui::CutPos *ui;
};

#endif // CUTPOS_H
