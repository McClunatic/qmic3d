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
private:
    QChartView *m_chartView = nullptr;
    QLineSeries *m_series = nullptr;
};

#endif // SPECTRUM_H
