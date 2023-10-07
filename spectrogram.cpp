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

    Q3DCamera *camera = new Q3DCamera;
    camera->setCameraPreset(Q3DCamera::CameraPresetDirectlyAbove);
    m_surface->scene()->setActiveCamera(camera);
    m_surface->setOrthoProjection(true);

    QLinearGradient gr;
    gr.setColorAt(0., Qt::black);
    gr.setColorAt(.33, Qt::blue);
    gr.setColorAt(.67, Qt::red);
    gr.setColorAt(1.0, Qt::yellow);

    m_series = new QSurface3DSeries;
    m_series->setBaseGradient(gr);
    m_series->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    m_surface->addSeries(m_series);
    return true;
}
