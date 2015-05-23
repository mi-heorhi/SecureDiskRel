#ifndef GENERATEKEY_H
#define GENERATEKEY_H

#include <QDialog>
#include "ui_generatekey.h"

#include "cryptocpp\rsa.h"
#include "cryptocpp\osrng.h"
#include "cryptocpp\files.h"
#include "cryptocpp\hex.h"

#define PUBLIC_KEY "rsa-public.key"
#define PRIVATE_KEY "rsa-private.key"

using namespace CryptoPP;

class GenerateKey : public QDialog
{
	Q_OBJECT

public:
	GenerateKey(QWidget *parent = 0);
	~GenerateKey();
private: 
	void SavePublicKey(const QString& filename, const PublicKey& key);
	void SavePrivateKey(const QString& filename, const PrivateKey& key);
	void Save(const QString& filename, const BufferedTransformation& bt);
	void SaveHex(const QString& filename, const BufferedTransformation& bt);
private slots:
    void accept();
	void discart();

private:
	Ui::GenerateKey ui;
};

#endif // GENERATEKEY_H
