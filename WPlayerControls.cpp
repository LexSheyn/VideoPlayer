#include "WPlayerControls.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QAudio>

WPlayerControls::WPlayerControls(QWidget *parent)
    : QWidget (parent)
{
    const QStyle* style = this->style();

    m_playButton = new QToolButton(this);
    m_playButton->setIcon(style->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked, this, &WPlayerControls::onPlayClicked);

    m_stopButton = new QToolButton(this);
    m_stopButton->setIcon(style->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setEnabled(false);

    connect(m_stopButton, &QAbstractButton::clicked, this, &WPlayerControls::stop);

    m_nextButton = new QToolButton(this);
    m_nextButton->setIcon(style->standardIcon(QStyle::SP_MediaSkipForward));

    connect(m_nextButton, &QAbstractButton::clicked, this, &WPlayerControls::next);

    m_previousButton = new QToolButton(this);
    m_previousButton->setIcon(style->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(m_previousButton, &QAbstractButton::clicked, this, &WPlayerControls::previous);

    m_muteButton = new QToolButton(this);
    m_muteButton->setIcon(style->standardIcon(QStyle::SP_MediaVolume));

    connect(m_muteButton, &QAbstractButton::clicked, this, &WPlayerControls::onMuteClicked);

    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);

    connect(m_volumeSlider, &QSlider::valueChanged, this, &WPlayerControls::onVolumeSliderValueChanged);

    m_rateBox = new QComboBox(this);
    m_rateBox->addItem("0.5x", QVariant(0.5));
    m_rateBox->addItem("1.0x", QVariant(1.0));
    m_rateBox->addItem("2.0x", QVariant(2.0));
    m_rateBox->setCurrentIndex(1);

    connect(m_rateBox, QOverload<qint32>::of(&QComboBox::activated), this, &WPlayerControls::onRateUpdated);

    QBoxLayout* layout = new QHBoxLayout();

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_stopButton);
    layout->addWidget(m_previousButton);
    layout->addWidget(m_playButton);
    layout->addWidget(m_nextButton);
    layout->addWidget(m_muteButton);
    layout->addWidget(m_volumeSlider);
    layout->addWidget(m_rateBox);

    this->setLayout(layout);
}

WPlayerControls::~WPlayerControls()
{
}

QMediaPlayer::State WPlayerControls::state() const
{
    return m_playerState;
}

qint32 WPlayerControls::volume() const
{
    const qreal linearVolume = QAudio::convertVolume(m_volumeSlider->value() / qreal(100), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}

bool WPlayerControls::isMuted() const
{
    return mb_playerMuted;
}

qreal WPlayerControls::playbackRate() const
{
    return m_rateBox->itemData(m_rateBox->currentIndex()).toDouble();
}

void WPlayerControls::setState(QMediaPlayer::State state)
{
    if (state != m_playerState)
    {
        m_playerState = state;

        const QStyle* style = this->style();

        switch (m_playerState)
        {
            case QMediaPlayer::StoppedState:
            {
                m_stopButton->setEnabled(false);
                m_playButton->setIcon(style->standardIcon(QStyle::SP_MediaPlay));

                break;
            }

            case QMediaPlayer::PlayingState:
            {
                m_stopButton->setEnabled(true);
                m_playButton->setIcon(style->standardIcon(QStyle::SP_MediaPause));

                break;
            }

            case QMediaPlayer::PausedState:
            {
                m_stopButton->setEnabled(true);
                m_playButton->setIcon(style->standardIcon(QStyle::SP_MediaPlay));

                break;
            }
        }
    }
}

void WPlayerControls::setVolume(qint32 volume)
{
    const qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100), QAudio::LinearVolumeScale, QAudio::LogarithmicVolumeScale);

    m_volumeSlider->setValue(qRound(logarithmicVolume * 100));
}

void WPlayerControls::setMuted(bool b_muted)
{
    if (mb_playerMuted != b_muted)
    {
        mb_playerMuted = b_muted;

        const QStyle* style = this->style();

        if (mb_playerMuted)
        {
            m_muteButton->setIcon(style->standardIcon(QStyle::SP_MediaVolumeMuted));
        }
        else
        {
            m_muteButton->setIcon(style->standardIcon(QStyle::SP_MediaVolume));
        }
    }
}

void WPlayerControls::setPlaybackRate(qreal rate)
{
    for (qint32 i = 0; i < m_rateBox->count(); ++i)
    {
        if (qFuzzyCompare(rate, qreal(m_rateBox->itemData(i).toDouble())))
        {
            m_rateBox->setCurrentIndex(i);

            return;
        }
    }

    m_rateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
    m_rateBox->setCurrentIndex(m_rateBox->count() - 1);
}

void WPlayerControls::onPlayClicked()
{
    switch (m_playerState)
    {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
        {
            emit this->play();

            break;
        }

        case QMediaPlayer::PlayingState:
        {
            emit this->pause();

            break;
        }
    }
}

void WPlayerControls::onMuteClicked()
{
    emit this->muteChanged(!mb_playerMuted);
}

void WPlayerControls::onRateUpdated()
{
    const qreal playbackRate = this->playbackRate();

    emit this->rateChanged(playbackRate);
}

void WPlayerControls::onVolumeSliderValueChanged()
{
    const qint32 volume = this->volume();

    emit this->volumeChanged(volume);
}
