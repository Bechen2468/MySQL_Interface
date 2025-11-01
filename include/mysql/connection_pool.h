#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <string>
#include <queue>
#include <map>
#include <mutex>
#include <mysqlx/xdevapi.h>


class ConnectionPool {
private:
    std::queue<std::unique_ptr<mysqlx::Session>> _sessions;
    std::mutex _lock;
    mysqlx::SessionSettings _settings;

public:
    size_t size;

public:
    ConnectionPool(const std::string& Host, int Port, const std::string& User, const std::string& Password, const std::string& Database, const size_t& Size);
    ConnectionPool(const mysqlx::SessionSettings Settings, const size_t& Size);
    ~ConnectionPool();

    std::string databaseName();

    std::unique_ptr<mysqlx::Session> get_session();
    void release_session(std::unique_ptr<mysqlx::Session>& session);
};

void AddConnectionPool(std::shared_ptr<ConnectionPool> pool);

std::shared_ptr<ConnectionPool> GetConnectionPool(const std::string& databaseName);

#endif
