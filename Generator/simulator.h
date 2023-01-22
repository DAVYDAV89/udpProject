#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QUdpSocket>
#include <QTimer>
#include <QRandomGenerator>

namespace dt::widgets {

class Simulator : public QWidget
{
    Q_OBJECT

public:
    Simulator(QWidget *parent = nullptr);

private:
    QTextEdit *m_textEdit{nullptr};
    QPushButton *m_startBtn{nullptr};
    QPushButton *m_resetBtn{nullptr};
    QSlider *m_slider{nullptr};
    QLabel *m_label{nullptr};
    QUdpSocket *m_udpSocket{nullptr};
    QTimer *m_timer{nullptr};

    bool m_isStarted{false};
    QByteArray m_dump;
    int m_datagrams{0};
    QRandomGenerator m_rand;

    void generateDump();

    static constexpr uint16_t DEST_PORT{10002};
    static constexpr uint16_t SRC_PORT{10001};

private slots:
    void onStartBtn();
    void onResetBtn();
    void onTimeout();
    void onSliderValueChanged();
    void onSliderReleased();

};

}
