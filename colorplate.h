#ifndef COLORPLATE_H
#define COLORPLATE_H

#include <QWidget>
#include <QList>
#include <QStandardItemModel>
#include "ui_colorplate.h"
#include <QComboBox>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QSignalMapper>

class TiffDataRead;
class ColorThread;
class GeoSimulator;
class DynaSimulation;

class ColorPlate : public QWidget
{
	Q_OBJECT

public:

	ColorPlate(QWidget *parent,DynaSimulation* _dsl);
	ColorPlate(QWidget *parent,GeoSimulator* _gsr,bool patten);  // ����
	~ColorPlate();

public slots:
	bool showCountlist(); // ģ��
	bool probabilityShowCountlist(); // ����
	void setInvalidValue(QString position); // ģ��
	void resetInvalidValue(QString position);// ����
	void setColor(QString position); // ģ��
	void finished(); // ģ��
	void refinished();// ����
	void buildconfigfile(); // ģ��
	void loadconfigfile(); // ģ��
	void closeWin(); // ͨ��

protected:
	QList<QComboBox* > L_comBox;
	QList<QPushButton*> L_btn;
	
private:
	Ui::ColorPlate ui;
	GeoSimulator* m_gslr;
	DynaSimulation* m_dsl;
};


#endif // COLORPLATE_H
