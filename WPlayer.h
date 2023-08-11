#ifndef WPLAYER_H
#define WPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QSlider;
class QStatusBar;
class QVideoProbe;
class QVideoWidget;
class QAudioProbe;
QT_END_NAMESPACE

class WPlaylistModel;
class WHistogramWidget;

class WPlayer : public QWidget
{
    Q_OBJECT;

public:

    explicit WPlayer(QWidget* parent = nullptr);
    virtual ~WPlayer() override;

    bool isPlayerAvailable() const;

    void addToPlaylist(const QList<QUrl>& urls);
    void setCustomAudioRole(const QString& role);

signals:

    void fullScreenChanged(bool b_fullScreen);

private slots:

    void open();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onMetaDataChanged();

    void onPreviousClicked();

    void seek(qint32 seconds);
    void jump(const QModelIndex& index);
    void onPlaylistPositionChanged(qint32 position);

    void onStatusChanged(QMediaPlayer::MediaStatus status);
    void onStateChanged(QMediaPlayer::State state);
    void onBufferProgressChanged(qint32 progress);
    void onVideoAvailableChanged(bool b_available);

    void displayErrorMessage();

    void showColorDialog();

private:

    void clearHistogram();
    void setTrackInfo(const QString& info);
    void setStatusInfo(const QString& info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 durationInfo);

private:

    QMediaPlayer*      m_player;
    QMediaPlaylist*    m_playlist;
    QVideoWidget*      m_vieoWidget;
    QLabel*            m_coverLabel;
    QSlider*           m_slider;
    QLabel*            m_durationLabel;
    QPushButton*       m_fullScreenButton;
    QPushButton*       m_colorButton;
    QDialog*           m_colorDialog;
    QLabel*            m_statusLabel;
    QStatusBar*        m_statusBar;

    QLabel*            m_histogramLabel;
    WHistogramWidget*  m_videoHistogram;
    WHistogramWidget*  m_audioHistogram;
    QVideoProbe*       m_videoProbe;
    QAudioProbe*       m_audioProbe;

    WPlaylistModel*    m_playlistModel;
    QAbstractItemView* m_playlistView;
    QString            m_trackInfo;
    QString            m_statusInfo;
    qint64             m_duration;
};

#endif // WPLAYER_H


// TO DO:
// File: https://code.qt.io/cgit/qt/qtmultimedia.git/tree/examples/multimediawidgets/player/player.h?h=5.15
// Line: 113











































