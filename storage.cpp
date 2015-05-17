#include "storage.h"

Storage::Storage(QObject *parent) :
QObject(parent), _storage(new QFile())
{
}

QFile* Storage::open(QString path)
{
	_storage = new QFile(path);
	_storage->open(QIODevice::ReadWrite | QIODevice::Append);
	return _storage;
}



void Storage::close()
{
	_storage->close();
}

void Storage::put(QFile* file, QString filename)
{
	char* buf;
	fileInfo info(filename.toStdString().c_str(), file->size());
	file->open(QIODevice::ReadOnly);
	int fileSize = file->size();
	if (file->isOpen())
	{
		_storage->write((const char*)&info, info.length());
		buf = new char[fileSize];
		file->read(buf, fileSize);
		_storage->write(buf, fileSize);
		_storage->flush();
		file->close();
	}
}

void Storage::out(QFile* fp, QString fileName)
{
	fileInfo info;
	fp->open(QIODevice::WriteOnly);
	char* buf;
	do
	{
		memset(&info, 0, sizeof(fileInfo));

		_storage->read((char*)&info, info.length());
		if (info.size == 0)
			break;
		else
		{
			if (fileName == QString(info.name))
			{
				buf = new char[info.size];
				_storage->read(buf, info.size);
				fp->write(buf, info.size);
				delete[] buf;
			}
			_storage->seek(info.size);
		}
	}
	while (!_storage->atEnd());
	fp->close();
}

QStringList Storage::getNames()
{
	QStringList fileList;
	fileInfo info;
	_storage->seek(0);
	do
	{
		memset(&info, 0, sizeof(fileInfo));

		_storage->read((char*)&info, info.size);
		if (info.size == 0)
			break;
		else
		{
			fileList << QString(info.name);
			_storage->seek(info.size);
		}
	}
	while (!_storage->atEnd());
	return fileList;
}
