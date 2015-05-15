#include "cryptohandler.h"

#include "cryptocpp\config.h"
#include "cryptocpp\cryptlib.h"
#include "cryptocpp\default.cpp"
#include "cryptocpp\files.h"
#include "cryptocpp\rsa.h"
#include "cryptocpp\randpool.h"
#include "cryptocpp\hex.h"

using namespace CryptoPP;

CryptoHandler::CryptoHandler(QObject *parent)
: QObject(parent)
{

}

CryptoHandler::~CryptoHandler()
{

}

QString CryptoHandler::randomStrGen(int length)
{
	static QString charset
		= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	QString result;
	result.resize(length);
	for (int i = 0; i < length; i++) {
		result[i] = charset[rand() % charset.length()];
	}
	return result;
}

QString CryptoHandler::RSAEncryptString(QString publickeyfile,
	QString message)
{
	QString seed = randomStrGen(50);

	FileSource pubFile(publickeyfile.toStdString().c_str(), true, new HexDecoder());
	RSAES_OAEP_SHA_Encryptor pub(pubFile);

	RandomPool randPool;
	randPool.IncorporateEntropy((byte *)seed.toStdString().c_str(),
		strlen(seed.toStdString().c_str()));

	QString result;
	StringSource(message.toStdString().c_str(), true,
		new PK_EncryptorFilter(
		randPool,
		pub,
		new HexEncoder(new StringSink(result.toStdString()))));
	return result;
}
QString CryptoHandler::RSADecryptString(QString privatekeyfile,
	QString ciphertext)
{
	FileSource privFile(privatekeyfile.toStdString().c_str(), true, new HexDecoder());
	RSAES_OAEP_SHA_Decryptor priv(privFile);

	QString result;
	StringSource(ciphertext.toStdString().c_str(), true,
		new HexDecoder(
		new PK_DecryptorFilter(RandomNumberGenerator(),
		priv,
		new StringSink(result.toStdString()))));
	return result;
}
void CryptoHandler::EncryptFile(QString publickeyfile,
	QString filetoencrypt,
	QString encryptedfile)
{
	// Generate passphrase
	QString passphrase = randomStrGen(50);
	QString encrypted_passphrase = RSAEncryptString(publickeyfile,
		passphrase);

	// Encrypt the file and save encrypted data to the string
	QString data;
	FileSource f(filetoencrypt.toStdString().c_str(), true,
		new DefaultEncryptorWithMAC(
		passphrase.toStdString().c_str(),
		new HexEncoder(new StringSink(data.toStdString()))));

	// Append encrypted passphrase to the beginning of the string
	data = encrypted_passphrase + data;

	// Save string to the file
	StringSource ss(data.toStdString().c_str(), true,
		new FileSink(encryptedfile.toStdString().c_str()));
}
void CryptoHandler::DecryptFile(QString privatekeyfile,
	QString filetodecrypt,
	QString decryptedfile)
{
	// Read string from file
	QString data;
	FileSource ff(filetodecrypt.toStdString().c_str(), true, new StringSink(data.toStdString()));

	// Grab passphrase from the string
	QString encrypted_passphrase = QString(data.toStdString().substr(0, 256).c_str());
	QString passphrase = RSADecryptString(privatekeyfile,
		encrypted_passphrase);

	// Grab cipher from the string
	QString cipher = QString(data.toStdString().substr(256, data.length()).c_str());

	// Decrypt string and save to file
	StringSource s(cipher.toStdString().c_str(), true,
		new HexDecoder(new DefaultDecryptorWithMAC(
		passphrase.toStdString().c_str(),
		new FileSink(decryptedfile.toStdString().c_str()))));
}