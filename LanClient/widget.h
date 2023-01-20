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

    void on_color_activated(int _index);

    void on_xUpDown_valueChanged(int _arg1);

    void on_width_valueChanged(int _arg1);

    void slotRangeChanged(QCPRange);

private:
    Ui::Widget *ui;
    QUdpSocket *mSocket;

    int _x{0};

    double m_mediana;

    QVector<double> x,y/*, x_max, y_max*/;

};
#endif // WIDGET_H
