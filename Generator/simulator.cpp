#include "simulator.h"
#include <QGridLayout>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <qmath.h>

namespace dt::widgets {

Simulator::Simulator(QWidget *parent)
    : QWidget(parent)
{
    m_textEdit = new QTextEdit{this};
    QPalette p;
    p.setColor(QPalette::Base, QColor(50, 50, 100));
    p.setColor(QPalette::Text, Qt::white);
    m_textEdit->setPalette(p);
    m_textEdit->setCurrentFont(QFont("Consolas", 10));
    m_textEdit->setReadOnly(true);
    m_textEdit->setText(QString::asprintf("Sent %3d datagrams" , m_datagrams));
    m_startBtn = new QPushButton{"Start", this};
    m_resetBtn = new QPushButton{"Reset", this};
    m_slider = new QSlider{this};
    m_slider->setRange(5, 1000);
    m_slider->setValue(1000);
    m_slider->setOrientation(Qt::Horizontal);
    m_label = new QLabel{"Period:   50 ms ", this};
    p.setColor(QPalette::WindowText, Qt::white);
    m_label->setPalette(p);

    m_udpSocket = new QUdpSocket{this};
    m_udpSocket->bind(SRC_PORT, QUdpSocket::ShareAddress);
    m_timer = new QTimer{this};

    connect(m_startBtn, &QPushButton::clicked, this, &Simulator::onStartBtn);
    connect(m_resetBtn, &QPushButton::clicked, this, &Simulator::onResetBtn);
    connect(m_timer, &QTimer::timeout, this, &Simulator::onTimeout);
    connect(m_slider, &QSlider::valueChanged, this, &Simulator::onSliderValueChanged);
    connect(m_slider, &QSlider::sliderReleased, this, &Simulator::onSliderReleased);

    auto mainLayout = new QGridLayout;
    mainLayout->addWidget(m_textEdit, 0, 0, 10, 10);
    mainLayout->addWidget(m_label, 10, 0, 1, 2);
    mainLayout->addWidget(m_slider, 10, 2, 1, 4);
    mainLayout->addWidget(m_startBtn, 10, 6, 1, 2);
    mainLayout->addWidget(m_resetBtn, 10, 8, 1, 2);
    setLayout(mainLayout);

    p.setColor(QPalette::Window, QColor(25, 25, 50));
    setAutoFillBackground(true);
    setPalette(p);
    setWindowTitle(tr("Simulator"));
    resize(400, 100);

    setWindowIcon(QIcon(":/icons/digitizer.png"));

//    QFile fin{"d:/WORK/_MegaFon/_Matlab/1.dat"};
//    if (!fin.open(QIODevice::ReadOnly)) {
//        qDebug() << fin.errorString();
//        return;
//    }
//    m_dump = fin.readAll();
//    fin.close();

    m_dump.reserve(8192);
}

void Simulator::generateDump()
{
    auto cnt = m_datagrams % 20;
    if (cnt >= 10) {
        cnt = 20 - cnt;
    }
    auto tau1 = 50.0 + 0.02 * double(cnt);
    auto tau2 = 500.0 - 0.02 * double(cnt);
    auto cent = 1000 + cnt * 10;
    auto len = 4096;
    auto k_noise = 0.1;
//    k_noise *= (cnt + 1);

    m_dump.resize(0);
    QDataStream ds{&m_dump, QIODevice::WriteOnly};

    for (auto i = 0; i < len; i++) {
        qreal sample = 0.0;
        if (i < cent) {
            sample = exp(-fabs(double(i - cent)) / tau1) + k_noise * (0.5 - m_rand.generateDouble());
        } else {
            sample = exp(-fabs(double(i - cent)) / tau2) + k_noise * (0.5 - m_rand.generateDouble());
        }
        sample *= 4096;

        ds << qint16(sample);
    }
//    qDebug() << m_dump.size();
}

void Simulator::onStartBtn()
{
    m_isStarted = !m_isStarted;
    if (m_isStarted) {
        m_timer->start(m_slider->value());
        m_startBtn->setText("Stop");
    } else {
        m_timer->stop();
        m_startBtn->setText("Start");
    }
}

void Simulator::onResetBtn()
{
    m_datagrams = 0;
    m_textEdit->setText(QString::asprintf("Sent %3d datagrams" , m_datagrams));
}

void Simulator::onTimeout()
{
    generateDump();
    m_udpSocket->writeDatagram(m_dump, QHostAddress::LocalHost, DEST_PORT);
    m_textEdit->setText(QString::asprintf("Sent %3d datagrams" , ++m_datagrams));
}

void Simulator::onSliderValueChanged()
{
    m_label->setText(QString::asprintf("Period: %4d ms ", m_slider->value()));
}

void Simulator::onSliderReleased()
{
    if (m_isStarted) {
        m_timer->stop();
        m_timer->start(m_slider->value());
    }
}


}
