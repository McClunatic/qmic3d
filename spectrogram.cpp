#include "spectrogram.h"

#include <QVBoxLayout>

Spectrogram::Spectrogram(QWidget *parent)
    : QWidget{parent}
    , m_surface{new Q3DSurface}
{
}

bool Spectrogram::initialize(const QSize &minimumGraphSize, const QSize &maximumGraphSize)
{
    if (!m_surface->hasContext()) {
        free(m_surface);
        return false;
    }

    auto layout = new QVBoxLayout(this);
    m_container = QWidget::createWindowContainer(m_surface, this);

    m_surface->resize(minimumGraphSize);
    m_container->setMinimumSize(minimumGraphSize);
    m_container->setMaximumSize(maximumGraphSize);
    m_container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_container->setFocusPolicy(Qt::StrongFocus);
    layout->addWidget(m_container);

    auto camera = m_surface->scene()->activeCamera();
    camera->setCameraPreset(Q3DCamera::CameraPresetDirectlyAbove);
    camera->setZoomLevel(240.f);
    camera->setTarget({0.125f, 0.f, -1.0f});
    m_surface->setHorizontalAspectRatio(6.0f);
    m_surface->setFlipHorizontalGrid(true);
    m_surface->setOrthoProjection(true);
    m_surface->axisX()->setTitle("Frequency (Hz)");
    m_surface->axisX()->setTitleVisible(true);
    m_surface->axisZ()->setTitle("Time (s)");
    m_surface->axisZ()->setTitleVisible(true);

    m_proxy = new QSurfaceDataProxy;
    m_series = new QSurface3DSeries(m_proxy);
    m_series->setDrawMode(QSurface3DSeries::DrawSurface);

    QLinearGradient gr;
    gr.setColorAt(0., Qt::darkRed);
    gr.setColorAt(.33, Qt::red);
    gr.setColorAt(.67, Qt::yellow);
    gr.setColorAt(1.0, Qt::darkGreen);
    m_series->setBaseGradient(gr);
    m_series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

    m_surface->addSeries(m_series);
    return true;
}
