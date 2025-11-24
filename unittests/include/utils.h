#ifndef UNITTEST_UTILS_H
#define UNITTEST_UTILS_H

#include <string>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <chrono>
#include <sqlxeigen-lib.h>
#include <mysqlx/xdevapi.h>


namespace utils {
    namespace mysql {
        void add_db_pool(std::string db_name) {
            std::shared_ptr<sqlxeigen::ConnectionPool> pool = std::make_shared<sqlxeigen::ConnectionPool>("localhost", 33060, "root", "testpass", db_name, 3);
            sqlxeigen::ConnectionPool::AddPool(std::move(pool));
        }

        bool wait_for_spoolup(int seconds_max) {
            mysqlx::SessionSettings settings(
                mysqlx::SessionOption::HOST, "localhost", 
                mysqlx::SessionOption::PORT, 33060, 
                mysqlx::SessionOption::USER, "root", 
                mysqlx::SessionOption::PWD, "testpass",
                mysqlx::SessionOption::DB, "db_test" 
            );

            for(int i = 0; i < seconds_max; i++) {
                try {
                    mysqlx::Session sess = mysqlx::Session(settings);
                    return true;
                }
                catch (const std::exception &err) {
                    std::cerr << "[STD Exception] Session creation try nr. " << (i+1) << ": " << err.what() << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
            return false;
        }

    };



    namespace docker {
        bool start_mysql() {
            const std::string container = "test-mysql";
            const std::string root_pass = "testpass";
            const std::string db_name = "db_test";
            const std::string port = "3306";

            // Path to your SQL file (relative to build directory)
            std::filesystem::path dir = __FILE__;
            std::filesystem::path sql_file = dir.parent_path().parent_path() / "sql" / "init.sql";

            std::cout << "--- Starting MySQL Docker ---" << std::endl;

            std::string cmd =
                "docker run -d --rm --name " + container +
                " -e MYSQL_ROOT_PASSWORD=" + root_pass +
                " -e MYSQL_DATABASE=" + db_name +
                " -p " + port + ":3306"
                " -p 33060:33060"
                " -v \"" + sql_file.string() + "\":/docker-entrypoint-initdb.d/init.sql"
                " mysql:8.0";

            std::cout << cmd << std::endl;
            int result = std::system(cmd.c_str());
            if (result != 0) {
                std::cout << "Failed to start MySQL Docker container" << std::endl;
                return false;
            }

            std::cout << "--- Waiting for MySQL to initialize ---" << std::endl;

            return utils::mysql::wait_for_spoolup(60);
        }



        void stop_mysql() {
            sqlxeigen::ConnectionPool::RemoveAllPools();
            std::cout << "--- Stopping MySQL Docker ---" << std::endl;
            std::system("docker stop test-mysql >nul 2>&1");
        }
    };



    namespace num {
        template<typename T>
        bool floatsEqual(T a, T b) {
            return std::abs(a - b) < 1e-9;
        }
    };



    namespace time {
        struct Timer {
            std::chrono::steady_clock::time_point _start;
            std::chrono::steady_clock::time_point _stop;

            void start() {
                _start = std::chrono::steady_clock::now();
            }

            void stop() {
                _stop = std::chrono::steady_clock::now();
            }

            long long duration_microS() const {
                return std::chrono::duration_cast<std::chrono::microseconds>(_stop - _start).count();
            }

            long long duration_ms() const {
                return std::chrono::duration_cast<std::chrono::milliseconds>(_stop - _start).count();
            }
        };

    }



    namespace benchmark {
        void printRow(const std::string& label, long long time) {
            std::cout 
                << std::left  << std::setw(30) << label
                << std::right << std::setw(12) << time
                << "\n";
        }

        void printHeader() {
            std::cout 
                << std::left  << std::setw(30) << "Benchmark"
                << std::right << std::setw(12) << "Time (ms)"
                << "\n";
            std::cout << std::string(42, '-') << "\n";
        }


        long long avg_vec_inorder(std::vector<int>& data) {
            std::vector<long long> results;
            for(int i = 0; i < 20; ++i) {
                utils::time::Timer t;
                int r = 0;
                t.start();
                for(int j = 0; j < data.size(); ++j) {
                    r += data[j];
                }

                t.stop();
                results.push_back(t.duration_microS());
            }

            long long res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        long long avg_mymat_inorder(sqlxeigen::matrix::Matrix& data, const std::string& colName) {
            std::vector<long long> results;
            for(int i = 0; i < 20; ++i) {
                utils::time::Timer t;
                int r = 0;
                t.start();
                for(int j = 0; j < data.rows(); ++j) {
                    r += data.get<int>(j, colName);
                }

                t.stop();
                results.push_back(t.duration_microS());
            }

            long long res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        long long avg_mymat_index_inorder(sqlxeigen::matrix::Matrix& data, const size_t colIndex) {
            std::vector<long long> results;
            for(int i = 0; i < 20; ++i) {
                utils::time::Timer t;
                int r = 0;
                t.start();
                for(int j = 0; j < data.rows(); ++j) {
                    r += data.get<int>(j, colIndex);
                }

                t.stop();
                results.push_back(t.duration_microS());
            }

            long long res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        long long avg_mycol_inorder(sqlxeigen::matrix::Column& data) {
            std::vector<long long> results;
            for(int i = 0; i < 20; ++i) {
                utils::time::Timer t;
                int r = 0;
                t.start();
                for(int j = 0; j < data.size(); ++j) {
                    r += data.get<int>(j);
                }

                t.stop();
                results.push_back(t.duration_microS());
            }

            long long res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }


        long long avg_eigen_inorder(Eigen::VectorXi& data) {
            std::vector<long long> results;
            for(int i = 0; i < 20; ++i) {
                utils::time::Timer t;
                int r = 0;
                t.start();
                for(int j = 0; j < data.rows(); ++j) {
                    r += data(j);
                }

                t.stop();
                results.push_back(t.duration_microS());
            }

            long long res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }


        long long avg_eigenraw_inorder(Eigen::VectorXi& data) {
            std::vector<long long> results;
            for(int i = 0; i < 20; ++i) {
                utils::time::Timer t;
                int r = 0;
                int* data_raw = data.data();
                t.start();
                for(int j = 0; j < data.rows(); ++j) {
                    r += data_raw[j];
                }

                t.stop();
                results.push_back(t.duration_microS());
            }

            long long res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }


        long long avg_mysql_inorder(std::vector<mysqlx::Row>& data) {
            std::vector<long long> results;
            for(int i = 0; i < 20; ++i) {
                utils::time::Timer t;
                int r = 0;
                t.start();
                for(int j = 0; j < data.size(); ++j) {
                    r += data[j][0].get<int>();
                }

                t.stop();
                results.push_back(t.duration_microS());
            }

            long long res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }
    }
};


#endif