#ifndef CRYPTOHANDLER_H
#define CRYPTOHANDLER_H

#include <QObject>

class CryptoHandler : public QObject
{
	Q_OBJECT

public:
	CryptoHandler(QObject *parent);
	~CryptoHandler();
	QString randomStrGen(int length);
	QString RSAEncryptString(QString publickeyfile,
							 QString message);
	QString RSADecryptString(QString privatekeyfile,
							 QString ciphertext);
	void EncryptFile(QString publickeyfile,
					 QString filetoencrypt,
					 QString encryptedfile);
	void DecryptFile(QString privatekeyfile,
					 QString filetodecrypt,
					 QString decryptedfile);

private:

};

#endif // CRYPTOHANDLER_H
