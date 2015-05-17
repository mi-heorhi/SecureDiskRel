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
	QString folderPath = ui.folderPathLineEdit->text();
	QString storagePath = ui.storagePathLineEdit->text();
	QString publickKeyPath = ui.publicKeyPathLineEdit->text();
	QString tempFile = folderPath + "\\temp.dat";
	QDir directory(folderPath);
	QStringList files = directory.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

	Storage storage;

	storage.open(tempFile);

	for each (QString file in files)
	{
		QFile* fileHandler = new QFile(folderPath + file);
		fileHandler->open(QIODevice::ReadOnly);
		storage.put(fileHandler, file);
		fileHandler->close();
		delete fileHandler;
	}
	storage.close();

	CryptoHandler::EncryptFile(publickKeyPath, tempFile, storagePath);
}

void SecureDisk::decryptStorage()
{
	QString folderPath = ui.folderPathLineEdit_2->text();
	QString storagePath = ui.storagePathLineEdit_3->text();
	QString privateKeyPath = ui.privateKeyPathLineEdit->text();
	QString tempFile = folderPath + "\\temp.dat";
	CryptoHandler::DecryptFile(privateKeyPath, storagePath, tempFile);
	Storage storage;

	storage.open(tempFile);
	QStringList files = storage.getNames();
	for each (QString file in files)
	{
		QFile* fileHandler = new QFile(folderPath + file);
		fileHandler->open(QIODevice::ReadOnly);
		storage.out(fileHandler, file);
		fileHandler->close();
		delete fileHandler;
	}
	storage.close();
}