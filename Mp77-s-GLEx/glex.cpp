#include "glex.h"
#include <QHboxLayout>


GLEx::GLEx(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	mainview = new GLExMainView(this);

	mainview->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(mainview);
	Qt::Alignment ali = Qt::AlignLeft | Qt::AlignTop;
	mainLayout->setAlignment( ali );
	mainLayout->setMargin(0);
	this->centralWidget()->setLayout(mainLayout);
}

GLEx::~GLEx()
{

}
