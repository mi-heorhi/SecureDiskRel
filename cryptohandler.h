#ifndef CRYPTOHANDLER_H
#define CRYPTOHANDLER_H

#include <QObject>

class CryptoHandler : public QObject
{
	Q_OBJECT

public:
	CryptoHandler(QObject *parent);
	~CryptoHandler();
	static QString randomStrGen(int length);
	static QString RSAEncryptString(QString publickeyfile,
							 QString message);
	static QString RSADecryptString(QString privatekeyfile,
							 QString ciphertext);
	static void EncryptFile(QString publickeyfile,
					 QString filetoencrypt,
					 QString encryptedfile);
	static void DecryptFile(QString privatekeyfile,
					 QString filetodecrypt,
					 QString decryptedfile);

private:

};

#endif // CRYPTOHANDLER_H
