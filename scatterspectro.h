#ifndef SCATTERSPECTRO_H
#define SCATTERSPECTRO_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

class ScatterSpectro : public QWidget
{
    Q_OBJECT
public:
    explicit ScatterSpectro(QWidget *parent = nullptr);

    bool initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize);
    QChart* chart() { return m_chartView->chart(); }
private:
    QChartView *m_chartView = nullptr;
};

#endif // SCATTERSPECTRO_H
