#include "listview.h"
#include <QDebug>
#include <QScrollBar>

ListView::ListView(QWidget* parent/* = NULL*/)
    : QListView(parent)
    , m_pModel(nullptr)
    , m_pDelegate(nullptr)
{
    initListView();
}

ListView::~ListView()
{
    reset();
}

void ListView::initListView()
{
    setFrameShape(QFrame::NoFrame);
    setSpacing(0);

    setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    verticalScrollBar()->setContentsMargins(0, 0, 0, 0);
    verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    horizontalScrollBar()->setContentsMargins(0, 0, 0, 0);
    horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void ListView::setListDelegate(ListItemDelegate *pDelegate)
{
    if (pDelegate != nullptr)
    {
        m_pDelegate = pDelegate;
        setItemDelegate(m_pDelegate);
    }
}

void ListView::setListModel(ListModel *pModel)
{
    if (pModel != nullptr)
    {
        m_pModel = pModel;
        setModel(m_pModel);
    }
}

void ListView::insert(const QVariant& var)
{
    if (m_pModel != nullptr && var.isValid())
    {
        m_pModel->insert(var);
        update();
    }
}

void ListView::insert(const QVector<QVariant>& vec)
{
    if (m_pModel != nullptr && !vec.isEmpty())
    {
        m_pModel->insert(vec);
        update();
    }
}

void ListView::remove(const QVariant& var)
{
    if (m_pModel != nullptr && var.isValid())
    {
        m_pModel->remove(var);
        update();
    }
}

void ListView::remove(int row)
{
    if (m_pModel != nullptr && row >= 0)
    {
        m_pModel->remove(row);
        update();
    }
}

void ListView::clear()
{
    if (m_pModel != nullptr)
    {
        m_pModel->clear();
        update();
    }
}

//////////////////////////////////////////////////////////////////////////
ListModel::ListModel(QObject* parent /*= NULL*/)
    : QAbstractListModel(parent)
{

}

ListModel::~ListModel()
{
    m_vecVariant.clear();
}

int ListModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const
{
    Q_UNUSED(parent);
    return m_vecVariant.size();
}

QVariant ListModel::data(const QModelIndex & index, int role /* = Qt::DisplayRole */) const
{
    QVariant var;
    if (index.isValid() && role == Qt::DisplayRole)
    {
        int nIndex = index.row();
        if (nIndex < m_vecVariant.size())
        {
            var.setValue(m_vecVariant[nIndex]);
        }
    }
    return var;
}

void ListModel::clear()
{
    if (!m_vecVariant.isEmpty())
    {
        beginResetModel();
        m_vecVariant.clear();
        endResetModel();

        emit sizeChanged(m_vecVariant.size());
    }
}

void ListModel::insert(const QVariant& var)
{
    if (var.isValid())
    {
        beginInsertRows(QModelIndex(), m_vecVariant.size(), m_vecVariant.size());
        m_vecVariant << var;
        endInsertRows();

        emit sizeChanged(m_vecVariant.size());
    }
}

void ListModel::insert(const QVector<QVariant>& vec)
{
    if (vec.size() > 0)
    {
        beginInsertRows(QModelIndex(), m_vecVariant.size(), m_vecVariant.size() + vec.size() - 1);
        m_vecVariant << vec;
        endInsertRows();

        emit sizeChanged(m_vecVariant.size());
    }
}

void ListModel::remove(const QVariant& var)
{
    if (var.isValid() && m_vecVariant.contains(var))
    {
        int index = m_vecVariant.indexOf(var);
        Q_ASSERT(index < rowCount());
        beginRemoveRows(QModelIndex(), index, index);
        m_vecVariant.removeOne(var);
        endRemoveRows();

        emit sizeChanged(m_vecVariant.size());
    }
}

void ListModel::remove(int row)
{
    if (row >= 0 && m_vecVariant.size() > row)
    {
        beginRemoveRows(QModelIndex(), row, row);
        m_vecVariant.remove(row);
        endRemoveRows();

        emit sizeChanged(m_vecVariant.size());
    }
}

void ListModel::resetAll(const QVector<QVariant>& vec)
{
    if (vec.size() > 0)
    {
        beginResetModel();
        endResetModel();

        beginInsertRows(QModelIndex(), 0, vec.size() - 1);
        m_vecVariant = vec;
        endInsertRows();

        emit sizeChanged(m_vecVariant.size());
    }
    else
    {
        clear();
    }
}

//////////////////////////////////////////////////////////////////////////
ListItemDelegate::ListItemDelegate(QObject* parent /*= NULL*/)
    : QStyledItemDelegate(parent)
{

}

ListItemDelegate::~ListItemDelegate()
{

}

QSize ListItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return __super::sizeHint(option, index);
}

void ListItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    __super::paint(painter, option, index);
}

bool ListItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
    const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return __super::editorEvent(event, model, option, index);
}