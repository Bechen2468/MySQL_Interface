#ifndef SQLXEIGEN_CONNECTION_POOL_H
#define SQLXEIGEN_CONNECTION_POOL_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <mutex>
#include <mysqlx/xdevapi.h>


namespace sqlxeigen {

class ConnectionPool {

private:
    std::queue<std::unique_ptr<mysqlx::Session>> _sessions;
    std::mutex _lock;
    mysqlx::SessionSettings _settings;

public:
    size_t size;

public:
    ConnectionPool(const std::string& Host, int Port, const std::string& User, const std::string& Password, const std::string& Database, const size_t& Size);
    ConnectionPool(const mysqlx::SessionSettings Settings, size_t Size);
    ~ConnectionPool();

    std::string databaseName();

    std::unique_ptr<mysqlx::Session> get_session();
    void release_session(std::unique_ptr<mysqlx::Session>& session);



protected:
    static std::mutex _connectionPoolMapMutex;
    static std::unordered_map<std::string, std::shared_ptr<ConnectionPool>> _connectionPoolMap;

public:
    static void AddPool(std::shared_ptr<ConnectionPool> pool) {
        std::scoped_lock cpMapLock(_connectionPoolMapMutex);
        _connectionPoolMap.insert(std::pair<std::string, std::shared_ptr<ConnectionPool>>(pool->databaseName(), pool));
    }

    static std::shared_ptr<ConnectionPool> GetPool(const std::string& databaseName) {
        std::scoped_lock cpMapLock(_connectionPoolMapMutex);
        auto result = _connectionPoolMap.find(databaseName);
        if(result == _connectionPoolMap.end()) throw std::runtime_error("No such database created: " + databaseName);
        return result->second;
    }

    static void RemoveAllPools() {
        _connectionPoolMap.clear();
    }
};

};
#endif
