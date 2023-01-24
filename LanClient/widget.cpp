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

    ui -> customPlot -> legend -> setVisible(true);
    ui -> customPlot -> legend -> setBorderPen(QColor(Qt::red));
    ui -> customPlot -> legend -> setMinimumSize(170, 90);

//    ui -> customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
//    ui -> customPlot->axisRect()->setRangeZoom(Qt::Horizontal);

    ui -> customPlot -> xAxis -> setRange(0, 1000);
    ui -> customPlot -> yAxis -> setRange(-350, 350);

    m_countSample = 0;
    connect(mSocket, &QUdpSocket::readyRead, [&]() {
        m_countSample++;

        ui -> customPlot -> clearGraphs();

        QByteArray buffer;
        buffer.resize(mSocket->pendingDatagramSize());

        if (!buffer.size() == 8192) {
            return;
        }

        double _maxValue = 0;
        int _xValue = 0;
        double m_sumValue = 0;

        QVector<double> x,y;

        while (mSocket->hasPendingDatagrams()) {

            QHostAddress sender;
            quint16 senderPort;
            mSocket -> readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

            for (int i = 0; i < buffer.size(); ++i) {

                qint16 _data =  qint16(buffer.at(i));

                x.push_back(i);
                y.push_back(_data);
                m_sumValue += _data;
                if ( _data > _maxValue ) {
                    _maxValue = _data;
                    _xValue = i;
                }
            }
        }

        double _mediana = m_sumValue / double(y.size());

//        qDebug() << _mediana;

        ui -> customPlot -> addGraph();
        ui -> customPlot -> graph(0) -> setName("sample: " + QString::number(m_countSample));
        ui -> customPlot -> graph(0) -> addData(x,y);

        QVector<double> x_med(2) , y_med(2);
        x_med[0] = 0;
        y_med[0] = _mediana;
        x_med[1] = buffer.size();
        y_med[1] = _mediana;

        QPen _pen;
        _pen.setBrush((QColor(Qt::green)));
        _pen.setWidth(3);

        ui -> customPlot -> addGraph();
        ui -> customPlot -> graph(1) -> setName("Mediana: " + QString::number(_mediana) );
        ui -> customPlot -> graph(1) -> addData(x_med,y_med);
        ui -> customPlot -> graph(1) -> setPen(_pen);

        QVector<double> x_max(2) , y_max(2);
        x_max[0] = _xValue;
        y_max[0] = _maxValue;
        x_max[1] = _xValue;
        y_max[1] = _maxValue;

        _pen.setBrush((QColor(Qt::red)));
        _pen.setWidth(3);

        ui -> customPlot -> addGraph();
        ui -> customPlot -> graph(2) -> setName("MaxValue\nX: " + QString::number(_xValue) + "\nY: " + QString::number(_maxValue) );
        ui -> customPlot -> graph(2) -> addData(x_max,y_max);
        ui -> customPlot -> graph(2) -> setPen(_pen);
        ui -> customPlot -> graph(2) -> setLineStyle(QCPGraph::lsNone);
        ui -> customPlot -> graph(2) -> setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

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
