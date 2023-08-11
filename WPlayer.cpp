#include "WPlayer.h"

WPlayer::WPlayer(QWidget *parent)
    : QWidget            (parent)
    , m_player           (nullptr)
    , m_playlist         (nullptr)
    , m_vieoWidget       (nullptr)
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

}

WPlayer::~WPlayer()
{

}

bool WPlayer::isPlayerAvailable() const
{

}

void WPlayer::addToPlaylist(const QList<QUrl> &urls)
{

}

void WPlayer::setCustomAudioRole(const QString &role)
{

}

void WPlayer::open()
{

}

void WPlayer::onDurationChanged(qint64 duration)
{

}

void WPlayer::onPositionChanged(qint64 position)
{

}

void WPlayer::onMetaDataChanged()
{

}

void WPlayer::onPreviousClicked()
{

}

void WPlayer::seek(qint32 seconds)
{

}

void WPlayer::jump(const QModelIndex &index)
{

}

void WPlayer::onPlaylistPositionChanged(qint32 position)
{

}

void WPlayer::onStatusChanged(QMediaPlayer::MediaStatus status)
{

}

void WPlayer::onStateChanged(QMediaPlayer::State state)
{

}

void WPlayer::onBufferProgressChanged(qint32 progress)
{

}

void WPlayer::onVideoAvailableChanged(bool b_available)
{

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
