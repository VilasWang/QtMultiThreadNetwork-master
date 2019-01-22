#ifndef listview_h
#define listview_h

#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QListView>
#include <QPointer>

class Model;
class Delegate;

class Listview : public QListView
{
	Q_OBJECT

public:
	explicit Listview(QWidget* parent = NULL );
	~Listview();

public:
	void setListDelegate(Delegate *);
	void setListModel(Model *);
	void insert(const QVariant&);
	void insert(QVector<QVariant>& vec);
	void updateAll(QVector<QVariant>&);

private:
	void initView();
	void initModelDelagate();

private:
	QPointer<Model> m_pModel;
	QPointer<Delegate> m_pDelegate;
};

class Model : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit Model(QObject* parent = NULL);
	~Model();

protected:
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

public:
	void insert(const QVariant&);
	void insert(QVector<QVariant>& vec);
	void updateAll(const QVector<QVariant>&);
	void clearModel();

private:
	QVector<QVariant> m_vecVariant;
};

class Delegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit Delegate(QObject* parent = NULL);
	~Delegate();

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