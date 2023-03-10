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

    Ui::Widget *ui;
    QUdpSocket *mSocket;

    int m_countSample;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void slot_show();
};
#endif // WIDGET_H
