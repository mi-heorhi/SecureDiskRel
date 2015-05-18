#include "generatekey.h"

GenerateKey::GenerateKey(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
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
	RSAES_OAEP_SHA_Decryptor rsaPrivate;

	rsaPrivate.AccessKey().GenerateRandomWithKeySize(rnd, 1024);
	

	RSAES_OAEP_SHA_Encryptor rsaPublic(rsaPrivate);
	SavePrivateKey(path + QString("\\") + QString(PRIVATE_KEY), rsaPrivate.AccessKey());
	SavePublicKey(path + QString("\\") + QString(PUBLIC_KEY), rsaPublic.AccessKey());
	close();
}

void GenerateKey::SaveHex(const QString& filename, const BufferedTransformation& bt)
{
	HexEncoder encoder;

	bt.CopyTo(encoder);
	encoder.MessageEnd();

	Save(filename, encoder);
}