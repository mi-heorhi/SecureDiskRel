#include "securedisk.h"
#include "cryptohandler.h"
#include "storage.h"

#include <qdir.h>

SecureDisk::SecureDisk(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.encryptPushButton, SIGNAL(released()), this, SLOT(encryptFolder()));
	connect(ui.decryptPushButton, SIGNAL(released()), this, SLOT(decryptStorage()));
}

SecureDisk::~SecureDisk()
{

}

void SecureDisk::encryptFolder()
{
	QString folderPath = "D:\\dev\\src\\SecureDisk\\test\\";//ui.folderPathLineEdit->text();
	QString storagePath = "D:\\dev\\src\\SecureDisk\\out\\data.dat";// ui.storagePathLineEdit->text();

	QDir directory(folderPath);
	QStringList files = directory.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

	Storage storage;

	storage.open(storagePath);

	for each (QString file in files)
	{
		QFile* fileHandler = new QFile(folderPath + file);
		fileHandler->open(QIODevice::ReadOnly);
		storage.put(fileHandler, file);
		fileHandler->close();
		delete fileHandler;
	}
	storage.close();
}

void SecureDisk::decryptStorage()
{
	QString folderPath = "D:\\dev\\src\\SecureDisk\\test\\";//ui.folderPathLineEdit_2->text();
	QString storagePath = "D:\\dev\\src\\SecureDisk\\out\\data.dat";// ui.storagePathLineEdit_2->text();

	QDir directory(folderPath);
	QStringList files = directory.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

	Storage storage;

	storage.open(storagePath);

	for each (QString file in files)
	{
		QFile* fileHandler = new QFile(folderPath + file);
		fileHandler->open(QIODevice::ReadOnly);
		storage.put(fileHandler, file);
		fileHandler->close();
		delete fileHandler;
	}
	storage.close();
}