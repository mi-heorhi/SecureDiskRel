#ifndef CRYPTOHANDLER_H
#define CRYPTOHANDLER_H

#include <QObject>

using namespace std;

class CryptoHandler : public QObject
{
	Q_OBJECT

public:
	CryptoHandler(QObject *parent);
	~CryptoHandler();
	static string randomStrGen(int length);
	static string RSAEncryptString(string publickeyfile,
								   string message);
	static string RSADecryptString(string privatekeyfile,
								   string ciphertext);
	static void EncryptFile(QString publickeyfile,
					 QString filetoencrypt,
					 QString encryptedfile);
	static void DecryptFile(QString privatekeyfile,
					 QString filetodecrypt,
					 QString decryptedfile);

private:

};

#endif // CRYPTOHANDLER_H
