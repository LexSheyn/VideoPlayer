#ifndef WPLAYERCONTROLS_H
#define WPLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
QT_END_NAMESPACE

class WPlayerControls : public QWidget
{
    Q_OBJECT;

public:

    explicit WPlayerControls(QWidget* parent = nullptr);
    virtual ~WPlayerControls() override;

    QMediaPlayer::State state() const;
    qint32 volume() const;
    bool isMuted() const;
    qreal playbackRate() const;

public slots:

    void setState(QMediaPlayer::State state);
    void setVolume(qint32 volume);
    void setMuted(bool b_muted);
    void setPlaybackRate(qreal rate);

signals:

    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void volumeChanged(qint32 volume);
    void muteChanged(bool b_muted);
    void rateChanged(qreal rate);

private slots:

    void onPlayClicked();
    void onMuteClicked();
    void onRateUpdated();
    void onVolumeSliderValueChanged();

private:

    QMediaPlayer::State m_playerState;
    bool                mb_playerMuted;
    QAbstractButton*    m_playButton;
    QAbstractButton*    m_stopButton;
    QAbstractButton*    m_nextButton;
    QAbstractButton*    m_previousButton;
    QAbstractButton*    m_muteButton;
    QAbstractSlider*    m_volumeSlider;
    QComboBox*          m_rateBox;
};

#endif // WPLAYERCONTROLS_H

// TO DO:
// File: https://code.qt.io/cgit/qt/qtmultimedia.git/tree/examples/multimediawidgets/player/playercontrols.h?h=5.15
// Line: 54
