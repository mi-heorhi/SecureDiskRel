#include "cryptohandler.h"

#include "cryptocpp\cryptlib.h"
#include "cryptocpp\default.cpp"
#include "cryptocpp\files.h"
#include "cryptocpp\rsa.h"
#include "cryptocpp\randpool.h"
#include "cryptocpp\hex.h"
#include "cryptocpp\osrng.h"

#include <QMessageBox>

using namespace CryptoPP;

static OFB_Mode<AES>::Encryption s_globalRNG;
RandomNumberGenerator & GlobalRNG()
{
	return s_globalRNG;
}

CryptoHandler::CryptoHandler(QObject *parent)
: QObject(parent)
{

}

CryptoHandler::~CryptoHandler()
{

}

string CryptoHandler::randomStrGen(int length)
{
	static string charset
		= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	string result;
	result.resize(length);
	for (int i = 0; i < length; i++)
	{
		result[i] = charset[rand() % charset.length()];
	}
	return result;
}

string CryptoHandler::RSAEncryptString(string publickeyfile,
									   string message)
{
	string seed = randomStrGen(50);

	FileSource pubFile(publickeyfile.c_str(), true, new HexDecoder());
	RSAES_OAEP_SHA_Encryptor pub(pubFile);
	RandomPool randPool;
	randPool.IncorporateEntropy((byte *)seed.c_str(),
								strlen(seed.c_str()));

	string result;
	StringSource(message.c_str(), true,
				 new PK_EncryptorFilter(
				 randPool,
				 pub,
				 new HexEncoder(new StringSink(result))));
	return result;
}

string CryptoHandler::RSADecryptString(string privatekeyfile,
									   string ciphertext)
{
	FileSource privFile(privatekeyfile.c_str(), true, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privFile);
	std::string seed2 = IntToString(time(NULL));
	seed2.resize(16);
	s_globalRNG.SetKeyWithIV((byte *)seed2.data(), 16, (byte *)seed2.data());
	string result;
	StringSource(ciphertext, true, new HexDecoder(new PK_DecryptorFilter(GlobalRNG(), priv, new StringSink(result))));
	return result;
}

void CryptoHandler::EncryptFile(QString publickeyfile,
								QString filetoencrypt,
								QString encryptedfile)
{
	try
	{
		// Generate passphrase
		string passphrase = randomStrGen(50);
		string encrypted_passphrase = RSAEncryptString(publickeyfile.toStdString(),
													   passphrase);

		// Encrypt the file and save encrypted data to the string
		string data;
		FileSource f(filetoencrypt.toStdString().c_str(), true,
					 new DefaultEncryptorWithMAC(
					 passphrase.c_str(),
					 new HexEncoder(new StringSink(data))));

		// Append encrypted passphrase to the beginning of the string
		data = encrypted_passphrase + data;

		// Save string to the file
		StringSource ss(data.c_str(), true,
						new FileSink(encryptedfile.toStdString().c_str()));
	}
	catch (CryptoPP::Exception e)
	{
		QMessageBox msgBox;
		msgBox.setText("Error");
		msgBox.setInformativeText(e.what());
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
	}
}

void CryptoHandler::DecryptFile(QString privatekeyfile,
								QString filetodecrypt,
								QString decryptedfile)
{
	try
	{
		// Read string from file
		std::string data;
		FileSource ff(filetodecrypt.toStdString().c_str(), true, new StringSink(data));

		// Grab passphrase from the string
		string encrypted_passphrase = data.substr(0, 256);
		encrypted_passphrase.resize(256);
		string passphrase = RSADecryptString(privatekeyfile.toStdString(),
											 encrypted_passphrase);

		// Grab cipher from the string
		std::string cipher = data.substr(256, data.length()).c_str();

		// Decrypt string and save to file
		StringSource s(cipher.c_str(), true,
					   new HexDecoder(new DefaultDecryptorWithMAC(
					   passphrase.c_str(),
					   new FileSink(decryptedfile.toStdString().c_str()))));
	}
	catch (CryptoPP::Exception e)
	{
		QMessageBox msgBox;
		msgBox.setText("Error");
		msgBox.setInformativeText(e.what());
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
	}
}