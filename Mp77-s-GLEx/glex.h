#ifndef GLEX_H
#define GLEX_H

#include <QtGui/QMainWindow>
#include "ui_glex.h"

class GLEx : public QMainWindow
{
	Q_OBJECT

public:
	GLEx(QWidget *parent = 0, Qt::WFlags flags = 0);
	~GLEx();

private:
	Ui::GLExClass ui;
};

#endif // GLEX_H
