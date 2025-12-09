#pragma once

#include <string>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <chrono>
#include <Eigen/Dense>
#include <sqlxeigen-lib.h>
#include <mysqlx/xdevapi.h>




namespace utils {
    namespace mysql {
        void reset_database() {
            std::shared_ptr<sqlxeigen::ConnectionPool> pool = sqlxeigen::ConnectionPool::GetPool("db_test");
            std::unique_ptr<mysqlx::Session> sess = pool->get_session();

            sess->sql("CALL reset_db()").execute();

            pool->release_session(sess);
        }

        void add_db_pool(
            const std::string& root_pass = "testpass",
            const std::string& db_name = "db_test",
            const std::string& portx = "33060"
        ) {
            std::shared_ptr<sqlxeigen::ConnectionPool> pool = std::make_shared<sqlxeigen::ConnectionPool>("127.0.0.1", std::stoi(portx), "root", root_pass, db_name, 3);
            sqlxeigen::ConnectionPool::AddPool(std::move(pool));
        }

        bool wait_for_spoolup(
            size_t seconds_max,
            const std::string& root_pass = "testpass",
            const std::string& db_name = "db_test",
            const std::string& portx = "33060"
        ) {
            mysqlx::SessionSettings settings(
                mysqlx::SessionOption::HOST, "127.0.0.1", 
                mysqlx::SessionOption::PORT, std::stoi(portx), 
                mysqlx::SessionOption::USER, "root", 
                mysqlx::SessionOption::PWD, root_pass,
                mysqlx::SessionOption::DB, db_name
            );

            for(int i = 0; i < seconds_max; i++) {
                try {
                    mysqlx::Session sess = mysqlx::Session(settings);
                    return true;
                }
                catch (const std::exception &err) {
                    std::cerr << "[STD Exception] Session connection try nr." << std::right << std::setw(3) << (i+1) << std::left << ": " << err.what() << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
            return false;
        }

    };



    namespace docker {
        bool start_mysql(
            const std::string& container = "test-mysql",
            const std::string& root_pass = "testpass",
            const std::string& db_name = "db_test",
            const std::string& port = "3306",
            const std::string& portx = "33060"
        ) {

            // Path to your SQL file (relative to build directory)
            std::filesystem::path dir = std::filesystem::canonical(__FILE__);
            std::filesystem::path sql_file = dir.parent_path().parent_path() / "sql" / "init.sql";

            std::cout << "--- Starting MySQL Docker ---" << std::endl;

            std::string cmd =
                "docker run -d --rm --name " + container +
                " -e MYSQL_ROOT_PASSWORD=" + root_pass +
                " -e MYSQL_DATABASE=" + db_name +
                " -p " + port  + ":3306"
                " -p " + portx + ":33060"
                " -v \"" + sql_file.string() + "\":/docker-entrypoint-initdb.d/init.sql"
                " mysql:8.0";

            std::cout << cmd << std::endl;
            int result = std::system(cmd.c_str());
            if (result != 0) {
                std::cout << "Failed to start MySQL Docker container" << std::endl;
                return false;
            }

            std::cout << "--- Waiting for MySQL to initialize ---" << std::endl;

            return utils::mysql::wait_for_spoolup(60, root_pass, db_name, portx);
        }



        void stop_mysql() {
            sqlxeigen::ConnectionPool::RemoveAllPools();
            std::cout << "--- Stopping MySQL Docker ---" << std::endl;
            if(std::system("docker stop test-mysql >nul 2>&1") < 0) std::cerr << "ERROR: Could not stop mysql docker container." << std::endl;
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

            int64_t duration_muS() const {
                return std::chrono::duration_cast<std::chrono::microseconds>(_stop - _start).count();
            }

            int64_t duration_nS() const {
                return std::chrono::duration_cast<std::chrono::nanoseconds>(_stop - _start).count();
            }

            int64_t duration_ms() const {
                return std::chrono::duration_cast<std::chrono::milliseconds>(_stop - _start).count();
            }
        };

    }



    namespace benchmark {
        #define FORCE_SIMD

        void printRow(const std::string& label, int64_t time) {
            std::cout 
                << std::left  << std::setw(33) << label
                << std::right << std::setw(12) << time
                << "\n";
        }

        void printRowRelativ(const std::string& label, double rel) {
            std::cout 
                << std::left  << std::setw(38) << label
                << std::right << std::setw(7) << std::fixed << std::setprecision(2) << rel
                << "\n";
        }

        void printHeader() {
            std::cout 
                << std::left  << std::setw(33) << "Benchmark"
                << std::right << std::setw(12) << "Time (µs)"
                << "\n";
            std::cout << std::string(45, '-') << "\n";
        }

        void printHeaderRelativ() {
            std::cout 
                << std::left  << std::setw(25) << "Benchmark"
                << std::right << std::setw(20) << "Relativ Runtime"
                << "\n";
            std::cout << std::string(45, '-') << "\n";
        }



        int64_t avg_vec_inorder(std::vector<float>& data) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0; 
                const size_t s = data.size();
                
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data[j];
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        int64_t avg_mymat_inorder(std::shared_ptr<sqlxeigen::SqlMatrix>& data, const std::string& colName) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data->rows();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data->get<float>(j, colName);
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        int64_t avg_mymat_index_inorder(std::shared_ptr<sqlxeigen::SqlMatrix>& data, const size_t colIndex) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data->rows();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data->get<float>(j, colIndex);
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        int64_t avg_mycol_inorder(std::shared_ptr<sqlxeigen::Column<float>> data) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data->size();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data->get(j);
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        int64_t avg_mycolv2_inorder(sqlxeigen::matrix::ColumnV2& data) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data.size();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data.get<float>(j);
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }

        
        int64_t avg_mycolv3_inorder(sqlxeigen::matrix::ColumnV3& data) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data.size();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data.get_float(j);
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }


        int64_t avg_eigen_inorder(Eigen::VectorXf& data) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data.rows();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data(j);
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }


        int64_t avg_eigenraw_inorder(Eigen::VectorXf& data) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data.size();
                float* data_raw = data.data();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data_raw[j];
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }


        int64_t avg_mysql_inorder(std::vector<mysqlx::Row>& data) {
            std::vector<int64_t> results;
            for(int i = 0; i < 100; ++i) {
                utils::time::Timer t;
                float r = 0;
                const size_t s = data.size();
                t.start();

                #ifdef FORCE_SIMD
                #pragma clang loop vectorize(enable) interleave(enable) 
                #pragma GCC ivdep
                #endif
                for(int j = 0; j < s; ++j) {
                    r += data[j][0].get<float>();
                }

                t.stop();
                results.push_back(t.duration_muS());

                asm volatile("" :: "r"(r));
            }

            int64_t res = 0;
            for(int i = 0; i < results.size(); ++i) {
                res += results[i];
            }
            res /= results.size();
            return res;
        }
    }
};

