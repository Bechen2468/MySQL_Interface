#include "mysql/connection_pool.h"



namespace sqlxeigen {


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



ConnectionPool::ConnectionPool(const mysqlx::SessionSettings Settings, size_t Size): 
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

    if(!_sessions.empty()) {
        std::unique_ptr<mysqlx::Session> session = std::move(_sessions.front());
        _sessions.pop();
        return session;
    }


    // create new session if no connection available ( also a temporary session if size many sessions are busy )
    try {
        return std::make_unique<mysqlx::Session>(_settings);
    } 
    catch (const mysqlx::Error &err) {
        std::cerr << "[MySQLX Error] Session creation failed: " << err.what() << std::endl;
        throw; // rethrow
    } 
    catch (const std::exception &err) {
        std::cerr << "[STD Exception] Session creation failed: " << err.what() << std::endl;
        throw; // rethrow
    }
}



void ConnectionPool::release_session(std::unique_ptr<mysqlx::Session>& session) {
    std::scoped_lock lock(_lock);

    // destroy if queue already full
    if(_sessions.size() >= size) {
        session->close();
        return;
    }

    _sessions.push(std::move(session));
}



std::mutex ConnectionPool::_connectionPoolMapMutex;
std::unordered_map<std::string, std::shared_ptr<ConnectionPool>> ConnectionPool::_connectionPoolMap;

};