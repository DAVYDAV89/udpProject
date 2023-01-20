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

    ui -> color -> addItem(("Синий"));
    ui -> color -> addItem(("Красный"));
    ui -> color -> addItem(("Зеленый"));

    ui -> customPlot -> setInteraction(QCP::iRangeZoom, true);
    ui -> customPlot -> setInteraction(QCP::iRangeDrag, true);

    ui -> customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui -> customPlot->axisRect()->setRangeZoom(Qt::Horizontal);

    ui -> customPlot -> xAxis -> setRange(0, 1000);
    ui -> customPlot -> yAxis -> setRange(0, 20);

    ui -> customPlot -> addGraph();

    ui -> customPlot -> addGraph();//Max Value
    ui -> customPlot -> graph(1) -> setPen(QPen(Qt::red));


    connect(ui -> customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
             this, SLOT(slotRangeChanged(QCPRange)));

    connect(mSocket, &QUdpSocket::readyRead, [&]() {

        while (mSocket->hasPendingDatagrams()) {

            QByteArray buffer;
            buffer.resize(mSocket->pendingDatagramSize());

//            if (buffer.size() == 8192) {

                QHostAddress sender;
                quint16 senderPort;
                mSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

                QTextStream T(buffer.data());
                auto text = T.readAll();

                x.push_back(_x);
                y.push_back(text.toInt());

                m_mediana += text.toInt();

                _x = _x + (ui -> spinBox -> text().toDouble());

                if ( _x > ui -> customPlot -> xAxis -> range().upper ) {
                    ui -> customPlot -> xAxis -> setRange(0, _x);
                    ui -> xUpDown -> setValue(_x);
                }

                if ( text.toInt() > ui -> customPlot -> yAxis -> range().upper ) {
                    ui -> maxValue -> setValue(text.toInt());
                    ui -> customPlot -> yAxis -> setRange(0, text.toInt());
                }

                ui -> customPlot -> graph(0) -> addData(x,y);
                ui -> customPlot -> replot();

                ui -> mediana -> setValue(m_mediana / y.size());
//            }

//            qDebug() << buffer.data();
        }

    });
}

Widget::~Widget()
{
    delete mSocket;
    delete ui;
}

void Widget::on_color_activated(int _index)
{
    switch (_index) {
    case 0:
        ui -> customPlot -> graph(0) -> setPen(QPen(QColor(Qt::blue)));
        break;
    case 1:
        ui -> customPlot -> graph(0) -> setPen(QPen(QColor(Qt::red)));
        break;
    case 2:
        ui -> customPlot -> graph(0) -> setPen(QPen(QColor(Qt::green)));
        break;
    }

    on_width_valueChanged(ui -> width -> text().toInt());
}

void Widget::on_width_valueChanged(int _arg1)
{
    QPen _pen = ui -> customPlot -> graph(0) -> pen();
    ui -> customPlot -> graph(0) -> setPen(_pen);

    _pen.setWidth(_arg1);
    ui -> customPlot -> graph(0) -> setPen(_pen);
}

void Widget::slotRangeChanged(QCPRange _range)
{
//    qDebug() << ui -> customPlot -> xAxis -> range().upper;
    if (ui -> customPlot -> xAxis -> range().lower < 0)
        ui -> customPlot -> xAxis -> setRange(0, ui -> customPlot -> xAxis -> range().upper);

    ui -> xUpDown -> setValue(ui -> customPlot -> xAxis -> range().upper);
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
