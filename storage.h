#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <QFile>
#include <QStringList>

struct fileInfo
{
	char name[32];
	qint64 size;
	fileInfo(void) {}
	fileInfo(const char* name, qint64 size)
	{
		strcpy(this->name, this->getName(name));
		this->size = size;
	}
	const char* getName(const char* fileName)
	{
		const char* str = fileName + strlen(fileName);
		int         sz = strlen(fileName);
		while (*str != '\\' && sz--)
			--str;
		return str;
	}

	int length(void) const
	{
		return sizeof(name)+sizeof(size);
	}
};

class Storage : public QObject
{
	Q_OBJECT
public:
	explicit Storage(QObject *parent = 0);
	QFile* open(QString path);
	void close();
	void put(QFile* file, QString filename);
	void out(QFile* file, QString filename);
	QStringList getNames();
signals:

	public slots :
private:
	QFile* _storage;
};

#endif // STORAGE_H
