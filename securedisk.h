#ifndef SECUREDISK_H
#define SECUREDISK_H

#include <QtWidgets/QMainWindow>
#include "ui_securedisk.h"

class SecureDisk : public QMainWindow
{
	Q_OBJECT
public:
	SecureDisk(QWidget *parent = 0);
	~SecureDisk();
private slots:
    void encryptFolder();
	void decryptStorage();
	void generateKeys();
private:
	Ui::SecureDiskClass ui;
};

#endif // SECUREDISK_H
