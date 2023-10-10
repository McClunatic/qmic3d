#include "mainwindow.h"
#include "spectrum.h"
#include "scatterspectro.h"
#include "plotiodevice.h"

#include <QScreen>
#include <QAudioDevice>
#include <QAudioSource>
#include <QMediaDevices>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>

using namespace Qt::StringLiterals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create the central widget
    auto *widget = new QWidget;
    setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::initialize()
{
    const QSize screenSize = screen()->size();
    const QSize minimumGraphSize{screenSize.width() / 2, qRound(screenSize.height() / 3.5)};

    // Add audio input device
    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
        QMessageBox::warning(nullptr, u"MP3D"_s,
                             u"There is no audio input device available."_s);
        return false;
    }

    // Create spectrum 2D graph and spectrogram 3D graph
    auto *spectrum = new Spectrum(this);
    auto *spectrogram = new ScatterSpectro(this);
    if (!spectrum->initialize(minimumGraphSize, screenSize)
        || !spectrogram->initialize(minimumGraphSize, screenSize))
    {
        QMessageBox::warning(nullptr, u"MP3D"_s,
                             u"Couldn't initialize the OpenGL context."_s);
        delete spectrum;
        delete spectrogram;
        return false;
    }

    // Add spectrum 2D graph to layout
    auto *layout = new QVBoxLayout;
    layout->addWidget(spectrum);
    layout->addWidget(spectrogram);
    centralWidget()->setLayout(layout);

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(PlotIODevice::sampleRate);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleFormat(QAudioFormat::Float);
    auto source = new QAudioSource(inputDevice, formatAudio);

    auto device = new PlotIODevice(spectrum->series(), spectrogram->chart(), this);
    device->open(QIODevice::WriteOnly);

    source->start(device);

    return true;
}

