#include "widget.h"
#include "menu.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui -> mainWidget->hide();

    mSocket =  new QUdpSocket(this);

    ui -> customPlot -> setInteraction(QCP::iRangeZoom, true);
    ui -> customPlot -> setInteraction(QCP::iRangeDrag, true);

//    ui -> customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
//    ui -> customPlot->axisRect()->setRangeZoom(Qt::Horizontal);

    ui -> customPlot -> xAxis -> setRange(0, 260);
    ui -> customPlot -> yAxis -> setRange(0, 260);

    ui -> customPlot -> addGraph();

    connect(mSocket, &QUdpSocket::readyRead, [&]() {

        QByteArray buffer;
        buffer.resize(mSocket->pendingDatagramSize());

        if (!buffer.size() == 8192) {
            return;
        }

        double _maxValue = 0;
        while (mSocket->hasPendingDatagrams()) {

            x.clear();
            y.clear();
            m_mediana = 0;

            QHostAddress sender;
            quint16 senderPort;
            mSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

            QString _st = "";
            for (int i = 0; i < buffer.size(); ++i) {

                QString _data =  QString::number(uchar(uchar(buffer.at(i))), 10);

                if (i % 2 ) {
                    x.push_back(_data.toDouble());
                }
                else {
                    y.push_back(_data.toDouble());
                    m_mediana += _data.toDouble();
                    if ( _data.toDouble() > _maxValue )
                        _maxValue = _data.toDouble();
                }

//                _st += (_data + " ");
            }
//            qDebug() << _st << "\n\n\n\n";

        }

        ui -> mediana -> setValue(m_mediana / y.size());
        ui -> maxValue -> setValue(_maxValue);

        ui -> customPlot -> graph(0) -> addData(x,y);
        ui -> customPlot -> replot();

        QVector<double> x_med(2) , y_med(2);
        x_med[0] = 0;
        y_med[0] = m_mediana / y.size();
        x_med[1] = 255;
        y_med[1] = m_mediana / y.size();

        ui -> customPlot -> addGraph();
        ui -> customPlot -> graph(1) -> addData(x_med,y_med);
        ui -> customPlot -> graph(1) -> setPen(QPen(QColor(Qt::green)));
        ui -> customPlot -> replot();

        QVector<double> x_max(2) , y_max(2);
        x_max[0] = 0;
        y_max[0] = _maxValue;
        x_max[1] = 255;
        y_max[1] = _maxValue;

        ui -> customPlot -> addGraph();
        ui -> customPlot -> graph(2) -> addData(x_max,y_max);
        ui -> customPlot -> graph(2) -> setPen(QPen(QColor(Qt::red)));
        ui -> customPlot -> replot();
    });


}

Widget::~Widget()
{
    delete mSocket;
    delete ui;
}

void Widget::slot_show()
{
    quint16 _port = 10002;
    if (ui -> mainWidget -> isHidden()) {
        ui -> mainWidget -> show();
        mSocket -> bind(QHostAddress::LocalHost, _port);
    }
    else {
        ui -> mainWidget -> hide();
        mSocket -> abort();
    }
}
