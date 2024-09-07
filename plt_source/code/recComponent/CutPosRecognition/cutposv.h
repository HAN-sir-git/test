#ifndef CUTPOSV_H
#define CUTPOSV_H

#include <QWidget>

namespace Ui {
class CutPosV;
}

class CutPosV : public QWidget
{
    Q_OBJECT

public:
    explicit CutPosV(QWidget *parent = nullptr);
    ~CutPosV();

private:
    Ui::CutPosV *ui;
};

#endif // CUTPOSV_H
