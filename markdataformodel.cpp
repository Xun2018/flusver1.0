#include "markdataformodel.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDebug>
#include <iostream>
using namespace std;

MarkDataForModel::MarkDataForModel(QWidget *parent)
	: QWidget(parent)
{

}
/// <summary>
/// <���ع��캯��>
/// </summary>
MarkDataForModel::MarkDataForModel( QList<TiffDataRead*> _inUi_poDataset, int _serialnum )
{
	// <����ʼ��>
	ui.setupUi(this);
	ui.pro_mrkTree->setModel(ui.pro_mrkTree->FileListModel());/// <��ʼ���ļ��б�>
	ui.pro_mrkTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.pro_mrkTree->setContextMenuPolicy(Qt::CustomContextMenu);
	//ui.pro_mrkTree->setStyleSheet("");
	ui.pro_mrkTree->getOtherListModel(_inUi_poDataset,_serialnum);

	// <�ұ�1>
	_fileLauList=new QStandardItemModel;
	ui.listVlau->setModel(this->_fileLauList);
	// <�ұ�2>
	_fileMskList=new QStandardItemModel(this);
	ui.listVmsk->setModel(this->_fileMskList);
	// <�ұ�3>
	_fileDivList=new QStandardItemModel(this);
	ui.listVdiv->setModel(this->_fileDivList);
	// <�ұ�4>
	_fileConList=new QStandardItemModel(this);
	ui.listVcon->setModel(this->_fileConList);
	// <�ұ�5>
	_fileAjtList=new QStandardItemModel(this);
	ui.listVajt->setModel(this->_fileAjtList);


	this->allInputBtnEnable(false);
	this->judgeListViewNull();

	// <���½ǰ�ť>
	connect(ui.cancelButton,SIGNAL(clicked()),this,SLOT(winClose()));
	connect(ui.nextButton,SIGNAL(clicked()),this,SLOT(whenfinishclicked()));

	connect(ui.pro_mrkTree,SIGNAL(clicked(const QModelIndex &)),this,SLOT(move2TextInitial()));
	connect(ui.listVlau,SIGNAL(clicked(const QModelIndex &)),this,SLOT(lauItem2TreeIni()));
	connect(ui.listVmsk,SIGNAL(clicked(const QModelIndex &)),this,SLOT(mskItem2TreeIni()));
	connect(ui.listVdiv,SIGNAL(clicked(const QModelIndex &)),this,SLOT(divItem2TreeIni()));
	connect(ui.listVcon,SIGNAL(clicked(const QModelIndex &)),this,SLOT(conItem2TreeIni()));
	connect(ui.listVajt,SIGNAL(clicked(const QModelIndex &)),this,SLOT(ajtItem2TreeIni()));

	connect(ui.raBtnEmpty,SIGNAL(toggled(bool)),this,SLOT(radioConAjtGropAble()));// <ѡ��nodata�󲻿���>
	connect(ui.raBtnEmpty_2,SIGNAL(toggled(bool)),this,SLOT(radioConAjtGropAble()));// <ѡ��nodata�󲻿���>
	connect(ui.lauBtnIn,SIGNAL(clicked()),this,SLOT(move2TextlauToItem()));
	connect(ui.mskBtnIn,SIGNAL(clicked()),this,SLOT(move2TextmskToItem()));
	connect(ui.divBtnIn,SIGNAL(clicked()),this,SLOT(move2TextdivToItem()));
	connect(ui.conBtnIn,SIGNAL(clicked()),this,SLOT(move2TextconToItem()));
	connect(ui.ajtBtnIn,SIGNAL(clicked()),this,SLOT(move2TextajtToItem()));
	connect(ui.lauBtnOut,SIGNAL(clicked()),this,SLOT(lauItem2Tree()));
	connect(ui.mskBtnOut,SIGNAL(clicked()),this,SLOT(mskItem2Tree()));
	connect(ui.divBtnOut,SIGNAL(clicked()),this,SLOT(divItem2Tree()));
	connect(ui.conBtnOut,SIGNAL(clicked()),this,SLOT(conItem2Tree()));
	connect(ui.ajtBtnOut,SIGNAL(clicked()),this,SLOT(ajtItem2Tree()));
}

MarkDataForModel::~MarkDataForModel()
{
	this->winClose();
}



/// <summary>
/// <�������밴ť�������û�����>
/// </summary>
void MarkDataForModel::allInputBtnEnable( bool _btnendable )
{
	ui.lauBtnIn->setEnabled(_btnendable);
	ui.mskBtnIn->setEnabled(_btnendable);
	ui.divBtnIn->setEnabled(_btnendable);
	ui.conBtnIn->setEnabled(_btnendable);
	ui.ajtBtnIn->setEnabled(_btnendable);
	ui.raBtnInput->setChecked(true);
	ui.raBtnInput_2->setChecked(true);

	if (!_fileLauList->rowCount()<1)
	{
		ui.lauBtnIn->setEnabled(false);
	}
	if (!_fileMskList->rowCount()<1)
	{
		ui.mskBtnIn->setEnabled(false);
	}
}
/// <summary>
/// <ѡNodata,Con��Adj������>
/// </summary>
void MarkDataForModel::radioConAjtGropAble()
{
	if (ui.raBtnEmpty->isChecked())  
	{  
		if (_fileConList->rowCount()!=0)
		{
			// <����������>
			for (int kk=0;kk<_fileConList->rowCount();kk++)
			{
				for (int ii=0;ii<ui.pro_mrkTree->FileListModel()->rowCount();ii++)
				{
					if (0 == QString::compare(this->_fileConList->item(kk,0)->text(),ui.pro_mrkTree->FileListModel()->item(ii,0)->text(), Qt::CaseInsensitive))
					{
						ui.pro_mrkTree->FileListModel()->item(ii,0)->setForeground(QBrush(QColor(0, 0, 0)));
						this->_fileConList->removeRow(kk);
						for (int jj=0;jj<beMarkedNbrs.size();jj++)
						{
							if (beMarkedNbrs[jj]==ii)
							{
								beMarkedNbrs.removeAt(jj);
							}
						}
						continue;
					}
				}
			}
		}
		ui.grupBoxCon->setEnabled(false);
	}   
	if (ui.raBtnEmpty_2->isChecked())  
	{  
		if (_fileAjtList->rowCount()!=0)
		{
			// <����������>
			for (int kk=0;kk<_fileAjtList->rowCount();kk++)
			{
				for (int ii=0;ii<ui.pro_mrkTree->FileListModel()->rowCount();ii++)
				{
					if (0 == QString::compare(this->_fileAjtList->item(kk,0)->text(),ui.pro_mrkTree->FileListModel()->item(ii,0)->text(), Qt::CaseInsensitive))
					{
						ui.pro_mrkTree->FileListModel()->item(ii,0)->setForeground(QBrush(QColor(0, 0, 0)));
						this->_fileAjtList->removeRow(kk);
						for (int jj=0;jj<beMarkedNbrs.size();jj++)
						{
							if (beMarkedNbrs[jj]==ii)
							{
								beMarkedNbrs.removeAt(jj);
							}
						}
						continue;
					}
				}
			}
		}
		ui.grupBoxAjt->setEnabled(false);
	} 
	judgeListViewNull();
}
/// <summary>
/// <�ж��ұ��ı��Ƿ�Ϊ��>
/// </summary>
void MarkDataForModel::judgeListViewNull()
{
	// <���������ı����������ť����>
	if (_fileLauList->rowCount()==0)
	{
		ui.lauBtnOut->setEnabled(false);
	}
	// <��ģ�ı����������ť����>
	if (_fileMskList->rowCount()==0)
	{
		ui.mskBtnOut->setEnabled(false);
	}
	// <�����������ı����������ť����>
	if (_fileDivList->rowCount()==0)
	{
		ui.divBtnOut->setEnabled(false);
	}
	// <�����������ı����������ť����>
	if (_fileConList->rowCount()==0)
	{
		ui.conBtnOut->setEnabled(false);
	}
	// <������������ı����������ť����>
	if (_fileAjtList->rowCount()==0)
	{
		ui.ajtBtnOut->setEnabled(false);
	}
	// <�����û������ʱ>
	if (beMarkedNbrs.size()==ui.pro_mrkTree->FileListModel()->rowCount())
	{
		this->allInputBtnEnable(false);
	}
	// <next ������������ʱ��ע�͵������Է���>
// 	if (_fileLauList->rowCount()<1||_fileMskList->rowCount()<1||_fileDivList->rowCount()<1)
// 	{
// 		ui.nextButton->setEnabled(false);
// 	}
// 	else
// 	{
// 		ui.nextButton->setEnabled(true);
// 	}
}
/// <summary>
/// <�ж��Ƿ�ѡ���Ѿ�ѡ��������>
/// </summary>
bool MarkDataForModel::judgeIfSelect(int _selectSrl)
{
	for (int ii=0;ii<beMarkedNbrs.size();ii++)
	{
		if (_selectSrl==beMarkedNbrs[ii])
		{
			QMessageBox::about(NULL,"Title","The image has been marked, please choose other images!");
			return false;
		}
	}
	return true;
}
/// <summary>
/// <�����ƶ����ݰ�ť��ʼ��>
/// </summary>
void MarkDataForModel::move2TextInitial()
{
	allInputBtnEnable(true);
	judgeListViewNull();
}
/// <summary>
/// <���������������>
/// </summary>
void MarkDataForModel::move2TextlauToItem()
{
	// <������������>
	index=ui.pro_mrkTree->currentIndex();
	if (ui.pro_mrkTree->FileListModel()->item(index.row(),0)->rowCount()!=1)
	{
		QMessageBox::about(NULL,"Title","land use image must has only 1 band!");
		return;
	}
	bool _selecteditem=judgeIfSelect(index.row());
	if (_selecteditem==true)
	{
		lauSrlnum=index.row();
		beMarkedNbrs.append(lauSrlnum);

		_fileNode=new QStandardItem(ui.pro_mrkTree->FileListModel()->item(lauSrlnum,0)->text());
		_fileLauList->appendRow(_fileNode);
		_fileLauList->setItem(_fileLauList->rowCount()-1, _fileNode);

		ui.pro_mrkTree->FileListModel()->item(lauSrlnum,0)->setForeground(QBrush(QColor(255, 255, 255)));
		ui.pro_mrkTree->FileListModel()->item(lauSrlnum,0)->setSelectable(false);
	}
	this->allInputBtnEnable(false);// <��֪��Ҫ��Ҫע����������˳��>
	this->judgeListViewNull();
}
/// <summary>
/// <�����ģ����>
/// </summary>
void MarkDataForModel::move2TextmskToItem()
{
	// <��ģ����>
	index=ui.pro_mrkTree->currentIndex();
	if (ui.pro_mrkTree->FileListModel()->item(index.row(),0)->rowCount()!=1)
	{
		QMessageBox::about(NULL,"Title","mask image must has only 1 band!");
		return;
	}
	bool _selecteditem=judgeIfSelect(index.row());
	if (_selecteditem==true)
	{
		mskSrlnum=index.row();
		beMarkedNbrs.append(mskSrlnum);

		_fileNode=new QStandardItem(ui.pro_mrkTree->FileListModel()->item(mskSrlnum,0)->text());
		_fileMskList->appendRow(_fileNode);
		_fileMskList->setItem(_fileMskList->rowCount()-1, _fileNode);

		ui.pro_mrkTree->FileListModel()->item(mskSrlnum,0)->setForeground(QBrush(QColor(255, 255, 255)));
		ui.pro_mrkTree->FileListModel()->item(mskSrlnum,0)->setSelectable(false);
	}
	this->allInputBtnEnable(false);
	this->judgeListViewNull();
}
/// <summary>
/// <�������������>
/// </summary>
void MarkDataForModel::move2TextdivToItem()
{
	// <����������>
	index=ui.pro_mrkTree->currentIndex();
	bool _selecteditem=judgeIfSelect(index.row());
	if (_selecteditem==true)
	{
		beMarkedNbrs.append(index.row());
		divSrlnum.append(index.row());
		int _whichnum=divSrlnum[divSrlnum.size()-1];

		_fileNode=new QStandardItem(ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->text());
		_fileDivList->appendRow(_fileNode);
		_fileDivList->setItem(_fileDivList->rowCount()-1, _fileNode);

		ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->setForeground(QBrush(QColor(255, 255, 255)));
		ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->setSelectable(false);
	}
	this->allInputBtnEnable(false);
	this->judgeListViewNull();
}
/// <summary>
/// <��Ǳ���������>
/// </summary>
void MarkDataForModel::move2TextconToItem()
{
	// <����������>
	index=ui.pro_mrkTree->currentIndex();
	bool _selecteditem=judgeIfSelect(index.row());
	if (_selecteditem==true)
	{
		beMarkedNbrs.append(index.row());
		conSrlnum.append(index.row());
		int _whichnum=conSrlnum[conSrlnum.size()-1];

		_fileNode=new QStandardItem(ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->text());
		_fileConList->appendRow(_fileNode);
		_fileConList->setItem(_fileConList->rowCount()-1, _fileNode);

		ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->setForeground(QBrush(QColor(255, 255, 255)));
		ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->setSelectable(false);
	}
	this->allInputBtnEnable(false);
	this->judgeListViewNull();
}
/// <summary>
/// <��Ǹ��ʵ���������>
/// </summary>
void MarkDataForModel::move2TextajtToItem()
{
	// <����������>
	index=ui.pro_mrkTree->currentIndex();
	bool _selecteditem=judgeIfSelect(index.row());
	if (_selecteditem==true)
	{
		beMarkedNbrs.append(index.row());
		ajtSrlnum.append(index.row());
		int _whichnum=ajtSrlnum[ajtSrlnum.size()-1];

		_fileNode=new QStandardItem(ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->text());
		_fileAjtList->appendRow(_fileNode);
		_fileAjtList->setItem(_fileAjtList->rowCount()-1, _fileNode);

		ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->setForeground(QBrush(QColor(255, 255, 255)));
		ui.pro_mrkTree->FileListModel()->item(_whichnum,0)->setSelectable(false);
	}
	this->allInputBtnEnable(false);
	this->judgeListViewNull();
}
/// <summary>
/// <��ʼ�����ǰ�ťlau>
/// </summary>
void MarkDataForModel::lauItem2TreeIni()
{
	ui.lauBtnOut->setEnabled(true);
}
/// <summary>
/// <��ʼ�����ǰ�ťmsk>
/// </summary>
void MarkDataForModel::mskItem2TreeIni()
{
	ui.mskBtnOut->setEnabled(true);
}
/// <summary>
/// <��ʼ�����ǰ�ťdiv>
/// </summary>
void MarkDataForModel::divItem2TreeIni()
{
	ui.divBtnOut->setEnabled(true);
}
/// <summary>
/// <��ʼ�����ǰ�ťcon>
/// </summary>
void MarkDataForModel::conItem2TreeIni()
{
	ui.conBtnOut->setEnabled(true);
}
/// <summary>
/// <��ʼ�����ǰ�ťajt>
/// </summary>
void MarkDataForModel::ajtItem2TreeIni()
{
	ui.ajtBtnOut->setEnabled(true);
}
/// <summary>
/// <����������������>
/// </summary>
void MarkDataForModel::lauItem2Tree()
{
	// <������������>
	index=ui.listVlau->currentIndex();
	int _lau=index.row();
	for (int ii=0;ii<ui.pro_mrkTree->FileListModel()->rowCount();ii++)
	{
		if (0 == QString::compare(this->_fileLauList->item(_lau,0)->text(),ui.pro_mrkTree->FileListModel()->item(ii,0)->text(), Qt::CaseInsensitive))
		{
			ui.pro_mrkTree->FileListModel()->item(ii,0)->setForeground(QBrush(QColor(0, 0, 0)));
			this->_fileLauList->removeRow(_lau);
			for (int jj=0;jj<beMarkedNbrs.size();jj++)
			{
				if (beMarkedNbrs[jj]==ii)
				{
					beMarkedNbrs.removeAt(jj);
				}
			}
			break;
		}
	}
	judgeListViewNull();
}
/// <summary>
/// <������ģ����>
/// </summary>
void MarkDataForModel::mskItem2Tree()
{
	// <��ģ����>
	index=ui.listVmsk->currentIndex();
	int _msk=index.row();
	for (int ii=0;ii<ui.pro_mrkTree->FileListModel()->rowCount();ii++)
	{
		if (0 == QString::compare(this->_fileMskList->item(_msk,0)->text(),ui.pro_mrkTree->FileListModel()->item(ii,0)->text(), Qt::CaseInsensitive))
		{
			ui.pro_mrkTree->FileListModel()->item(ii,0)->setForeground(QBrush(QColor(0, 0, 0)));
			this->_fileMskList->removeRow(_msk);
			for (int jj=0;jj<beMarkedNbrs.size();jj++)
			{
				if (beMarkedNbrs[jj]==ii)
				{
					beMarkedNbrs.removeAt(jj);
				}
			}
			break;
		}
	}
	judgeListViewNull();
}
/// <summary>
/// <��������������>
/// </summary>
void MarkDataForModel::divItem2Tree()
{
	// <����������>
	index=ui.listVdiv->currentIndex();
	int _div=index.row();
	for (int ii=0;ii<ui.pro_mrkTree->FileListModel()->rowCount();ii++)
	{
		if (0 == QString::compare(this->_fileDivList->item(_div,0)->text(),ui.pro_mrkTree->FileListModel()->item(ii,0)->text(), Qt::CaseInsensitive))
		{
			ui.pro_mrkTree->FileListModel()->item(ii,0)->setForeground(QBrush(QColor(0, 0, 0)));
			this->_fileDivList->removeRow(_div);
			for (int jj=0;jj<beMarkedNbrs.size();jj++)
			{
				if (beMarkedNbrs[jj]==ii)
				{
					beMarkedNbrs.removeAt(jj);
				}
			}
			break;
		}
	}
	judgeListViewNull();
}
/// <summary>
/// <���Ǳ���������>
/// </summary>
void MarkDataForModel::conItem2Tree()
{
	// <����������>
	index=ui.listVcon->currentIndex();
	int _con=index.row();
	for (int ii=0;ii<ui.pro_mrkTree->FileListModel()->rowCount();ii++)
	{
		if (0 == QString::compare(this->_fileConList->item(_con,0)->text(),ui.pro_mrkTree->FileListModel()->item(ii,0)->text(), Qt::CaseInsensitive))
		{
			ui.pro_mrkTree->FileListModel()->item(ii,0)->setForeground(QBrush(QColor(0, 0, 0)));
			this->_fileConList->removeRow(_con);
			for (int jj=0;jj<beMarkedNbrs.size();jj++)
			{
				if (beMarkedNbrs[jj]==ii)
				{
					beMarkedNbrs.removeAt(jj);
				}
			}
			break;
		}
	}
	judgeListViewNull();
}
/// <summary>
/// <���Ǹ��ʵ���������>
/// </summary>
void MarkDataForModel::ajtItem2Tree()
{
	// <���ʵ���������>
	index=ui.listVajt->currentIndex();
	int _ajt=index.row();
	for (int ii=0;ii<ui.pro_mrkTree->FileListModel()->rowCount();ii++)
	{
		if (0 == QString::compare(this->_fileAjtList->item(_ajt,0)->text(),ui.pro_mrkTree->FileListModel()->item(ii,0)->text(), Qt::CaseInsensitive))
		{
			ui.pro_mrkTree->FileListModel()->item(ii,0)->setForeground(QBrush(QColor(0, 0, 0)));
			this->_fileAjtList->removeRow(_ajt);
			for (int jj=0;jj<beMarkedNbrs.size();jj++)
			{
				if (beMarkedNbrs[jj]==ii)
				{
					beMarkedNbrs.removeAt(jj);
				}
			}
			break;
		}
	}
	judgeListViewNull();
}
/// <summary>
/// <�رմ���>
/// </summary>
void MarkDataForModel::winClose()
{
	this->close();
}
/// <summary>
/// <���ر������>
/// </summary>
void MarkDataForModel::whenfinishclicked()
{
	emit sandallmarkdata(lauSrlnum,mskSrlnum,divSrlnum,conSrlnum,ajtSrlnum);
	this->winClose();
}






















