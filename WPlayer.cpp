#include "WPlayer.h"

#include "WPlayerControls.h"
#include "WPlaylistModel.h"
#include "WHistogramWidget.h"
#include "WVideoWidget.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>

static bool urlIsPlaylist(const QUrl& url)
{
    if (!url.isLocalFile())
    {
        return false;
    }

    const QFileInfo fileInfo(url.toLocalFile());

    return fileInfo.exists() && (!fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive));
}

WPlayer::WPlayer(QWidget *parent)
    : QWidget            (parent)
    , m_player           (nullptr)
    , m_playlist         (nullptr)
    , m_videoWidget      (nullptr)
    , m_coverLabel       (nullptr)
    , m_slider           (nullptr)
    , m_durationLabel    (nullptr)
    , m_fullScreenButton (nullptr)
    , m_colorButton      (nullptr)
    , m_colorDialog      (nullptr)
    , m_statusLabel      (nullptr)
    , m_statusBar        (nullptr)
    , m_histogramLabel   (nullptr)
    , m_videoHistogram   (nullptr)
    , m_audioHistogram   (nullptr)
    , m_videoProbe       (nullptr)
    , m_audioProbe       (nullptr)
    , m_playlistModel    (nullptr)
    , m_playlistView     (nullptr)
    , m_duration         (0)
{
    m_player = new QMediaPlayer(this);
    m_player->setAudioRole(QAudio::VideoRole);

#ifdef QT_DEBUG
    qDebug() << "Supported audio roles:";

    for (QAudio::Role role : m_player->supportedAudioRoles())
    {
        qDebug() << "    " << role;
    }
#endif

    connect(m_player, QOverload<>::of(&QMediaPlayer::metaDataChanged), this, &WPlayer::onMetaDataChanged);
    connect(m_player, &QMediaPlayer::stateChanged, this, &WPlayer::onStateChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &WPlayer::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &WPlayer::onDurationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &WPlayer::onPositionChanged);
    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &WPlayer::onVideoAvailableChanged);
    connect(m_player, &QMediaPlayer::bufferStatusChanged, this, &WPlayer::onBufferStatusChanged);
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &WPlayer::displayErrorMessage);

    m_playlist = new QMediaPlaylist();

    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &WPlayer::onPlaylistPositionChanged);

    m_player->setPlaylist(m_playlist);

    m_videoWidget = new WVideoWidget(this);

    m_player->setVideoOutput(m_videoWidget);

    m_playlistModel = new WPlaylistModel(this);
    m_playlistModel->setPlaylist(m_playlist);

    m_playlistView = new QListView(this);
    m_playlistView->setModel(m_playlistModel);
    m_playlistView->setCurrentIndex(m_playlistModel->index(m_playlist->currentIndex(), 0));

    connect(m_playlistView, &QAbstractItemView::activated, this, &WPlayer::jump);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, m_player->duration() / 1000);

    connect(m_slider, &QSlider::sliderMoved, this, &WPlayer::seek);

    m_durationLabel = new QLabel(this);

    m_histogramLabel = new QLabel(this);
    m_histogramLabel->setText("Histogram:");

    m_videoHistogram = new WHistogramWidget(this);
    m_audioHistogram = new WHistogramWidget(this);

    QHBoxLayout* histogramLayout = new QHBoxLayout();

    histogramLayout->addWidget(m_histogramLabel);
    histogramLayout->addWidget(m_videoHistogram, 1);
    histogramLayout->addWidget(m_audioHistogram, 2);

    m_videoProbe = new QVideoProbe(this);
    m_videoProbe->setSource(m_player);

    connect(m_videoProbe, &QVideoProbe::videoFrameProbed, m_videoHistogram, &WHistogramWidget::processVideoFrame);

    m_audioProbe = new QAudioProbe(this);
    m_audioProbe->setSource(m_player);

    connect(m_audioProbe, &QAudioProbe::audioBufferProbed, m_audioHistogram, &WHistogramWidget::processAudioBuffer);

    QPushButton* openButton = new QPushButton(tr("Open"), this);

    connect(openButton, &QPushButton::clicked, this, &WPlayer::open);

    WPlayerControls* controls = new WPlayerControls(this);

    controls->setState(m_player->state());
    controls->setVolume(m_player->volume());
    controls->setMuted(controls->isMuted());

    connect(controls, &WPlayerControls::play, m_player, &QMediaPlayer::play);
    connect(controls, &WPlayerControls::pause, m_player, &QMediaPlayer::pause);
    connect(controls, &WPlayerControls::stop, m_player, &QMediaPlayer::stop);
    connect(controls, &WPlayerControls::next, m_playlist, &QMediaPlaylist::next);
    connect(controls, &WPlayerControls::previous, m_playlist, &QMediaPlaylist::previous);
    connect(controls, &WPlayerControls::volumeChanged, m_player, &QMediaPlayer::setVolume);
    connect(controls, &WPlayerControls::muteChanged, m_player, &QMediaPlayer::setMuted);
    connect(controls, &WPlayerControls::rateChanged, m_player, &QMediaPlayer::setPlaybackRate);
    connect(controls, &WPlayerControls::stop, m_videoWidget, QOverload<>::of(&QVideoWidget::update));

    connect(m_player, &QMediaPlayer::stateChanged, controls, &WPlayerControls::setState);
    connect(m_player, &QMediaPlayer::volumeChanged, controls, &WPlayerControls::setVolume);
    connect(m_player, &QMediaPlayer::mutedChanged, controls, &WPlayerControls::setMuted);

    m_fullScreenButton = new QPushButton(tr("FullScreen"), this);
    m_fullScreenButton->setCheckable(true);

    m_colorButton = new QPushButton(tr("Color Options"), this);
    m_colorButton->setEnabled(false);

    connect(m_colorButton, &QPushButton::clicked, this, &WPlayer::showColorDialog);

    QBoxLayout* displayLayout = new QHBoxLayout();

    displayLayout->addWidget(m_videoWidget, 2);
    displayLayout->addWidget(m_playlistView);

    QBoxLayout* controlLayout = new QHBoxLayout();

    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->addWidget(openButton);
    controlLayout->addStretch(1);
    controlLayout->addWidget(controls);
    controlLayout->addStretch(1);
    controlLayout->addWidget(m_fullScreenButton);
    controlLayout->addWidget(m_colorButton);

    QHBoxLayout* horizontalLayout = new QHBoxLayout();

    horizontalLayout->addWidget(m_slider);
    horizontalLayout->addWidget(m_durationLabel);

    QBoxLayout* layout = new QVBoxLayout();

    layout->addLayout(displayLayout);
    layout->addLayout(horizontalLayout);
    layout->addLayout(controlLayout);
    layout->addLayout(histogramLayout);

    m_statusLabel = new QLabel();

    m_statusBar = new QStatusBar();
    m_statusBar->addPermanentWidget(m_statusLabel);
    m_statusBar->setSizeGripEnabled(false);

    layout->addWidget(m_statusBar);

    this->setLayout(layout);

    if (!this->isAvailable())
    {
        QMessageBox::warning(this, tr("Service not available"), tr("The QMediaPlayer object does not have a valid service.\n"
                                                                   "Please check that media service plugins are installed."));

        controls->setEnabled(false);

        m_playlistView->setEnabled(false);

        openButton->setEnabled(false);

        m_colorButton->setEnabled(false);

        m_fullScreenButton->setEnabled(false);
    }

    this->onMetaDataChanged();
}

WPlayer::~WPlayer()
{
}

bool WPlayer::isAvailable() const
{
    return m_player->isAvailable();
}

void WPlayer::addToPlaylist(const QList<QUrl> &urls)
{
    for (const auto& url : urls)
    {
        if (urlIsPlaylist(url))
        {
            m_playlist->load(url);
        }
        else
        {
            m_playlist->addMedia(url);
        }
    }
}

void WPlayer::setCustomAudioRole(const QString &role)
{
    m_player->setCustomAudioRole(role);
}

void WPlayer::onMetaDataChanged()
{
    if (m_player->isMetaDataAvailable())
    {
        this->setTrackInfo(QString("%1 - %2")
                           .arg(m_player->metaData(QMediaMetaData::AlbumArtist).toString())
                           .arg(m_player->metaData(QMediaMetaData::Title).toString()));

        if (m_coverLabel)
        {
            const QUrl url = m_player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            QPixmap pixmap;

            if (!url.isEmpty())
            {
                pixmap = QPixmap(url.toString());
            }

            m_coverLabel->setPixmap(pixmap);
        }
    }
}

void WPlayer::onStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::StoppedState)
    {
        this->clearHistogram();
    }
}

void WPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    this->handleCursor(status);

    switch (status)
    {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    {
        this->setStatusInfo(QString());

        break;
    }
    case QMediaPlayer::LoadingMedia:
    {
        this->setStatusInfo(tr("Loading media..."));

        break;
    }
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
    {
        this->setStatusInfo(tr("Buffering %1%").arg(m_player->bufferStatus()));

        break;
    }
    case QMediaPlayer::StalledMedia:
    {
        this->setStatusInfo(tr("Stalled %1%").arg(m_player->bufferStatus()));

        break;
    }
    case QMediaPlayer::EndOfMedia:
    {
        QApplication::alert(this);

        break;
    }
    case QMediaPlayer::InvalidMedia:
    {
        this->displayErrorMessage();

        break;
    }
    default: break;
    }
}

void WPlayer::onDurationChanged(qint64 duration)
{
    m_duration = duration / 1000;

    m_slider->setMaximum(m_duration);
}

void WPlayer::onPositionChanged(qint64 position)
{
    const qint64 duration = position / 1000;

    if (!m_slider->isSliderDown())
    {
        m_slider->setValue(duration);
    }

    this->updateDurationInfo(duration);
}

void WPlayer::onVideoAvailableChanged(bool b_available)
{
    if (!b_available)
    {
        disconnect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
        disconnect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);

        m_videoWidget->setFullScreen(false);
    }
    else
    {
        connect(m_fullScreenButton, &QPushButton::clicked, m_videoWidget, &QVideoWidget::setFullScreen);
        connect(m_videoWidget, &QVideoWidget::fullScreenChanged, m_fullScreenButton, &QPushButton::setChecked);

        if (m_fullScreenButton->isChecked())
        {
            m_videoWidget->setFullScreen(true);
        }
    }

    m_colorButton->setEnabled(b_available);
}

void WPlayer::onBufferStatusChanged(qint32 percentFilled)
{
    if (m_player->mediaStatus() == QMediaPlayer::StalledMedia)
    {
        this->setStatusInfo(tr("Stalled %1%").arg(percentFilled));
    }
    else
    {
        this->setStatusInfo(tr("Buffering %1%").arg(percentFilled));
    }
}

void WPlayer::onPlaylistPositionChanged(qint32 position)
{
    this->clearHistogram();

    m_playlistView->setCurrentIndex(m_playlistModel->index(position, 0));
}

void WPlayer::open()
{
    QFileDialog fileDialog(this);

    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));

    QStringList supportedMimeTypes = m_player->supportedMimeTypes();

    if (!supportedMimeTypes.isEmpty())
    {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists.

        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }

    const QString videoPath = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath());

    fileDialog.setDirectory(videoPath);

    if (fileDialog.exec() == QDialog::Accepted)
    {
        this->addToPlaylist(fileDialog.selectedUrls());
    }
}

void WPlayer::seek(qint32 seconds)
{
    m_player->setPosition(seconds * 1000);
}

void WPlayer::jump(const QModelIndex &index)
{
    if (index.isValid())
    {
        m_playlist->setCurrentIndex(index.row());

        m_player->play();
    }
}

void WPlayer::displayErrorMessage()
{

}

void WPlayer::showColorDialog()
{

}

void WPlayer::clearHistogram()
{

}

void WPlayer::setTrackInfo(const QString &info)
{

}

void WPlayer::setStatusInfo(const QString &info)
{

}

void WPlayer::handleCursor(QMediaPlayer::MediaStatus status)
{

}

void WPlayer::updateDurationInfo(qint64 durationInfo)
{

}

// TO DO:
// File: https://code.qt.io/cgit/qt/qtmultimedia.git/tree/examples/multimediawidgets/player/player.cpp?h=5.15
// Line: 248









































