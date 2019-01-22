
#include "listview.h"
#include <qDebug>
#include <QScrollBar>

Listview::Listview(QWidget* parent/* = NULL*/ )
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
	verticalScrollBar()->setContentsMargins(0,0,0,0);
	verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
	horizontalScrollBar()->setContentsMargins(0,0,0,0);
	horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void Listview::initModelDelagate()
{
}

void Listview::setListDelegate(Delegate *pDelegate)
{
	if (pDelegate != nullptr)
	{
		m_pDelegate = pDelegate;
		setItemDelegate(m_pDelegate);
	}
}

void Listview::setListModel(Model *pModel)
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

void Listview::updateAll(QVector<QVariant>& vec)
{
	if (m_pModel != nullptr)
	{
		m_pModel->updateAll(vec);
	}
}

//////////////////////////////////////////////////////////////////////////
Model::Model(QObject* parent /*= NULL*/)
	: QAbstractListModel(parent)
{

}

Model::~Model()
{
	m_vecVariant.clear();
}

int Model::rowCount(const QModelIndex & parent /*= QModelIndex()*/) const
{
	Q_UNUSED(parent);
	return m_vecVariant.size();
}

QVariant Model::data(const QModelIndex & index, int role /* = Qt::DisplayRole */) const
{
	QVariant var;
	if ( index.isValid() && role == Qt::DisplayRole )
	{
		int nIndex = index.row();
		if ( nIndex < m_vecVariant.size() )
		{
			var.setValue(m_vecVariant[nIndex]);
		}
	}
	return var;
}

void Model::clearModel()
{
	beginRemoveRows(QModelIndex(),0,m_vecVariant.size());
	m_vecVariant.clear();
	endRemoveRows();
}

void Model::insert(const QVariant& var)
{
	beginInsertRows(QModelIndex(), m_vecVariant.size(), m_vecVariant.size());
	m_vecVariant << var;
	endInsertRows();
}

void Model::insert(QVector<QVariant>& vec)
{
	if (vec.size() > 0)
	{
		beginInsertRows(QModelIndex(), m_vecVariant.size(), m_vecVariant.size() + vec.size() - 1);
		m_vecVariant << vec;
		endInsertRows();
	}
}

void Model::updateAll(const QVector<QVariant>& vec)
{
	clearModel();

	if (vec.size() > 0)
	{
		beginInsertRows(QModelIndex(), 0, m_vecVariant.size() - 1);
		m_vecVariant << vec;
		endInsertRows();
	}
}

//////////////////////////////////////////////////////////////////////////
Delegate::Delegate(QObject* parent /*= NULL*/)
	: QStyledItemDelegate(parent)
{
	
}

Delegate::~Delegate()
{

}

QSize Delegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return __super::sizeHint(option, index);
}

void Delegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	__super::paint(painter, option, index);
}

bool Delegate::editorEvent(QEvent *event, QAbstractItemModel *model,
				 const QStyleOptionViewItem &option, const QModelIndex &index)
{
	return __super::editorEvent(event, model, option, index);
}