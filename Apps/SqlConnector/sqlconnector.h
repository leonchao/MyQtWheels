#ifndef SQLCONNECTOR_H
#define SQLCONNECTOR_H

#include <QObject>
#include <QSqlDatabase>
#include <QMutex>

class SqlConnector : public QObject
{
    Q_OBJECT
public:
    static SqlConnector *getInstance();
    static void deleteInstance();

    bool isConnected();
    bool connectToHost(QString hostName,QString password,int port,QString userName,QString dataBaseName);
    void closeConnection();
signals:

public slots:

private:
    explicit SqlConnector(QObject *parent = nullptr);
    ~SqlConnector();

    static SqlConnector *_instance;
    QSqlDatabase _database;
};

#endif // SQLCONNECTOR_H
