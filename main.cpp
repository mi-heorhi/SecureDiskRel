#include "securedisk.h"
#include <QtWidgets/QApplication>

#include <cryptocpp\files.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SecureDisk w;
	w.show();
	return a.exec();
}
