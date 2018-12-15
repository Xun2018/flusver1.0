#ifndef MAPSHOWVIEWER_H
#define MAPSHOWVIEWER_H

#include <QGraphicsView>
#include <QtGui>
#include <QtCore>
#include "TiffDataRead.h"

class MapShowViewer : public  QGraphicsView
{
	Q_OBJECT

public:
	MapShowViewer(QWidget *parent);
	~MapShowViewer();


public slots:
	bool chooseImageToshow(QList<TiffDataRead*> _inUi_poDataset, int _serialnumint,QList<int> _ischecked);
	bool deleteForRenew();
	void dynamicShow(unsigned char* ___u_rgbArray,unsigned char* ___u_rgbShowArray,int _rows,int _cols,double m_scaleFactor);
	void changecolorbar();
	int isfinished()
	{
		return finishedcount;
	}

private:
	template<class TT> bool imgSketch2uArray(unsigned char* buffer);
	bool styleBaseSketch(unsigned char* _buffer,GDALDataType _datatype);
	void getScalePara(bool _iniornot);
	void destoryCharPointer(unsigned char* _destroied);
    
	
private:
	QList<QGraphicsPixmapItem *> imgItemlist;
	QGraphicsPixmapItem * imgItem_child;
	QGraphicsPixmapItem *imgItem;
	QGraphicsScene *picScene;
private:
	QList<GDALRasterBand*> Gra_show;
	unsigned char* u_rgbShowArray;
	unsigned char* u_sketchBand;
	unsigned char* u_tmpArray;
	unsigned char* u_rgbArray;
	size_t cols;
	size_t rows;
	int bands;
	int perPsize;
	size_t _iScaleWidth;
	size_t _iScaleHeight;
	double noValue;
	GDALDataType _dataType;
	size_t dataLength;
	int _serialcode;

	double max;
	double min;
	double mdMiddleValue;
	double minmax[2];

/// <------------------------------------------------------�㷨�޹ز���---------------------------------------------------------->
/// <summary>
/// <����ϵ��>
/// </summary>
float m_scaleFactor;
/// <summary>
/// <��׼>
/// </summary>
float m_level;
/// <summary>
/// <ͼ������>
/// </summary>
/// <paramname="factor">��������</param>
protected:
	int finishedcount;


/// <summary>
/// <ͼ��ԭ>
/// </summary>
/// <paramname="factor">��������</param>
void ScaleImg( double factor )
{
	m_scaleFactor *= factor;
	QMatrix matrix;
	matrix.scale( m_scaleFactor,m_scaleFactor );
	this->setMatrix( matrix );
};
// 
/// <summary>
/// <��һ������¼�����ʱ����λ��>
/// </summary>
QPoint lastEventCursorPos;


public slots:
/// <summary>
/// <�Ŵ�ͼ��>
/// </summary>
	void ZoomIn()
	{
		ScaleImg( 1.2);
	};
/// <summary>
/// <��Сͼ��>
/// </summary>
	void ZoomOut()
	{
		ScaleImg(0.8);
	};

	/// <summary>
	/// <��ԭͼ��>
	/// </summary>
	void ZoomFit()
	{
		double _scale;
		_scale=m_level/m_scaleFactor;
		ScaleImg(_scale);
		m_scaleFactor=m_level;
	};

protected:
	void wheelEvent( QWheelEvent *event );
	void mousePressEvent( QMouseEvent *event );
	void mouseMoveEvent( QMouseEvent *event );
	void mouseReleaseEvent( QMouseEvent *event);

};

#endif // MAPSHOWVIEWER_H
