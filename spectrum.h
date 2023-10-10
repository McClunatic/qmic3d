#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

class Spectrum : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrum(QWidget *parent = nullptr);

    bool initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize);
    QLineSeries* series() { return m_series; }
private:
    QLineSeries *m_series = nullptr;
    QChartView *m_chartView = nullptr;
};

#endif // SPECTRUM_H
