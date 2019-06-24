#ifndef CUSTOM_LISTVIEW_H
#define CUSTOM_LISTVIEW_H

#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QListView>
#include <QPointer>

class ListModel;
class ListItemDelegate;
class ListView : public QListView
{
    Q_OBJECT

public:
    explicit ListView(QWidget* parent = NULL);
    virtual ~ListView();

public:
    void setListDelegate(ListItemDelegate *);
    void setListModel(ListModel *);

    void insert(const QVariant&);
    void insert(const QVector<QVariant>& vec);
    void remove(const QVariant&);
    void remove(int row);
    void clear();

private:
    void initListView();

private:
    QPointer<ListModel> m_pModel;
    QPointer<ListItemDelegate> m_pDelegate;
};

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ListModel(QObject* parent = NULL);
    virtual ~ListModel();

protected:
    int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

public:
    void insert(const QVariant&);
    void insert(const QVector<QVariant>& vec);
    void remove(const QVariant&);
    void remove(int row);
    void clear();
    void resetAll(const QVector<QVariant>& vec);

Q_SIGNALS:
    void sizeChanged(int);

protected:
    QVector<QVariant> m_vecVariant;
};

class ListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ListItemDelegate(QObject* parent = NULL);
    virtual ~ListItemDelegate();

public:
    void paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const Q_DECL_OVERRIDE;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
        const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
};

#endif