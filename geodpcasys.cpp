#include "geodpcasys.h"
#include "showproperty.h"
#include <iostream>
#include <QtGui/QFileDialog>
#include <QApplication>
#include <QLocale>
#include <QLibraryInfo>
#include <QDesktopServices>
#include "mapshowviewer.h"
#include "dynasimulation.h"
#include "geosimulator.h"
#include "kappacalculate.h"
#include "predictionform.h"
#include "med.h"

using namespace std;

GeoDpCAsys::GeoDpCAsys(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	/// <ʱ������>
	ui.setupUi(this);

	this->setWindowTitle("FLUS/UGB-FLUS V2.3 - Sun Yat-sen University (SYSU)");

	imgProperty=NULL;
	geosim=NULL;
	dynasim=NULL;

	ifPickFile=false;
	serialNum=0;
	child_serialNum=0;
	NumImage=0;

	QIcon qicon(":/new/prefix1/�д�.png");
	this->setWindowIcon(qicon);

	QIcon openicon(":/new/prefix1/��.png");
	this->ui.actionOpen->setIcon(openicon);

	QIcon clearicon(":/new/prefix1/���.png");
	this->ui.actionClear->setIcon(clearicon);

	QIcon probicon(":/new/prefix1/����.png");
	this->ui.actionGeo_Simulation->setIcon(probicon);

	QIcon neibicon(":/new/prefix1/����.png");
	this->ui.actionDynamic_Simulation->setIcon(neibicon);

	QIcon qicona(":/new/prefix1/����.png");
	ui.actionAbout->setIcon(qicona);

	QIcon qiconZi(":/new/prefix1/zoomIn.png");
	ui.actionZoom_In->setIcon(qiconZi);

	QIcon qiconZo(":/new/prefix1/zoomOut.png");
	ui.actionZoom_Out->setIcon(qiconZo);

	QIcon qiconclose(":/new/prefix1/�ر�.png");
	ui.actionExit->setIcon(qiconclose);

	QIcon qiconfe(":/new/prefix1/��ԭ.png");
	ui.actionFull_Extent->setIcon(qiconfe);

	QIcon qiconume(":/new/prefix1/pdf.png");
	ui.actionUser_Guide_English->setIcon(qiconume);

	QIcon qiconumc(":/new/prefix1/pdf.png");
	ui.actionUser_Guide_Chinese->setIcon(qiconumc);

	QIcon prebicon(":/new/prefix1/Ԥ��.png");
	ui.actionMarkov_chain->setIcon(prebicon);

	ui.pro_tView->setModel(ui.pro_tView->FileListModel());/// <��ʼ���ļ��б�>
	ui.pro_tView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.pro_tView->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.actionOpen,SIGNAL(triggered()),this,SLOT(pickOpenFile()));
	connect(ui.actionClear,SIGNAL(triggered()),this,SLOT(clearAll()));
	connect(ui.pro_tView,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(showMouseRightMenu(const QPoint &)));	/// <չʾ�˵����¼�Ӧ��Ϊ�Ҽ�����>
	connect(ui.rCBox,SIGNAL(activated(int)),this,SLOT(comboBoxChange()));
	connect(ui.gCBox,SIGNAL(activated(int)),this,SLOT(comboBoxChange()));
	connect(ui.bCBox,SIGNAL(activated(int)),this,SLOT(comboBoxChange()));
	connect(ui.actionGeo_Simulation,SIGNAL(triggered()),this,SLOT(geosimulatorstart()));
	connect(ui.actionDynamic_Simulation,SIGNAL(triggered()),this,SLOT(dynasimulationprocess()));
	connect(ui.actionAbout,SIGNAL(triggered()),this,SLOT(infomation()));
	connect(ui.actionKappa, SIGNAL( triggered() ), this, SLOT(kappacoefficient()));

	connect( ui.actionZoom_In, SIGNAL( triggered() ), this->ui.pro_GView, SLOT( ZoomIn() ) );
	connect( ui.actionZoom_Out, SIGNAL( triggered() ), this->ui.pro_GView, SLOT( ZoomOut() ) );
	connect( ui.actionFull_Extent, SIGNAL( triggered() ), this->ui.pro_GView, SLOT( ZoomFit() ) );
	connect( ui.actionExit, SIGNAL( triggered() ), this, SLOT(close()) ); 
	connect( ui.actionNew_Version,SIGNAL( triggered() ),this,SLOT(openUrl()));
	connect( ui.actionUser_Guide_English,SIGNAL( triggered() ),this,SLOT(openUrlusermanualen()));
	connect( ui.actionUser_Guide_Chinese,SIGNAL( triggered() ),this,SLOT(openUrluserManualchs()));
	connect( ui.actionMarkov_chain,SIGNAL( triggered() ),this,SLOT(openMarkov()));
	connect( ui.actionMorphological_method,SIGNAL( triggered() ),this,SLOT(openMED()));

	QDateTime dt;  
	QTime time;  
	QDate date;  
	dt.setTime(time.currentTime());  
	dt.setDate(date.currentDate());  
	QString currentDate = dt.toString("yyyy:MM:dd:hh:mm:ss"); 
	QStringList smsglist = currentDate.split(":");

	double _year = smsglist[0].trimmed().toDouble();
	double _month = smsglist[1].trimmed().toDouble();
	double _day = smsglist[2].trimmed().toDouble();
	
	int deadline_year=2016;
	int deadline_month=12;
	int deadline_day=31;

	//if (_year>deadline_year||_month>deadline_month||_day>deadline_day)
	//{
	//	this->infomation();
	//	ui.menuBar->setEnabled(false);
	//	this->close();
	//	this->~GeoDpCAsys();
	//}
	/// <ʱ������>
	//ui.actionMorphological_method->setVisible(false); // ��ӡ����

}

GeoDpCAsys::~GeoDpCAsys()
{
	this->clearAll();
	this->close();
}

/**************************Image Scan**************************/
/// <summary>
/// <����Ӱ������>
/// </summary>
bool GeoDpCAsys::loadImage( const char* _fileName )
{
	//register
	GDALAllRegister();
	//OGRRegisterAll();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	serialNum=NumImage;

	// <�½�IO��>
	TiffDataRead* pread = new TiffDataRead;

	// <����>
	Ui_poDataset.append(pread);

	// <��ȡ����>
	if (!Ui_poDataset[serialNum]->loadFrom(_fileName))
	{
		cout<<"load error!"<<endl;
	}
	else
	{
		cout<<"load success!"<<endl;
	}

	NumImage=Ui_poDataset.size();// <���ڼ�¼�ܲ�����>
	return true;
}
/// <summary>
/// <ѡ��Ӱ���ļ�>
/// </summary>
void GeoDpCAsys::pickOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(
		this,
		tr( "Pick a image file to open..." ),
		QDir::currentPath(),
		tr( "tiff(*.tif);;jpg(*.jpg);;img(*.img);;All files(*.*)" ));
	if ( !fileName.isNull() )
	{
		this->loadImage(fileName.toStdString().c_str());
		ifPickFile=true;
		ui.pro_tView->getBandList(Ui_poDataset,serialNum);
		ui.pro_tView->expandAll();// <չ��>
		this->showImageOrBand();
	}
}
/// <summary>
/// <�Ҽ��¼���>
/// </summary>
void GeoDpCAsys::showMouseRightMenu(const QPoint &pos)
{
	// <��ȡ���ڵ���>
	QModelIndex FaChindex = ui.pro_tView->indexAt(pos); 
	if (FaChindex.parent()==QModelIndex())
	{
		this->serialNum=FaChindex.row();  // <��ȡ���ڵ���>
		child_serialNum=0;  // <ѡ�и��׽ڵ㣬�ӽڵ㲻�Զ���1>
		// <��¼Ҫ��ʾ��RGB���>
		int a;
		a=Ui_poDataset.size();
		if (serialNum>0&&Ui_poDataset[serialNum]->bandnum()>=3)
		{
			ischecked.clear();
			for (int ii=0;ii<3;ii++)
			{
				ischecked.append(ii);
			}
		}
		else
		{
			ischecked.clear();
			for (int ii=0;ii<3;ii++)
			{
				ischecked.append(child_serialNum);
			}
		}
	}
	if (FaChindex.parent()!=QModelIndex())
	{
		this->child_serialNum=FaChindex.row();
		serialNum=FaChindex.parent().row();// <��ȡ�ӽڵ��������ڵ���>
		ischecked.clear();
		for (int ii=0;ii<3;ii++)
		{
			ischecked.append(child_serialNum);
		}
	}
	
	// <��ȡ�ļ���>
	QString fileName = FaChindex.data().toString();
	// <�²˵�>
	QMenu *qMenu = NULL;
	if (qMenu)
	{
		delete qMenu;
		qMenu = NULL;
	}                                   
	qMenu = new QMenu(ui.pro_tView);

	/// <���Ǹ��ڵ���ִ��>
	if (fileName.isNull()==false&&FaChindex.parent()!=QModelIndex())
	{
		ifPickFile=false;
		QAction* showBandAction = new QAction(tr("Show Gray-Scale Map").arg(serialNum).arg(child_serialNum), this);
		connect(showBandAction, SIGNAL(triggered()), this, SLOT(showImageOrBand()));
		qMenu->addAction(showBandAction);

		// ��ӡ����
// 		ifPickFile=false;
// 		QAction* changeColorBar = new QAction(tr("Change Color Bar").arg(serialNum), this);
// 		connect(changeColorBar, SIGNAL(triggered()), this->ui.pro_GView, SLOT(changecolorbar()));
// 		qMenu->addAction(changeColorBar);
	}
	/// <���ڵ���ִ��>
	if(fileName.isNull()==false&&FaChindex.parent()==QModelIndex())
	{
		if (Ui_poDataset[serialNum]->bandnum()<3)//&&Ui_poDataset[serialNum]->bandnum()!=2)
		{
			ifPickFile=false;
			QAction* showGRAYAction = new QAction(tr("Show Gray-Scale Map").arg(serialNum), this);
			connect(showGRAYAction, SIGNAL(triggered()), this, SLOT(showImageOrBand()));
			qMenu->addAction(showGRAYAction);

			// ��ӡ����
// 			ifPickFile=false;
// 			QAction* changeColorBar = new QAction(tr("Change Color Bar").arg(serialNum), this);
// 			connect(changeColorBar, SIGNAL(triggered()), this->ui.pro_GView, SLOT(changecolorbar()));
// 			qMenu->addAction(changeColorBar);
		}
		else
		{
			ifPickFile=true;
			QAction* showRGBAction = new QAction(tr("Show RGB Map ").arg(serialNum), this);
			connect(showRGBAction, SIGNAL(triggered()), this, SLOT(showImageOrBand()));
			qMenu->addAction(showRGBAction);

		}

		QAction* closeChoosenAction = new QAction(tr("&Close"),this);
		connect(closeChoosenAction, SIGNAL(triggered()),this, SLOT(closeChoosenData()));
		qMenu->addAction(closeChoosenAction);

		// ��ӡ����
// 		QAction* addTreeItemAction = new QAction(tr("&Add Item"), this);
// 		connect(addTreeItemAction, SIGNAL(triggered()), this, SLOT(pickOpenFile()));
// 		qMenu->addAction(addTreeItemAction);

		QAction* _imgProperty = new QAction(tr("&Image Property"),this);
		connect(_imgProperty, SIGNAL(triggered()), this, SLOT(showImgProperty()));
		qMenu->addAction(_imgProperty);
	}
	qMenu->exec(QCursor::pos()); /// <���������λ����ʾ����Ҽ��˵�>
}
/// <summary>
/// <��ʾӰ��>
/// </summary>
void GeoDpCAsys::showImageOrBand()
{
	this->comboBoxAdd();
	this->setCheck();
	ui.pro_GView->chooseImageToshow(Ui_poDataset,serialNum,ischecked);
}
/// <summary>
/// <�ر���ѡӰ��>
/// </summary>
void GeoDpCAsys::closeChoosenData()
{
	ifPickFile=true;
	ui.rCBox->clear();
	ui.gCBox->clear();
	ui.bCBox->clear();
	ui.pro_tView->FileListModel()->removeRow(serialNum);
	Ui_poDataset[serialNum]->close();
	Ui_poDataset.removeAt(serialNum);
	NumImage=Ui_poDataset.size();
	if (serialNum-1<0)
	{
		serialNum=NumImage-1; // <��һ����ɾ��ʾ���һ��>
	}
	else
	{
		serialNum=serialNum-1; // <��ɾ��ʾ��һ��>
	}
	if (NumImage!=0)
	{
		this->showImageOrBand();
	}
	else
	{
		ui.pro_GView->deleteForRenew(); // <�޲������>
		ui.bandOpBox->setTitle(tr("RGB"));
	}
}
/// <summary>
/// <��ʾ���Դ���>
/// </summary>
void GeoDpCAsys::showImgProperty()
{
	imgProperty=new ShowProperty(Ui_poDataset,serialNum);// <���ع��캯������>
	imgProperty->show();
}
/// <summary>
/// <��������˵�>
/// </summary>
void GeoDpCAsys::comboBoxAdd()
{
	ui.rCBox->clear();
	ui.gCBox->clear();
	ui.bCBox->clear();
	if (Ui_poDataset.size()!= 0)
	{
		QFileInfo fileInfo(Ui_poDataset[serialNum]->getFileName());
		ui.bandOpBox->setTitle(tr(fileInfo.fileName().toStdString().c_str()));
		for ( int i = 0; i <Ui_poDataset[serialNum]->poDataset()->GetRasterCount(); i++ )
		{
			ui.rCBox->addItem(QWidget::tr("Band %1").arg( i + 1 ));
			ui.gCBox->addItem(QWidget::tr("Band %1").arg( i + 1 ));
			ui.bCBox->addItem(QWidget::tr("Band %1").arg( i + 1 ));
		}
		if (ifPickFile==true)
		{
			if (Ui_poDataset[serialNum]->bandnum()>=3)
			{
				ischecked.clear();
				for (int ii=0;ii<3;ii++)
				{
					ischecked.append(ii);
				}
				child_serialNum=0; // <RGB��ʾʱ���ӽڵ��0>
			}
			else
			{
				ischecked.clear();
				for (int ii=0;ii<3;ii++)
				{
					ischecked.append(child_serialNum);
				}
			}
			ifPickFile=false;
		}
		this->setGroupName();
		ui.rCBox->setCurrentIndex(ischecked[0]);
		ui.gCBox->setCurrentIndex(ischecked[1]);
		ui.bCBox->setCurrentIndex(ischecked[2]);
		child_serialNum=0; // <RGB��ʾʱ���ӽڵ��0>
	}
}
/// <summary>
/// <��ȡ���>
/// </summary>
void GeoDpCAsys::comboBoxChange()
{
	ischecked.clear();
	ischecked.append(ui.rCBox->currentIndex());
	ischecked.append(ui.gCBox->currentIndex());
	ischecked.append(ui.bCBox->currentIndex());
	this->setCheck();
	this->setGroupName();
	ui.pro_GView->chooseImageToshow(Ui_poDataset,serialNum,ischecked);
	child_serialNum=0; // <RGB��ʾʱ���ӽڵ��0��Ϊ�´ε���������׼��>
}
/// <summary>
/// <���õ�ǰ��ʾΪchecked>
/// </summary>
void GeoDpCAsys::setCheck()
{
	for (int ii=0;ii<ui.pro_tView->FileListModel()->rowCount();ii++)
	{
		if (ii==serialNum)
		{
			//ui.pro_tView->FileListModel()->item(ii,0)->setCheckState(Qt::Checked);
			QFont font;    
			font.setBold(true);
			ui.pro_tView->FileListModel()->item(ii,0)->setFont(font);
		}
		else
		{
			//ui.pro_tView->FileListModel()->item(ii,0)->setCheckState(Qt::Unchecked);
			QFont font;    
			font.setBold(false);
			ui.pro_tView->FileListModel()->item(ii,0)->setFont(font);
		}
		//ui.pro_tView->FileListModel()->item(ii,0)->setCheckable(false);
	}
}
/// <summary>
/// <���õ�ǰ��ʾ�����>
/// </summary>
void GeoDpCAsys::setGroupName()
{
	QFileInfo fileInfo(Ui_poDataset[serialNum]->getFileName());
	if (ischecked[0]==ischecked[1]&&ischecked[1]==ischecked[2])
	{
		ui.bandOpBox->setTitle(tr(fileInfo.fileName().toStdString().c_str())+tr(" -> Band %1").arg(ischecked[0]+1));
	}
	else
	{
		ui.bandOpBox->setTitle(tr(fileInfo.fileName().toStdString().c_str()));
	}
}
/// <summary>
/// <���>
/// </summary>
void GeoDpCAsys::clearAll()
{
	ifPickFile=true;
	ui.rCBox->clear();
	ui.gCBox->clear();
	ui.bCBox->clear();
	ui.pro_tView->FileListModel()->clear();
	ui.pro_tView->FileListModel()->setHorizontalHeaderLabels(QStringList()<<"Data");
	for (int ii=0;ii<Ui_poDataset.size();ii++)
	{
		if (Ui_poDataset[ii]!=NULL)
		{
			Ui_poDataset[ii]->close();
			delete Ui_poDataset[ii];
		}
	}
	ischecked.clear();
	ui.pro_GView->deleteForRenew();
	Ui_poDataset.clear();

	//ifPickFile=false;
	serialNum=0;
	child_serialNum=0;
	NumImage=0;

}

/**************************Data Mark**************************/
/// <summary>
/// <ģ��������>
/// </summary>
void GeoDpCAsys::geosimulatorstart()
{
	geosim=new GeoSimulator(this);
	geosim->show();
}
/// <summary>
/// <�������ղ�>
/// </summary>
void GeoDpCAsys::dynasimulationprocess()
{
	dynasim=new DynaSimulation(this);
	dynasim->show();
}
/// <summary>
/// <�����Ϣ>
/// </summary>
void GeoDpCAsys::infomation()
{
	QMessageBox::about(this,tr("About GeoSOS-FLUS"),tr("GeoSOS-FLUS\nVersion: 2.3\nPublish Time: 2018/08/10\n\nInformation:\nGeoSOS-FLUS is developed by Prof. Xiaoping Liu, Prof. Xia Li and their research team, based on their studies on cellular automata (CA) and land use change. Dr. Xun Liang is responsible for the software development.\n\nLaboratory:\nGuangdong Key Laboratory for Urbanization and Geo-simulation, Sun Yat-sen University, Guangzhou 510275, PR China\n\nIf you have any ideas, suggestions, comments, criticisms, or questions, please contact Dr. Xun Liang at the following e-mail address: liangxunnice@163.com, we would love to hear from you about our model or software."));
}
/// <summary>
/// <����>
/// </summary>
void GeoDpCAsys::restart()
{
	qApp->quit();
	QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}
/// <summary>
/// <kappaϵ��>
/// </summary>
void GeoDpCAsys::kappacoefficient()
{
	kappacal=new KappaCalculate();
	kappacal->show();
}

void GeoDpCAsys::openUrl()
{
	QDesktopServices::openUrl(QUrl("http://www.geosimulation.cn/flus.html"));
}

void GeoDpCAsys::openUrluserManualchs()
{
	QDesktopServices::openUrl(QUrl("http://www.geosimulation.cn/FLUS/GeoSOS-FLUS%20Manual_CHS.pdf"));
}

void GeoDpCAsys::openUrlusermanualen()
{
	QDesktopServices::openUrl(QUrl("http://www.geosimulation.cn/FLUS/GeoSOS-FLUS%20Manual_En.pdf"));
}

void GeoDpCAsys::openMarkov()
{
	predf=new predictionform();
	predf->show();
}

void GeoDpCAsys::openMED()
{
	mMorEroDla=new med();
	mMorEroDla->show();
}


