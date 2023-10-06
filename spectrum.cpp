#include "spectrum.h"

#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

Spectrum::Spectrum(QWidget *parent)
    : QWidget{parent}
    , m_chartView{new QChartView(new QChart)}
    , m_series{new QLineSeries}
{

    auto chart = m_chartView->chart();
    chart->addSeries(m_series);

    auto axisX = new QLogValueAxis;
    axisX->setBase(2.0);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Frequency (Hz)");

    auto axisY = new QValueAxis;
    axisY->setLabelFormat("%g");
    axisY->setTitleText("Audio (dB)");
    chart->addAxis(axisX, Qt::AlignBottom);
    m_series->attachAxis(axisX);
    chart->addAxis(axisY, Qt::AlignLeft);
    m_series->attachAxis(axisY);
    chart->legend()->hide();
    chart->setTitle("Simple Sine Wave");

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
}

bool Spectrum::initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize)
{
    m_chartView->resize(minimumGraphSize);
    setMinimumSize(minimumGraphSize);
    setMaximumSize(maximumGraphSize);
    return true;
}
