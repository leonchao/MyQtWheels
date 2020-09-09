#include "sqlconnector.h"
#include <QSqlError>
#include <QMutexLocker>
#include <QDebug>

SqlConnector *SqlConnector::_instance = nullptr;

SqlConnector *SqlConnector::getInstance()
{
    QMutexLocker(&_mutex);
    if(_instance == nullptr)
        _instance = new SqlConnector;
    return _instance;
}

void SqlConnector::deleteInstance()
{
    QMutexLocker(&_mutex);
    if(_instance != nullptr)
        delete _instance;
    _instance = nullptr;
}

bool SqlConnector::isConnected()
{
    return _database.isOpen();
}

bool SqlConnector::connectToHost(QString hostName, QString password, int port, QString userName, QString dataBaseName)
{
    QMutexLocker(&_mutex);
    _database =QSqlDatabase::addDatabase("QMYSQL");
    _database.setHostName(hostName);      //连接数据库主机名，这里需要注意（若填的为”127.0.0.1“，出现不能连接，则改为localhost)
    _database.setPort(port);                 //连接数据库端口号，与设置一致
    _database.setDatabaseName(dataBaseName);      //连接数据库名，与设置一致
    _database.setUserName(userName);          //数据库用户名，与设置一致
    _database.setPassword(password);    //数据库密码，与设置一致
    if(!_database.open())
    {
        qDebug()<<"不能连接"<<"connect to mysql error"<<_database.lastError().text();
    }
    else
    {
        qDebug()<<"连接成功"<<"connect to mysql OK";
    }
    return _database.isOpen();
}

void SqlConnector::closeConnection()
{
    if(_database.isOpen())
        _database.close();
}

SqlConnector::SqlConnector(QObject *parent) : QObject(parent)
{
}

SqlConnector::~SqlConnector()
{
    if(_database.isOpen())
        _database.close();
}
