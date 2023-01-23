#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void slot_show();

private:
    Ui::Widget *ui;
    QUdpSocket *mSocket;

    double m_sumValue;

    QVector<double> x,y;

};
#endif // WIDGET_H
