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

    ui -> customPlot -> xAxis -> setRange(0, 1500);
    ui -> customPlot -> yAxis -> setRange(-750, 750);

    ui -> customPlot -> addGraph();

    connect(mSocket, &QUdpSocket::readyRead, [&]() {
        ui -> customPlot -> clearGraphs();
        ui -> customPlot -> addGraph();

        QByteArray buffer;
        buffer.resize(mSocket->pendingDatagramSize());

        if (!buffer.size() == 8192) {
            return;
        }

        double _maxValue = 0;
        int _xValue = 0;
        while (mSocket->hasPendingDatagrams()) {

            x.clear();
            y.clear();
            m_sumValue = 0;

            QHostAddress sender;
            quint16 senderPort;
            mSocket -> readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

            QString _st = "";
            for (int i = 0; i < buffer.size(); ++i) {

                qint16 _data =  qint16(buffer.at(i));

                x.push_back(i);
                y.push_back(_data);
                m_sumValue += _data;
                if ( _data > _maxValue ) {
                    _maxValue = _data;
                    _xValue = i;
                }

//                _st += (_data + " ");
            }
//            qDebug() << _st << "\n\n\n\n";

        }

        double _mediana = m_sumValue / double(y.size());

        ui -> mediana -> setValue(_mediana);
        ui -> maxValue -> setValue(_maxValue);

        ui -> customPlot -> graph(0) -> addData(x,y);
        ui -> customPlot -> replot();

        QVector<double> x_med(2) , y_med(2);
        x_med[0] = 0;
        y_med[0] = _mediana;
        x_med[1] = buffer.size();
        y_med[1] = _mediana;

        QPen _pen;
        _pen.setBrush((QColor(Qt::green)));
        _pen.setWidth(3);


        ui -> customPlot -> addGraph();
        ui -> customPlot -> graph(1) -> addData(x_med,y_med);
        ui -> customPlot -> graph(1) -> setPen(_pen);
        ui -> customPlot -> replot();

        QVector<double> x_max(2) , y_max(2);
        x_max[0] = _xValue;
        y_max[0] = _maxValue;
        x_max[1] = _xValue;
        y_max[1] = _maxValue;

        ui -> customPlot -> addGraph();
        ui -> customPlot -> graph(2) -> addData(x_max,y_max);

        _pen.setBrush((QColor(Qt::red)));
        _pen.setWidth(5);

        ui -> customPlot -> graph(2) -> setPen(_pen);
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
