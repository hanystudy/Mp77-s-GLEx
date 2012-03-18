#include "glex.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GLEx w;
	w.show();
	return a.exec();
}
