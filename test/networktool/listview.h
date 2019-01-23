#ifndef listview_h
#define listview_h

#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QListView>
#include <QPointer>

class ListModel;
class ListDelegate;

class Listview : public QListView
{
	Q_OBJECT

public:
	explicit Listview(QWidget* parent = NULL );
	virtual ~Listview();

public:
	void setListDelegate(ListDelegate *);
	void setListModel(ListModel *);
	void insert(const QVariant&);
	void insert(QVector<QVariant>& vec);
	void updateAll(QVector<QVariant>&);

private:
	void initView();

private:
	QPointer<ListModel> m_pModel;
	QPointer<ListDelegate> m_pDelegate;
};

class ListModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit ListModel(QObject* parent = NULL);
	virtual ~ListModel();

protected:
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public:
	void insert(const QVariant&);
	void insert(QVector<QVariant>& vec);
	void updateAll(const QVector<QVariant>&);
	void clear();

protected:
	QVector<QVariant> m_vecVariant;
};

class ListDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit ListDelegate(QObject* parent = NULL);
	virtual ~ListDelegate();

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