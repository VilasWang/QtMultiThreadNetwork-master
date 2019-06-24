
#include "tableview.h"
#include <qDebug>
#include <QScrollBar>

TableView::TableView(QWidget* parent/* = NULL*/)
    : QTableView(parent)
    , m_pModel(nullptr)
    , m_pDelegate(nullptr)
{
    initTableView();
}

TableView::~TableView()
{
    reset();
}

void TableView::initTableView()
{
    setFrameShape(QFrame::NoFrame);

    setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    verticalScrollBar()->setContentsMargins(0, 0, 0, 0);
    verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    horizontalScrollBar()->setContentsMargins(0, 0, 0, 0);
    horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void TableView::setTableItemDelegate(TableItemDelegate *pDelegate)
{
    if (pDelegate != nullptr)
    {
        m_pDelegate = pDelegate;
        setItemDelegate(m_pDelegate);
    }
}

void TableView::setTabletModel(TableModel *pModel)
{
    if (pModel != nullptr)
    {
        m_pModel = pModel;
        setModel(m_pModel);
    }
}

void TableView::insert(const QVariant& var)
{
    if (m_pModel != nullptr && var.isValid())
    {
        m_pModel->insert(var);
        update();
    }
}

void TableView::insert(const QVector<QVariant>& vec)
{
    if (m_pModel != nullptr && !vec.isEmpty())
    {
        m_pModel->insert(vec);
        update();
    }
}

void TableView::remove(const QVariant& var)
{
    if (m_pModel != nullptr && var.isValid())
    {
        m_pModel->remove(var);
        update();
    }
}

void TableView::clear()
{
    if (m_pModel != nullptr)
    {
        m_pModel->clear();
        update();
    }
}

//////////////////////////////////////////////////////////////////////////
TableModel::TableModel(QObject* parent /*= NULL*/)
    : QAbstractTableModel(parent)
{

}

TableModel::~TableModel()
{
    m_vecVariant.clear();
}

int TableModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const
{
    Q_UNUSED(parent);
    int mod = m_vecVariant.size() % columnCount();
    if (mod != 0)
    {
        return m_vecVariant.size() / columnCount() + 1;
    }
    return m_vecVariant.size() / columnCount();
}

int TableModel::columnCount(const QModelIndex &parent/* = QModelIndex()*/) const
{
    return 1;
}

QVariant TableModel::data(const QModelIndex & index, int role /* = Qt::DisplayRole */) const
{
    QVariant var;
    if (index.isValid() && role == Qt::DisplayRole)
    {
        int nIndex = index.row() * columnCount() + index.column();
        if (nIndex < m_vecVariant.size())
        {
            var.setValue(m_vecVariant[nIndex]);
        }
    }
    return var;
}

void TableModel::clear()
{
    if (!m_vecVariant.isEmpty())
    {
        beginResetModel();
        m_vecVariant.clear();
        endResetModel();

        emit sizeChanged(m_vecVariant.size());
    }
}

void TableModel::insert(const QVariant& var)
{
    if (var.isValid())
    {
        if (m_vecVariant.size() == 0 || m_vecVariant.size() % columnCount() == 0)
        {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_vecVariant << var;
            endInsertRows();
        }
        else
        {
            beginResetModel();
            m_vecVariant << var;
            endResetModel();
        }

        emit sizeChanged(m_vecVariant.size());
    }
}

void TableModel::insert(const QVector<QVariant>& vec)
{
    if (vec.size() > 0)
    {
        int newRowCount = vec.size() / columnCount();
        if (vec.size() % columnCount() > m_vecVariant.size() % columnCount())
        {
            newRowCount++;
        }

        if (newRowCount == 0 && m_vecVariant.size() == 0)
        {
            newRowCount++;
        }

        if (newRowCount > 0)
        {
            beginInsertRows(QModelIndex(), rowCount(), rowCount() + newRowCount - 1);
            m_vecVariant << vec;
            endInsertRows();
        }
        else
        {
            beginResetModel();
            m_vecVariant << vec;
            endResetModel();
        }

        emit sizeChanged(m_vecVariant.size());
    }
}

void TableModel::remove(const QVariant& var)
{
    if (var.isValid() && m_vecVariant.contains(var))
    {
        beginResetModel();
        m_vecVariant.removeOne(var);
        endResetModel();

        emit sizeChanged(m_vecVariant.size());
    }
}

void TableModel::resetAll(const QVector<QVariant>& vec)
{
    if (vec.size() > 0)
    {
        beginResetModel();
        endResetModel();

        int rowCount = vec.size() % columnCount() == 0 ? vec.size() / columnCount() : vec.size() / columnCount() + 1;
        beginInsertRows(QModelIndex(), 0, rowCount - 1);
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
TableItemDelegate::TableItemDelegate(QObject* parent /*= NULL*/)
    : QItemDelegate(parent)
{

}

TableItemDelegate::~TableItemDelegate()
{

}

QSize TableItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return __super::sizeHint( option, index);
}

void TableItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    __super::paint(painter, option, index);
}

bool TableItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
    const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return __super::editorEvent(event, model, option, index);
}