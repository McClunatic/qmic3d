#include "spectrum.h"

#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

#include <cmath>

Spectrum::Spectrum(QWidget *parent)
    : QWidget{parent}
    , m_chartView{new QChartView(new QChart)}
    , m_series{new QLineSeries}
{
    qreal x{ 0. };
    while (x < 2. * M_PI) {
        m_series->append(x, sin(x));
        x += 0.1;
    }
    m_series->append(2. * M_PI, sin(2. * M_PI));

    auto chart = m_chartView->chart();
    chart->addSeries(m_series);

    auto axisX = new QValueAxis;
    axisX->setRange(0, 2 * M_PI);
    axisX->setTitleText("Samples");

    auto axisY = new QValueAxis;
    axisY->setRange(-1, 1);
    axisY->setTitleText("Audio level");
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
