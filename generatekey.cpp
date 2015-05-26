#include "generatekey.h"

#include <QMessageBox>

GenerateKey::GenerateKey(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(discart()));
}

GenerateKey::~GenerateKey()
{

}


void GenerateKey::SavePublicKey(const QString& filename, const PublicKey& key)
{
	ByteQueue queue;
	key.Save(queue);

	SaveHex(filename, queue);
}

void GenerateKey::SavePrivateKey(const QString& filename, const PrivateKey& key)
{
	ByteQueue queue;
	key.Save(queue);

	SaveHex(filename, queue);
}

void GenerateKey::Save(const QString& filename, const BufferedTransformation& bt)
{
	FileSink file(filename.toStdString().c_str());

	bt.CopyTo(file);
	file.MessageEnd();
}

void GenerateKey::accept()
{
	AutoSeededRandomPool rnd;
	QString path = ui.keyPathLineEdit->text();
	if (!path.isEmpty())
	{
		RSAES_OAEP_SHA_Decryptor rsaPrivate;

		rsaPrivate.AccessKey().GenerateRandomWithKeySize(rnd, 1024);


		RSAES_OAEP_SHA_Encryptor rsaPublic(rsaPrivate);
		SavePrivateKey(path + QString("\\") + QString(PRIVATE_KEY), rsaPrivate.AccessKey());
		SavePublicKey(path + QString("\\") + QString(PUBLIC_KEY), rsaPublic.AccessKey());
		close();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Error");
		msgBox.setInformativeText("Path can't be empty");
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
	}
}

void GenerateKey::SaveHex(const QString& filename, const BufferedTransformation& bt)
{
	HexEncoder encoder;

	bt.CopyTo(encoder);
	encoder.MessageEnd();

	Save(filename, encoder);
}


void GenerateKey::discart()
{
	close();
}