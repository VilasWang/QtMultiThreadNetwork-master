
#include "listview.h"
#include <qDebug>
#include <QScrollBar>

Listview::Listview(QWidget* parent/* = NULL*/)
	: QListView(parent)
	, m_pModel(nullptr)
	, m_pDelegate(nullptr)
{
	initView();
}

Listview::~Listview()
{

}

void Listview::initView()
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

void Listview::setListDelegate(ListDelegate *pDelegate)
{
	if (pDelegate != nullptr)
	{
		m_pDelegate = pDelegate;
		setItemDelegate(m_pDelegate);
	}
}

void Listview::setListModel(ListModel *pModel)
{
	if (pModel != nullptr)
	{
		m_pModel = pModel;
		setModel(m_pModel);
	}
}

void Listview::insert(const QVariant& var)
{
	if (m_pModel != nullptr)
	{
		m_pModel->insert(var);
	}
}

void Listview::insert(QVector<QVariant>& vec)
{
	if (m_pModel != nullptr)
	{
		m_pModel->insert(vec);
	}
}

void Listview::resetAll(QVector<QVariant>& vec)
{
	if (m_pModel != nullptr)
	{
		m_pModel->resetAll(vec);
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
	beginResetModel();
	m_vecVariant.clear();
	endResetModel();

	emit sizeChanged(m_vecVariant.size());
}

void ListModel::insert(const QVariant& var)
{
	beginInsertRows(QModelIndex(), m_vecVariant.size(), m_vecVariant.size());
	m_vecVariant << var;
	endInsertRows();

	emit sizeChanged(m_vecVariant.size());
}

void ListModel::insert(QVector<QVariant>& vec)
{
	if (vec.size() > 0)
	{
		beginInsertRows(QModelIndex(), m_vecVariant.size(), m_vecVariant.size() + vec.size() - 1);
		m_vecVariant << vec;
		endInsertRows();

		emit sizeChanged(m_vecVariant.size());
	}
}

void ListModel::resetAll(const QVector<QVariant>& vec)
{
	if (vec.size() > 0)
	{
		beginInsertRows(QModelIndex(), 0, m_vecVariant.size() - 1);
		m_vecVariant << vec;
		endInsertRows();

		emit sizeChanged(m_vecVariant.size());
	}
}

//////////////////////////////////////////////////////////////////////////
ListDelegate::ListDelegate(QObject* parent /*= NULL*/)
	: QStyledItemDelegate(parent)
{

}

ListDelegate::~ListDelegate()
{

}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return __super::sizeHint(option, index);
}

void ListDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	__super::paint(painter, option, index);
}

bool ListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
	const QStyleOptionViewItem &option, const QModelIndex &index)
{
	return __super::editorEvent(event, model, option, index);
}