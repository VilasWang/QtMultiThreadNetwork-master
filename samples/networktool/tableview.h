#ifndef CUSTOM_TABLEVIEW_H
#define CUSTOM_TABLEVIEW_H

#include <QItemDelegate>
#include <QAbstractListModel>
#include <QTableView>
#include <QPointer>

class TableModel;
class TableItemDelegate;
class TableView : public QTableView
{
    Q_OBJECT

public:
    explicit TableView(QWidget* parent = NULL);
    virtual ~TableView();

public:
    void setTableItemDelegate(TableItemDelegate *);
    void setTabletModel(TableModel *);

    void insert(const QVariant&);
    void insert(const QVector<QVariant>& vec);
    void remove(const QVariant&);
    void clear();

private:
    void initTableView();

private:
    QPointer<TableModel> m_pModel;
    QPointer<TableItemDelegate> m_pDelegate;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject* parent = NULL);
    virtual ~TableModel();

protected:
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

public:
    void insert(const QVariant&);
    void insert(const QVector<QVariant>& vec);
    void remove(const QVariant&);
    void clear();
    void resetAll(const QVector<QVariant>& vec);

Q_SIGNALS:
    void sizeChanged(int);

protected:
    QVector<QVariant> m_vecVariant;
};

class TableItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit TableItemDelegate(QObject* parent = NULL);
    virtual ~TableItemDelegate();

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