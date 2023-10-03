#include "spectrogram.h"

#include <QVBoxLayout>

Spectrogram::Spectrogram(QWidget *parent)
    : QWidget{parent}
    , m_surface{new Q3DSurface}
{
}

bool Spectrogram::initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize)
{
    if (!m_surface->hasContext())
        return false;

    auto layout = new QVBoxLayout(this);
    m_container = QWidget::createWindowContainer(m_surface, this);

    m_surface->resize(minimumGraphSize);
    m_container->setMinimumSize(minimumGraphSize);
    m_container->setMaximumSize(maximumGraphSize);
    m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_container->setFocusPolicy(Qt::StrongFocus);
    layout->addWidget(m_container);

    QSurfaceDataArray *data = new QSurfaceDataArray;
    QSurfaceDataRow *dataRow1 = new QSurfaceDataRow;
    QSurfaceDataRow *dataRow2 = new QSurfaceDataRow;

    *dataRow1 << QVector3D(0.0f, 0.1f, 0.5f) << QVector3D(1.0f, 0.5f, 0.5f);
    *dataRow2 << QVector3D(0.0f, 1.8f, 1.0f) << QVector3D(1.0f, 1.2f, 1.0f);
    *data << dataRow1 << dataRow2;

    m_series = new QSurface3DSeries;
    m_series->dataProxy()->resetArray(data);
    m_surface->addSeries(m_series);
    return true;
}
