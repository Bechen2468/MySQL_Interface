#include "mysql/connection_pool.h"



ConnectionPool::ConnectionPool(const std::string& Host, int Port, const std::string& User, const std::string& Password, const std::string& Database, const size_t& Size):
_settings(
    mysqlx::SessionOption::HOST, Host, 
    mysqlx::SessionOption::PORT, Port, 
    mysqlx::SessionOption::USER, User, 
    mysqlx::SessionOption::PWD, Password,
    mysqlx::SessionOption::DB, Database 
),
size(Size)
{
}



ConnectionPool::ConnectionPool(const mysqlx::SessionSettings Settings, const size_t& Size): 
_settings(Settings),
size(Size)
{
}



ConnectionPool::~ConnectionPool() {
    std::scoped_lock lock(_lock);
    while(!_sessions.empty()) {
        _sessions.front()->close();
        _sessions.pop();
    }
}


std::string ConnectionPool::databaseName() {
    return _settings.find(mysqlx::SessionOption::DB).get<std::string>();
}


std::unique_ptr<mysqlx::Session> ConnectionPool::get_session() {
    std::scoped_lock lock(_lock);

    // create new session if no connection available ( also a temporary session if size many sessions are busy )
    if(_sessions.empty()) {
        std::unique_ptr<mysqlx::Session> session = std::make_unique<mysqlx::Session>(_settings);
        return session;
    }

    std::unique_ptr<mysqlx::Session> session = std::move(_sessions.front());
    _sessions.pop();
    return session;
}



void ConnectionPool::release_session(std::unique_ptr<mysqlx::Session>& session) {
    std::scoped_lock lock(_lock);

    // destroy if queue already full
    if(_sessions.size() > size) {
        session->close();
        return;
    }

    _sessions.push(session);
}


std::mutex _connectionPoolMapMutex;
std::unordered_map<std::string, std::shared_ptr<ConnectionPool>> _connectionPoolMap;

void AddConnectionPool(std::shared_ptr<ConnectionPool> pool) {
    std::scoped_lock cpMapLock(_connectionPoolMapMutex);
    _connectionPoolMap.insert(std::pair<std::string, std::shared_ptr<ConnectionPool>>(pool->databaseName(), pool));
}


std::shared_ptr<ConnectionPool> GetConnectionPool(const std::string& databaseName) {
    std::scoped_lock cpMapLock(_connectionPoolMapMutex);
    auto result = _connectionPoolMap.find(databaseName);
    if(result == _connectionPoolMap.end()) throw std::runtime_error("No such database created: " + databaseName);
    return result->second;
}
