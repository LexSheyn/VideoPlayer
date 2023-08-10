#ifndef WPLAYLISTMODEL_H
#define WPLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE
class QMediaPlaylist;
QT_END_NAMESPACE

class WPlaylistModel : public QAbstractItemModel
{
    Q_OBJECT;

public:

    enum Column
    {
        Title = 0,
        ColumtCount
    };

    explicit WPlaylistModel(QObject* parent = nullptr);
    virtual ~WPlaylistModel() override;

public:

    // QAbstractItemModel.

    virtual QModelIndex index(qint32 row, qint32 column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex& child) const override;

    virtual qint32 rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual qint32 columnCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex& index, qint32 role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex& index, const QVariant& value, qint32 role = Qt::DisplayRole) override;

public:

    void setPlaylist(QMediaPlaylist* playlist);
    QMediaPlaylist* getPlaylist() const;

private slots:

    void beginInsertItems(qint32 start, qint32 end);
    void endInsertItems();
    void beginRemoveItems(qint32 start, qint32 end);
    void endRemoveItems();
    void changeItems(qint32 start, qint32 end);

private:

    QScopedPointer<QMediaPlaylist> m_playlist;
    QMap<QModelIndex, QVariant>    m_data;
};

#endif // WPLAYLISTMODEL_H
