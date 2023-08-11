#include "WPlaylistModel.h"

#include <QMediaPlaylist>
#include <QFileInfo>

WPlaylistModel::WPlaylistModel(QObject *parent)
    : QAbstractItemModel (parent)
{
}

WPlaylistModel::~WPlaylistModel()
{
}

QModelIndex WPlaylistModel::index(qint32 row, qint32 column, const QModelIndex &parent) const
{
    if (m_playlist && (!parent.isValid()) &&
        (row >= 0) &&
        (row < m_playlist->mediaCount()) &&
        (column >= 0) &&
        (column < ColumnCount))
    {
        return this->createIndex(row, column);
    }

    return QModelIndex();
}

QModelIndex WPlaylistModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

qint32 WPlaylistModel::rowCount(const QModelIndex &parent) const
{
    if (m_playlist && (!parent.isValid()))
    {
        return m_playlist->mediaCount();
    }

    return 0;
}

qint32 WPlaylistModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return ColumnCount;
    }

    return 0;
}

QVariant WPlaylistModel::data(const QModelIndex &index, qint32 role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        const QVariant value = m_data[index];

        if (!value.isValid() && (index.column() == Title))
        {
            const QUrl location = m_playlist->media(index.row()).request().url();

            return QFileInfo(location.path()).fileName();
        }

        return value;
    }

    return QVariant();
}

bool WPlaylistModel::setData(const QModelIndex &index, const QVariant &value, qint32 role)
{
    Q_UNUSED(role);

    m_data[index] = value;

    emit this->dataChanged(index, index);

    return true;
}

void WPlaylistModel::setPlaylist(QMediaPlaylist *playlist)
{
    if (m_playlist)
    {
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted, this, &WPlaylistModel::beginInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaInserted, this, &WPlaylistModel::endInsertItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved, this, &WPlaylistModel::beginRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaRemoved, this, &WPlaylistModel::endRemoveItems);
        disconnect(m_playlist.data(), &QMediaPlaylist::mediaChanged, this, &WPlaylistModel::changeItems);
    }

    this->beginResetModel();

    m_playlist.reset(playlist);

    if (m_playlist)
    {
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeInserted, this, &WPlaylistModel::beginInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaInserted, this, &WPlaylistModel::endInsertItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaAboutToBeRemoved, this, &WPlaylistModel::beginRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaRemoved, this, &WPlaylistModel::endRemoveItems);
        connect(m_playlist.data(), &QMediaPlaylist::mediaChanged, this, &WPlaylistModel::changeItems);
    }

    this->endResetModel();
}

void WPlaylistModel::beginInsertItems(qint32 start, qint32 end)
{
    m_data.clear();

    this->beginInsertRows(QModelIndex(), start, end);
}

void WPlaylistModel::endInsertItems()
{
    this->endInsertRows();
}

void WPlaylistModel::beginRemoveItems(qint32 start, qint32 end)
{
    m_data.clear();

    this->beginRemoveRows(QModelIndex(), start, end);
}

void WPlaylistModel::endRemoveItems()
{
    this->endInsertRows();
}

void WPlaylistModel::changeItems(qint32 start, qint32 end)
{
    m_data.clear();

    emit this->dataChanged(this->index(start, 0), this->index(end, ColumnCount));
}






















































