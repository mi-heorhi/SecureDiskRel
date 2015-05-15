#ifndef KEYPATHDIALOG_H
#define KEYPATHDIALOG_H

#include <QDialog>
#include "ui_keypathdialog.h"

class KeyPathDialog : public QDialog
{
	Q_OBJECT

public:
	KeyPathDialog(QWidget *parent = 0);
	~KeyPathDialog();

private:
	Ui::KeyPathDialog ui;
};

#endif // KEYPATHDIALOG_H
