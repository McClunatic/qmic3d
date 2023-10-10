#include "plotiodevice.h"

#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QLineSeries>

#include "fftw3.h"

#include <algorithm>
#include <cmath>
#include <complex>

static QLinearGradient vlagGradient()
{
    QList<QList<double>> vlag {
        { 0.13850039, 0.41331206, 0.74052025},
        { 0.15077609, 0.41762684, 0.73970427},
        { 0.16235219, 0.4219191 , 0.7389667 },
        { 0.1733322 , 0.42619024, 0.73832537},
        { 0.18382538, 0.43044226, 0.73776764},
        { 0.19394034, 0.4346772 , 0.73725867},
        { 0.20367115, 0.43889576, 0.73685314},
        { 0.21313625, 0.44310003, 0.73648045},
        { 0.22231173, 0.44729079, 0.73619681},
        { 0.23125148, 0.45146945, 0.73597803},
        { 0.23998101, 0.45563715, 0.7358223 },
        { 0.24853358, 0.45979489, 0.73571524},
        { 0.25691416, 0.4639437 , 0.73566943},
        { 0.26513894, 0.46808455, 0.73568319},
        { 0.27322194, 0.47221835, 0.73575497},
        { 0.28117543, 0.47634598, 0.73588332},
        { 0.28901021, 0.48046826, 0.73606686},
        { 0.2967358 , 0.48458597, 0.73630433},
        { 0.30436071, 0.48869986, 0.73659451},
        { 0.3118955 , 0.49281055, 0.73693255},
        { 0.31935389, 0.49691847, 0.73730851},
        { 0.32672701, 0.5010247 , 0.73774013},
        { 0.33402607, 0.50512971, 0.73821941},
        { 0.34125337, 0.50923419, 0.73874905},
        { 0.34840921, 0.51333892, 0.73933402},
        { 0.35551826, 0.51744353, 0.73994642},
        { 0.3625676 , 0.52154929, 0.74060763},
        { 0.36956356, 0.52565656, 0.74131327},
        { 0.37649902, 0.52976642, 0.74207698},
        { 0.38340273, 0.53387791, 0.74286286},
        { 0.39025859, 0.53799253, 0.7436962 },
        { 0.39706821, 0.54211081, 0.744578  },
        { 0.40384046, 0.54623277, 0.74549872},
        { 0.41058241, 0.55035849, 0.74645094},
        { 0.41728385, 0.55448919, 0.74745174},
        { 0.42395178, 0.55862494, 0.74849357},
        { 0.4305964 , 0.56276546, 0.74956387},
        { 0.4372044 , 0.56691228, 0.75068412},
        { 0.4437909 , 0.57106468, 0.75183427},
        { 0.45035117, 0.5752235 , 0.75302312},
        { 0.45687824, 0.57938983, 0.75426297},
        { 0.46339713, 0.58356191, 0.75551816},
        { 0.46988778, 0.58774195, 0.75682037},
        { 0.47635605, 0.59192986, 0.75816245},
        { 0.48281101, 0.5961252 , 0.75953212},
        { 0.4892374 , 0.60032986, 0.76095418},
        { 0.49566225, 0.60454154, 0.76238852},
        { 0.50206137, 0.60876307, 0.76387371},
        { 0.50845128, 0.61299312, 0.76538551},
        { 0.5148258 , 0.61723272, 0.76693475},
        { 0.52118385, 0.62148236, 0.76852436},
        { 0.52753571, 0.62574126, 0.77013939},
        { 0.53386831, 0.63001125, 0.77180152},
        { 0.54020159, 0.63429038, 0.7734803 },
        { 0.54651272, 0.63858165, 0.77521306},
        { 0.55282975, 0.64288207, 0.77695608},
        { 0.55912585, 0.64719519, 0.77875327},
        { 0.56542599, 0.65151828, 0.78056551},
        { 0.57170924, 0.65585426, 0.78242747},
        { 0.57799572, 0.6602009 , 0.78430751},
        { 0.58426817, 0.66456073, 0.78623458},
        { 0.590544  , 0.66893178, 0.78818117},
        { 0.59680758, 0.67331643, 0.79017369},
        { 0.60307553, 0.67771273, 0.79218572},
        { 0.60934065, 0.68212194, 0.79422987},
        { 0.61559495, 0.68654548, 0.7963202 },
        { 0.62185554, 0.69098125, 0.79842918},
        { 0.62810662, 0.69543176, 0.80058381},
        { 0.63436425, 0.69989499, 0.80275812},
        { 0.64061445, 0.70437326, 0.80497621},
        { 0.6468706 , 0.70886488, 0.80721641},
        { 0.65312213, 0.7133717 , 0.80949719},
        { 0.65937818, 0.71789261, 0.81180392},
        { 0.66563334, 0.72242871, 0.81414642},
        { 0.67189155, 0.72697967, 0.81651872},
        { 0.67815314, 0.73154569, 0.81892097},
        { 0.68441395, 0.73612771, 0.82136094},
        { 0.69068321, 0.74072452, 0.82382353},
        { 0.69694776, 0.7453385 , 0.82633199},
        { 0.70322431, 0.74996721, 0.8288583 },
        { 0.70949595, 0.75461368, 0.83143221},
        { 0.7157774 , 0.75927574, 0.83402904},
        { 0.72206299, 0.76395461, 0.83665922},
        { 0.72835227, 0.76865061, 0.8393242 },
        { 0.73465238, 0.7733628 , 0.84201224},
        { 0.74094862, 0.77809393, 0.84474951},
        { 0.74725683, 0.78284158, 0.84750915},
        { 0.75357103, 0.78760701, 0.85030217},
        { 0.75988961, 0.79239077, 0.85313207},
        { 0.76621987, 0.79719185, 0.85598668},
        { 0.77255045, 0.8020125 , 0.85888658},
        { 0.77889241, 0.80685102, 0.86181298},
        { 0.78524572, 0.81170768, 0.86476656},
        { 0.79159841, 0.81658489, 0.86776906},
        { 0.79796459, 0.82148036, 0.8707962 },
        { 0.80434168, 0.82639479, 0.87385315},
        { 0.8107221 , 0.83132983, 0.87695392},
        { 0.81711301, 0.8362844 , 0.88008641},
        { 0.82351479, 0.84125863, 0.88325045},
        { 0.82992772, 0.84625263, 0.88644594},
        { 0.83634359, 0.85126806, 0.8896878 },
        { 0.84277295, 0.85630293, 0.89295721},
        { 0.84921192, 0.86135782, 0.89626076},
        { 0.85566206, 0.866432  , 0.89959467},
        { 0.86211514, 0.87152627, 0.90297183},
        { 0.86857483, 0.87663856, 0.90638248},
        { 0.87504231, 0.88176648, 0.90981938},
        { 0.88151194, 0.88690782, 0.91328493},
        { 0.88797938, 0.89205857, 0.91677544},
        { 0.89443865, 0.89721298, 0.9202854 },
        { 0.90088204, 0.90236294, 0.92380601},
        { 0.90729768, 0.90749778, 0.92732797},
        { 0.91367037, 0.91260329, 0.93083814},
        { 0.91998105, 0.91766106, 0.93431861},
        { 0.92620596, 0.92264789, 0.93774647},
        { 0.93231683, 0.9275351 , 0.94109192},
        { 0.93827772, 0.9322888 , 0.94432312},
        { 0.94404755, 0.93686925, 0.94740137},
        { 0.94958284, 0.94123072, 0.95027696},
        { 0.95482682, 0.9453245 , 0.95291103},
        { 0.9597248 , 0.94909728, 0.95525103},
        { 0.96422552, 0.95249273, 0.95723271},
        { 0.96826161, 0.95545812, 0.95882188},
        { 0.97178458, 0.95793984, 0.95995705},
        { 0.97474105, 0.95989142, 0.96059997},
        { 0.97708604, 0.96127366, 0.96071853},
        { 0.97877855, 0.96205832, 0.96030095},
        { 0.97978484, 0.96222949, 0.95935496},
        { 0.9805997 , 0.96155216, 0.95813083},
        { 0.98152619, 0.95993719, 0.95639322},
        { 0.9819726 , 0.95766608, 0.95399269},
        { 0.98191855, 0.9547873 , 0.95098107},
        { 0.98138514, 0.95134771, 0.94740644},
        { 0.98040845, 0.94739906, 0.94332125},
        { 0.97902107, 0.94300131, 0.93878672},
        { 0.97729348, 0.93820409, 0.93385135},
        { 0.9752533 , 0.933073  , 0.92858252},
        { 0.97297834, 0.92765261, 0.92302309},
        { 0.97049104, 0.92200317, 0.91723505},
        { 0.96784372, 0.91616744, 0.91126063},
        { 0.96507281, 0.91018664, 0.90514124},
        { 0.96222034, 0.90409203, 0.89890756},
        { 0.9593079 , 0.89791478, 0.89259122},
        { 0.95635626, 0.89167908, 0.88621654},
        { 0.95338303, 0.88540373, 0.87980238},
        { 0.95040174, 0.87910333, 0.87336339},
        { 0.94742246, 0.87278899, 0.86691076},
        { 0.94445249, 0.86646893, 0.86045277},
        { 0.94150476, 0.86014606, 0.85399191},
        { 0.93857394, 0.85382798, 0.84753642},
        { 0.93566206, 0.84751766, 0.84108935},
        { 0.93277194, 0.8412164 , 0.83465197},
        { 0.92990106, 0.83492672, 0.82822708},
        { 0.92704736, 0.82865028, 0.82181656},
        { 0.92422703, 0.82238092, 0.81541333},
        { 0.92142581, 0.81612448, 0.80902415},
        { 0.91864501, 0.80988032, 0.80264838},
        { 0.91587578, 0.80365187, 0.79629001},
        { 0.9131367 , 0.79743115, 0.78994   },
        { 0.91041602, 0.79122265, 0.78360361},
        { 0.90771071, 0.78502727, 0.77728196},
        { 0.90501581, 0.77884674, 0.7709771 },
        { 0.90235365, 0.77267117, 0.76467793},
        { 0.8997019 , 0.76650962, 0.75839484},
        { 0.89705346, 0.76036481, 0.752131  },
        { 0.89444021, 0.75422253, 0.74587047},
        { 0.89183355, 0.74809474, 0.73962689},
        { 0.88923216, 0.74198168, 0.73340061},
        { 0.88665892, 0.73587283, 0.72717995},
        { 0.88408839, 0.72977904, 0.72097718},
        { 0.88153537, 0.72369332, 0.71478461},
        { 0.87899389, 0.7176179 , 0.70860487},
        { 0.87645157, 0.71155805, 0.7024439 },
        { 0.8739399 , 0.70549893, 0.6962854 },
        { 0.87142626, 0.6994551 , 0.69014561},
        { 0.8689268 , 0.69341868, 0.68401597},
        { 0.86643562, 0.687392  , 0.67789917},
        { 0.86394434, 0.68137863, 0.67179927},
        { 0.86147586, 0.67536728, 0.665704  },
        { 0.85899928, 0.66937226, 0.6596292 },
        { 0.85654668, 0.66337773, 0.6535577 },
        { 0.85408818, 0.65739772, 0.64750494},
        { 0.85164413, 0.65142189, 0.64145983},
        { 0.84920091, 0.6454565 , 0.63542932},
        { 0.84676427, 0.63949827, 0.62941   },
        { 0.84433231, 0.63354773, 0.62340261},
        { 0.84190106, 0.62760645, 0.61740899},
        { 0.83947935, 0.62166951, 0.61142404},
        { 0.8370538 , 0.61574332, 0.60545478},
        { 0.83463975, 0.60981951, 0.59949247},
        { 0.83221877, 0.60390724, 0.593547  },
        { 0.82980985, 0.59799607, 0.58760751},
        { 0.82740268, 0.59209095, 0.58167944},
        { 0.82498638, 0.5861973 , 0.57576866},
        { 0.82258181, 0.5803034 , 0.56986307},
        { 0.82016611, 0.57442123, 0.56397539},
        { 0.81776305, 0.56853725, 0.55809173},
        { 0.81534551, 0.56266602, 0.55222741},
        { 0.81294293, 0.55679056, 0.5463651 },
        { 0.81052113, 0.55092973, 0.54052443},
        { 0.80811509, 0.54506305, 0.53468464},
        { 0.80568952, 0.53921036, 0.52886622},
        { 0.80327506, 0.53335335, 0.52305077},
        { 0.80084727, 0.52750583, 0.51725256},
        { 0.79842217, 0.5216578 , 0.51146173},
        { 0.79599382, 0.51581223, 0.50568155},
        { 0.79355781, 0.50997127, 0.49991444},
        { 0.79112596, 0.50412707, 0.49415289},
        { 0.78867442, 0.49829386, 0.48841129},
        { 0.7862306 , 0.49245398, 0.48267247},
        { 0.7837687 , 0.48662309, 0.47695216},
        { 0.78130809, 0.4807883 , 0.47123805},
        { 0.77884467, 0.47495151, 0.46553236},
        { 0.77636283, 0.46912235, 0.45984473},
        { 0.77388383, 0.46328617, 0.45416141},
        { 0.77138912, 0.45745466, 0.44849398},
        { 0.76888874, 0.45162042, 0.44283573},
        { 0.76638802, 0.44577901, 0.43718292},
        { 0.76386116, 0.43994762, 0.43155211},
        { 0.76133542, 0.43410655, 0.42592523},
        { 0.75880631, 0.42825801, 0.42030488},
        { 0.75624913, 0.42241905, 0.41470727},
        { 0.7536919 , 0.41656866, 0.40911347},
        { 0.75112748, 0.41071104, 0.40352792},
        { 0.74854331, 0.40485474, 0.3979589 },
        { 0.74594723, 0.39899309, 0.39240088},
        { 0.74334332, 0.39312199, 0.38685075},
        { 0.74073277, 0.38723941, 0.3813074 },
        { 0.73809409, 0.38136133, 0.37578553},
        { 0.73544692, 0.37547129, 0.37027123},
        { 0.73278943, 0.36956954, 0.36476549},
        { 0.73011829, 0.36365761, 0.35927038},
        { 0.72743485, 0.35773314, 0.35378465},
        { 0.72472722, 0.35180504, 0.34831662},
        { 0.72200473, 0.34586421, 0.34285937},
        { 0.71927052, 0.33990649, 0.33741033},
        { 0.71652049, 0.33393396, 0.33197219},
        { 0.71375362, 0.32794602, 0.32654545},
        { 0.71096951, 0.32194148, 0.32113016},
        { 0.70816772, 0.31591904, 0.31572637},
        { 0.70534784, 0.30987734, 0.31033414},
        { 0.70250944, 0.30381489, 0.30495353},
        { 0.69965211, 0.2977301 , 0.2995846 },
        { 0.6967754 , 0.29162126, 0.29422741},
        { 0.69388446, 0.28548074, 0.28887769},
        { 0.69097561, 0.2793096 , 0.28353795},
        { 0.68803513, 0.27311993, 0.27821876},
        { 0.6850794 , 0.26689144, 0.27290694},
        { 0.682108  , 0.26062114, 0.26760246},
        { 0.67911013, 0.2543177 , 0.26231367},
        { 0.67609393, 0.24796818, 0.25703372},
        { 0.67305921, 0.24156846, 0.25176238},
        { 0.67000176, 0.23511902, 0.24650278},
        { 0.66693423, 0.22859879, 0.24124404},
        { 0.6638441 , 0.22201742, 0.2359961 },
        { 0.66080672, 0.21526712, 0.23069468}
    };
    QLinearGradient gradient(0.f, 0.f, 0.f, 1.f);
    int stop = 0;
    for (const QList<double>& row: vlag) {
        auto rgb = qRgb(static_cast<int>(256. * row[0]),
                        static_cast<int>(256. * row[1]),
                        static_cast<int>(256. * row[2]));
        gradient.setColorAt(stop / 255., rgb);
        ++stop;
    }
    return gradient;
}

PlotIODevice::PlotIODevice(QLineSeries *lineSeries,
                           QChart *specChart,
                           QObject *parent)
    : QIODevice{parent}
    , m_lineSeries{lineSeries}
    , m_specChart{specChart}
{
    auto axes = m_lineSeries->attachedAxes();
    auto xAxis = dynamic_cast<QLogValueAxis*>(axes[0]);
    auto yAxis = dynamic_cast<QValueAxis*>(axes[1]);
    xAxis->setRange(1., sampleRate / 2.);  // 0 to Nyquist frequency, Hz
    yAxis->setRange(0., 60.);  // 0 to 60, dB

    m_specData.resize(101);
    for (int i = 100; i >= 0; --i) {
        float t = 0.f - (float(i) * n_fft) / sampleRate;
        for (int j = 0; j < (n_fft / 2 + 1); ++j) {
            auto x = 1e-10f + (float(j) * sampleRate) / n_fft;
            m_specData[100-i].append(QVector3D(x, 0.f, t));
        }
    }

    QObject::connect(m_specChart, &QChart::plotAreaChanged, this, &PlotIODevice::plotAreaChanged);
}

qint64 PlotIODevice::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return -1;
}

qint64 PlotIODevice::writeData(const char *data, qint64 maxSize)
{
    using namespace std::complex_literals;

    const float* fdata = reinterpret_cast<const float*>(data);
    const qint64 fsize = maxSize * sizeof(char) / sizeof(float);

    if (m_buffer.capacity() == 0) {
        m_buffer.reserve(n_fft - 1);
        m_in.reserve(n_fft);
        m_out.resize(n_fft / 2 + 1);
    }

    // If there's not enough data for an FFT, move on
    if (fsize + m_buffer.size() < n_fft) {
        std::copy(fdata, fdata + fsize, std::back_inserter(m_buffer));
        return maxSize;
    }

    // Otherwise do an FFT and save the remainder
    int remainder = n_fft - m_buffer.size();

    // Put all the buffer in m_in, then full the rest from data
    std::copy(m_buffer.begin(), m_buffer.end(), m_in.begin());
    std::copy(fdata, fdata + remainder, std::back_inserter(m_in));

    // Put the rest of data in the buffer for later
    m_buffer.clear();
    std::copy(fdata + remainder, fdata + fsize, m_buffer.begin());

    // Perform 1D r2c FFT
    fftwf_plan p = fftwf_plan_dft_r2c_1d(
        n_fft,
        m_in.data(),
        reinterpret_cast<fftwf_complex*>(m_out.data()),
        FFTW_ESTIMATE);
    fftwf_execute(p);
    fftwf_destroy_plan(p);

    // Convert m_out to decibels
    for (std::complex<float> &c : m_out) {
        c = std::norm(c);
        c = 10. * std::log10(std::max(1.e-10f, c.real()));
    }

    // Set t (time) for spectrogram
    qreal t{m_specData.isEmpty()
                ? m_specData.last().first().x() + qreal(n_fft) / sampleRate
                : 0.};

    // Create new points for spectrum and spectrogram
    QList<QPointF> line;
    QList<QVector3D> spec;
    QList<qreal> logx;
    for (int j = 0; j < (n_fft / 2 + 1); ++j) {
        auto x = std::max(1e-10f, (float(j)* sampleRate) / n_fft);
        auto y = m_out[j].real();
        line.append(QPointF(x, y));
        spec.append(QVector3D(x, y, t));
        logx.append(std::log2(x));
    }
    m_lineSeries->replace(line);

    // Update m_specData
    m_specData.remove(0);
    m_specData.append(spec);

    // Create a new image
    auto imgSize = m_specChart->plotAreaBackgroundBrush().textureImage().size();
    auto imgFormat = m_specChart->plotAreaBackgroundBrush().textureImage().format();
    QImage image(imgSize, imgFormat);
    auto fillColor{vlagGradient().stops().constFirst().second};
    image.fill(fillColor);

    // Prepare the interp data for t (y)
    QList<QPair<int, qreal>> t_interpData;
    t_interpData.reserve(image.height());
    for (int pt = 0; pt < image.height(); ++pt) {
        auto tfrac = static_cast<qreal>(pt) / image.height();
        auto tidx = tfrac * m_specData.size();
        t_interpData.append({int(tidx), tidx - int(tidx)});
    }

    // Prepare the interp data for x
    auto xmin = *std::min_element(logx.begin(), logx.end());
    auto xmax = *std::max_element(logx.begin(), logx.end());

    QList<QPair<int, qreal>> x_interpData;
    x_interpData.reserve(image.width());
    for (int px = 0; px < image.width(); ++px) {
        auto xfrac = static_cast<qreal>(px) / image.width();
        xfrac = xmin + (xmax - xmin) * xfrac;
        int xidx = std::distance(logx.begin(),
                                 std::lower_bound(logx.begin(), logx.end(), xfrac));
        xidx = std::max(0, xidx - 1);
        auto xtarget = (xfrac - logx[xidx]) / (logx[xidx+1] - logx[xidx]);
        x_interpData.append({xidx, xtarget});
    }

    for (int pt = 0; pt < image.height(); ++pt) {
        auto [t_idx, t_tgt] = t_interpData[pt];
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(pt));
        for (int x = 0; x < image.width(); ++x) {
            auto [x_idx, x_tgt] = x_interpData[x];
            QRgb &rgb = line[x];

            auto botLeft = m_specData.at(t_idx).at(x_idx).y();
            auto botRight = m_specData.at(t_idx).at(x_idx+1).y();
            auto topLeft = m_specData.at(t_idx+1).at(x_idx).y();
            auto topRight = m_specData.at(t_idx+1).at(x_idx+1).y();
            auto bot = std::lerp(botLeft, botRight, x_tgt);
            auto top = std::lerp(topLeft, topRight, x_tgt);
            auto ans = std::lerp(bot, top, t_tgt);
            int colorIdx = static_cast<int>(256. * ans / 60.);
            auto color = vlagGradient().stops().at(colorIdx).second;
            rgb = color.rgb();
            break;
        }
    }

    auto brush{m_specChart->plotAreaBackgroundBrush()};
    brush.setTextureImage(m_image);
    m_specChart->setPlotAreaBackgroundBrush(brush);

//    auto proxy = m_surfaceSeries->dataProxy();
//    auto t = proxy->array()->last()->last().z();
//    t += float(n_fft) / sampleRate;

//    QList<QPointF> line;
//    auto row = new QSurfaceDataRow;
//    for (int j = 0; j < (n_fft / 2 + 1); ++j) {
//        auto x = std::max(1e-10f, (float(j)* sampleRate) / n_fft);
//        auto y = m_out[j].real();
//        line.append(QPointF(x, y));
//        *row << QVector3D(x, y, t);
//    }
//    m_lineSeries->replace(line);
//    proxy->removeRows(0, 1);
//    proxy->addRow(row);

    qDebug() << "finishing";
    return maxSize;
}

void PlotIODevice::plotAreaChanged(const QRectF& plotArea)
{
    const auto area{plotArea.size().toSize()};
    auto brush{m_specChart->plotAreaBackgroundBrush()};
    if (brush.textureImage().isNull()) {
        auto fillColor{vlagGradient().stops().constFirst().second};
        m_image = QImage(area, QImage::Format_RGB32);
        m_image.fill(fillColor);
    } else {
        m_image = m_image.scaled(area);
    }
    brush.setTextureImage(m_image);
    m_specChart->setPlotAreaBackgroundBrush(brush);
}
