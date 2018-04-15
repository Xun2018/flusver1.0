#ifndef BANDLIST_H
#define BANDLIST_H

#include <QTreeView>
#include <QStandardItemModel>
#include "TiffDataRead.h"

class GeoDpCAsys;
class DynaSimulation;

class BandList : public QTreeView
{
	Q_OBJECT

public:
	BandList(QWidget *parent);
	~BandList();

public slots:
	bool getBandList(QList<TiffDataRead*> _inUi_poDataset, int _serialnum);
	bool getOtherListModel(QList<TiffDataRead*> _inUi_poDataset, int _serialnum);
	bool showlabel(DynaSimulation* _dsn);
	/// <summary>
	/// <�����ļ��б�����ģ��>
	/// </summary>
	/// <returns>�ļ��б�����ģ��.</returns>
	QStandardItemModel* FileListModel()
	{
		return fileListModel;
	};
	/// <summary>
	/// <����fileListModelͼ���ļ��б�����ģ��>
	/// </summary>
	/// <paramname="model">�ļ��б�����ģ��.</param>
	void SetFileListModel( QStandardItemModel* model)
	{
		this->fileListModel = model;
	};

	QStandardItem* getRootItem(); 

private:
	QStandardItemModel *fileListModel;
	QStandardItem *rootItem;
	QStandardItem *childItem;
};

#endif // BANDLIST_H
