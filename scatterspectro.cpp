#include "scatterspectro.h"

#include <QVBoxLayout>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>

ScatterSpectro::ScatterSpectro(QWidget *parent)
    : QWidget{parent}
    , m_chartView{new QChartView(new QChart)}
{
    auto chart = m_chartView->chart();

    auto axisX = new QLogValueAxis;
    auto axisY = new QValueAxis;
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    chart->legend()->hide();
    chart->setPlotAreaBackgroundVisible(true);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_chartView);
}

bool ScatterSpectro::initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize)
{
    m_chartView->resize(minimumGraphSize);
    setMinimumSize(minimumGraphSize);
    setMaximumSize(maximumGraphSize);
    return true;
}
