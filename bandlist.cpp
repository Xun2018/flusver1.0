#include "bandlist.h"
#include "geodpcasys.h"
#include "dynasimulation.h"
#include <QFileInfo>
#include <QStandardItem>
#include <QMessageBox>

BandList::BandList(QWidget *parent)
	: QTreeView(parent)
{
	fileListModel = new QStandardItemModel(this);
	fileListModel->setHorizontalHeaderLabels(QStringList()<<"Data");
	QSizePolicy policy( QSizePolicy::Preferred,QSizePolicy::Preferred );
	this->setSizePolicy( policy );
}


BandList::~BandList()
{

}
/// <summary>
/// <ͼ������б�>
/// </summary>
bool BandList::getBandList( QList<TiffDataRead*> _inUi_poDataset, int _serialnum)
{
	if (_inUi_poDataset[_serialnum]->poDataset() == NULL )
	{
		return true;
	}
	QFileInfo fileInfo(_inUi_poDataset[_serialnum]->getFileName());
	rootItem = new QStandardItem(fileInfo.fileName());
	// <���׽ڵ�Ӹ�ѡ��>
// 	rootItem->setCheckable(false); 
// 	rootItem->setCheckState(Qt::Unchecked);
	fileListModel->appendRow(rootItem);// <��ɾ��ԭ����>
	for ( int i = 0; i <_inUi_poDataset[_serialnum]->poDataset()->GetRasterCount(); i++ )
	{
		childItem = new QStandardItem( tr( "Band %1" ).arg( i + 1 ) );
		rootItem->setChild(i, childItem);
	}
	fileListModel->setItem(0, rootItem );
	return true;
}

QStandardItem* BandList::getRootItem()
{
	return rootItem;
}
/// <summary>
/// <���ݱ�ǣ��б�>
/// </summary>
bool BandList::getOtherListModel(QList<TiffDataRead*> _inUi_poDataset, int _serialnum)
{
	for (int ii=0;ii<_inUi_poDataset.size();ii++)
	{
		if (_inUi_poDataset[ii]->poDataset() == NULL )
		{
			return true;
		}
		QFileInfo fileInfo(_inUi_poDataset[ii]->getFileName());
		rootItem = new QStandardItem(fileInfo.fileName());
		fileListModel->appendRow(rootItem);// <��ɾ��ԭ����>
		for ( int i = 0; i <_inUi_poDataset[ii]->poDataset()->GetRasterCount(); i++ )
		{
			childItem = new QStandardItem( tr( "Band %1" ).arg( i + 1 ) );
			rootItem->setChild(i, childItem);
			childItem->setForeground(QBrush(QColor(255, 255, 255)));
			childItem->setEditable(false);
			childItem->setEnabled(false);
			childItem->setSelectable(false);
		}
		fileListModel->setItem(0, rootItem );
	}
	return true;
}


