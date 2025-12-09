#pragma once

#include <ctime>
#include <gtest/gtest.h>
#include <sqlxeigen-lib.h>
#include "testutils.h"





class MySQL_Interface_Tests : public ::testing::Test {
protected:
    void SetUp() {
        utils::mysql::reset_database();
        //db_running = utils::docker::start_mysql();
        //utils::mysql::add_db_pool("db_test");
    }


    void TearDown() {
        //if(db_running) utils::docker::stop_mysql();
    }


    //static void SetUpTestSuite() {
    //    utils::mysql::reset_database();
    //    //db_running = utils::docker::start_mysql();
    //    //utils::mysql::add_db_pool("db_test");
    //}


    //static void TearDownTestSuite() {
    //    //if(db_running) utils::docker::stop_mysql();
    //}
public:
    static inline bool db_running = false;
};



class MyEnvironment : public ::testing::Environment {
 public:
  ~MyEnvironment() override {}

  // Override this to define how to set up the environment.
  void SetUp() override {
        MySQL_Interface_Tests::db_running = utils::docker::start_mysql("test-mysql", "testpass", "db_test", "3406", "34060");
        utils::mysql::add_db_pool("testpass", "db_test", "34060");
    }

  // Override this to define how to tear down the environment.
  void TearDown() override {
        if (MySQL_Interface_Tests::db_running) {
            try {
                utils::docker::stop_mysql();
            } catch (...) {
                std::cerr << "Warning: Docker stop failed" << std::endl;
            }
        }
    }
};







TEST_F(MySQL_Interface_Tests, String_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string query = "SELECT * FROM test_string";
    sqlxeigen::QueryView qv(query, "db_test");
    qv.setOrderBy("value ASC");
    qv.executeSelect();
    std::cerr << "DEBUG END" << std::endl << std::flush;

    // Check correct amount
    EXPECT_TRUE(qv.result->cols() == 1);
    EXPECT_TRUE(qv.result->rows() == 10);
    std::cerr << "DEBUG3" << std::endl << std::flush;
    // Check datacorrectness
    for(int i = 0; i < 10; ++i) {
        std::string v = qv.result->get<std::string>(i, "value");
        EXPECT_TRUE(v[0] == 'a' + i);
    }
    std::cerr << "DEBUG4" << std::endl << std::flush;
}



TEST_F(MySQL_Interface_Tests, Float_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string query = "SELECT * FROM test_float";
    sqlxeigen::QueryView qv(query, "db_test");
    qv.setOrderBy("value");
    qv.executeSelect();

    // Check correct amount
    ASSERT_TRUE(qv.result->cols() == 1);
    ASSERT_TRUE(qv.result->rows() == 11);

    // Check datacorrectness
    for(int i = 0; i <= 10; ++i) {
        float v = qv.result->get<float>(i, "value");
        ASSERT_TRUE(utils::num::floatsEqual(v, static_cast<float>(i * 1.1)));
    }
}



TEST_F(MySQL_Interface_Tests, Double_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string query = "SELECT * FROM test_double";
    sqlxeigen::QueryView qv(query, "db_test");
    qv.setOrderBy("value DESC");
    qv.executeSelect();

    // Check correct amount
    ASSERT_TRUE(qv.result->cols() == 1);
    ASSERT_TRUE(qv.result->rows() == 11);

    // Check datacorrectness
    for(int i = 0; i <= 10; ++i) {
        double v = qv.result->get<double>(i, "value");
        ASSERT_TRUE(utils::num::floatsEqual(v, static_cast<double>((10-i) * 1.1)));
    }
}



TEST_F(MySQL_Interface_Tests, Int_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string query = "SELECT * FROM test_int";
    sqlxeigen::QueryView qv(query, "db_test");
    qv.setOrderBy("value");
    qv.executeSelect();

    // Check correct amount
    ASSERT_TRUE(qv.result->cols() == 1);
    ASSERT_TRUE(qv.result->rows() == 10);

    // Check datacorrectness
    for(int i = 0; i < 10; ++i) {
        int v = qv.result->get<int>(i, "value");
        ASSERT_TRUE(utils::num::floatsEqual(v, i + 1));
    }
}



TEST_F(MySQL_Interface_Tests, Bool_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string query = "SELECT * FROM test_bool";
    sqlxeigen::QueryView qv(query, "db_test");
    qv.setOrderBy("value");
    qv.executeSelect();

    // Check correct amount
    ASSERT_TRUE(qv.result->cols() == 1);
    ASSERT_TRUE(qv.result->rows() == 2);

    // Check datacorrectness int8
    ASSERT_TRUE(qv.result->get<uint8_t>(0, "value") == 0);
    ASSERT_TRUE(qv.result->get<uint8_t>(1, "value") == 1);
}



TEST_F(MySQL_Interface_Tests, Datetime_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string query = "SELECT value FROM test_datetime";
    sqlxeigen::QueryView qv(query, "db_test");
    qv.setOrderBy("value");
    qv.executeSelect();

    // Check correct amount
    ASSERT_TRUE(qv.result->cols() == 1);
    ASSERT_TRUE(qv.result->rows() == 10);


    sqlxeigen::datatype::Datetime dt;    //  2025-01-01 01:00:00
    dt.year(2025);
    dt.month(1);
    dt.day(1);
    dt.hour(1);
    dt.minute(0);
    dt.second(0);

    for(int i = 0; i < qv.result->rows(); ++i) {
        sqlxeigen::datatype::Datetime v(qv.result->get<int64_t>(i, "value"));
        
        ASSERT_TRUE(dt == v);

        dt.day(dt.day() + 1);
        dt.hour(dt.hour() + 1);
    }
}


TEST_F(MySQL_Interface_Tests, SqlRowView_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string query = "SELECT * FROM test_multi";
    sqlxeigen::QueryView qv(query, "db_test");
    qv.setOrderBy("id");
    qv.executeSelect();

    // Check correct amount
    ASSERT_TRUE(qv.result->cols() == 5);
    ASSERT_TRUE(qv.result->rows() == 7);


    sqlxeigen::datatype::Datetime dt;    //  2025-01-01 01:00:00
    dt.year(2025);
    dt.month(1);
    dt.day(1);
    dt.hour(1);
    dt.minute(0);
    dt.second(0);

    sqlxeigen::SqlMatrix::RowView row1 = qv.result->rowView(0);
    auto row1Secure = row1;
    ASSERT_TRUE(row1.isFirst());

    for(int i = 0; i < qv.result->rows(); ++i) {
        sqlxeigen::SqlMatrix::RowView row2 = qv.result->rowView(i);

        ASSERT_TRUE(row1.index == row2.index);
        ASSERT_TRUE(row2.index == row1Secure.index);

        ASSERT_TRUE(row2.get<int>("id") == i);
        ASSERT_TRUE(row2.get<double>("v1") == (double)i / 2.0);
        ASSERT_TRUE(row2.get<std::string>("v2") == "test");
        ASSERT_TRUE(row2.get<uint8_t>("v3") == (i + 1) % 2);
        ASSERT_TRUE(dt == row2.get<int64_t>("v4"));

        row1 = row1.next();
        row1Secure = row1Secure.nextSecure();
    }
    
    ASSERT_TRUE(row1Secure.isLast());
}


TEST_F(MySQL_Interface_Tests, TableView_Select_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string dbName = "db_test";
    std::string tableName = "test_multi";
    sqlxeigen::TableView table(dbName, tableName);
    
    ASSERT_TRUE(table.executeSelect());

    auto res = table.result;

    // Check correct amount
    ASSERT_TRUE(res->cols() == 5);
    ASSERT_TRUE(res->rows() == 7);


    sqlxeigen::datatype::Datetime dt;    //  2025-01-01 01:00:00
    dt.year(2025);
    dt.month(1);
    dt.day(1);
    dt.hour(1);
    dt.minute(0);
    dt.second(0);

    sqlxeigen::SqlMatrix::RowView row1 = res->rowView(0);
    ASSERT_TRUE(row1.isFirst());

    for(int i = 0; i < res->rows(); ++i) {
        sqlxeigen::SqlMatrix::RowView row2 = res->rowView(i);

        ASSERT_TRUE(row1.index == row2.index);

        ASSERT_TRUE(row2.get<int>("id") == i);
        ASSERT_TRUE(row2.get<double>("v1") == (double)i / 2.0);
        ASSERT_TRUE(row2.get<std::string>("v2") == "test");
        ASSERT_TRUE(row2.get<uint8_t>("v3") == (i + 1) % 2);
        ASSERT_TRUE(dt == row2.get<int64_t>("v4"));

        row1 = row1.nextSecure();
    }
    
    ASSERT_TRUE(row1.isLast());
}


TEST_F(MySQL_Interface_Tests, TableView_Insert_Simple) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string dbName = "db_test";
    std::string tableName = "test_int";
    sqlxeigen::TableView table(dbName, tableName);
    
    ASSERT_TRUE(table.executeSelect());

    std::shared_ptr<sqlxeigen::SqlMatrix> res = table.result;

    // Check correct amount
    ASSERT_TRUE(res->cols() == 1);
    ASSERT_TRUE(res->rows() == 10);


    // Insert data
    std::shared_ptr<sqlxeigen::SqlMatrix> data = std::make_shared<sqlxeigen::SqlMatrix>();
    data->addColumn<mysqlx::Type::INT>("value", 10);
    for(int i = 0; i < 10; ++i) {
        data->get<int>(i, "value") = 11 + i;
    }

    ASSERT_TRUE(data->cols() == 1);
    ASSERT_TRUE(data->rows() == 10);

    ASSERT_TRUE(table.executeInsert(data));

    
    ASSERT_TRUE(table.executeSelect());

    res = table.result;

    // Check correct amount
    ASSERT_TRUE(res->cols() == 1);
    ASSERT_TRUE(res->rows() == 20);

    
    // Check datacorrectness
    for(int i = 0; i < 20; ++i) {
        int v = res->get<int>(i, "value");
        ASSERT_TRUE(utils::num::floatsEqual(v, i + 1));
    }
}


TEST_F(MySQL_Interface_Tests, TableView_Insert_OnDuplicate) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string dbName = "db_test";
    std::string tableName = "test_int";
    sqlxeigen::TableView table(dbName, tableName);
    
    ASSERT_TRUE(table.executeSelect());

    auto res = table.result;

    // Check correct amount
    ASSERT_TRUE(res->cols() == 1);
    ASSERT_TRUE(res->rows() == 10);


    // Insert data
    std::shared_ptr<sqlxeigen::SqlMatrix> data = std::make_shared<sqlxeigen::SqlMatrix>();
    data->addColumn<mysqlx::Type::INT>("value", 10);
    for(int i = 0; i < 10; ++i) {
        data->get<int>(i, "value") = 6 + i;
    }

    ASSERT_TRUE(data->cols() == 1);
    ASSERT_TRUE(data->rows() == 10);

    ASSERT_TRUE(table.executeInsert(data,{{"value", "value"}}));

    
    ASSERT_TRUE(table.executeSelect());

    res = table.result;

    // Check correct amount
    ASSERT_TRUE(res->cols() == 1);
    ASSERT_TRUE(res->rows() == 15);

    
    // Check datacorrectness
    for(int i = 0; i < 15; ++i) {
        int v = res->get<int>(i, "value");
        ASSERT_TRUE(utils::num::floatsEqual(v, i + 1));
    }
}


TEST_F(MySQL_Interface_Tests, TableView_Delete) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";

    std::string dbName = "db_test";
    std::string tableName = "test_int";
    sqlxeigen::TableView table(dbName, tableName);
    
    ASSERT_TRUE(table.executeSelect());

    auto res = table.result;

    // Check correct amount
    ASSERT_TRUE(res->cols() == 1);
    ASSERT_TRUE(res->rows() == 10);


    // Delete data
    ASSERT_TRUE(table.executeDelete("value > 5"));

    
    ASSERT_TRUE(table.executeSelect());

    res = table.result;

    // Check correct amount
    ASSERT_TRUE(res->cols() == 1);
    ASSERT_TRUE(res->rows() == 5);

    
    // Check datacorrectness
    for(int i = 0; i < 5; ++i) {
        int v = res->get<int>(i, "value");
        ASSERT_TRUE(utils::num::floatsEqual(v, i + 1));
    }
}




TEST_F(MySQL_Interface_Tests, BenchmarkRaw_InOrder_Access) {
    if(!db_running) GTEST_SKIP() << "Skipping: DB not running.";
    int test_size = 1000000;

    // Build data
    std::vector<float> data_vec(test_size);
    std::shared_ptr<sqlxeigen::SqlMatrix> data_mat = std::make_shared<sqlxeigen::SqlMatrix>();
    data_mat->addColumn<mysqlx::Type::FLOAT>("value", test_size);

    sqlxeigen::matrix::ColumnV2 data_colv2(mysqlx::Type::FLOAT, test_size, "value");
    sqlxeigen::matrix::ColumnV3 data_colv3(mysqlx::Type::FLOAT, test_size, "value");

    for(int i = 0; i < test_size; ++i) {
        float val = i + i/10;
        data_vec[i] = val;
        data_mat->get<float>(i, "value") = val;
        data_colv2.get<float>(i) = val;
        data_colv3.get_float(i) = val;
    }
    std::vector<float, Eigen::aligned_allocator<float>> v = data_mat->column<float>("value")->asVectorRef();
    Eigen::VectorXf eigenv = Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, 1>>(v.data(), v.size());


    std::string dbName = "db_test";
    std::string tableName = "test_float";
    sqlxeigen::TableView table(dbName, tableName);
    ASSERT_TRUE(table.executeInsert(data_mat, {{"value", "value"}}));
    
    // Get sql result
    std::shared_ptr<sqlxeigen::ConnectionPool> pool = sqlxeigen::ConnectionPool::GetPool(dbName);
    std::unique_ptr<mysqlx::Session> sess = pool->get_session();
    auto sql = sess->sql("SELECT * FROM test_float");
    mysqlx::SqlResult result = sql.execute();
    std::vector<mysqlx::Row> data_sql;
    mysqlx::Row row;
    while((row = result.fetchOne())) {
        data_sql.push_back(row);
    }

    pool->release_session(sess);



    int64_t baseRuntime = utils::benchmark::avg_vec_inorder(data_vec);
    utils::benchmark::printHeaderRelativ();
    utils::benchmark::printRowRelativ("std::vector<float>[i]",                  1.0);
    utils::benchmark::printRowRelativ("MyMatrix.get<float>(i, colName)",        (double)utils::benchmark::avg_mymat_inorder(data_mat, "value") / baseRuntime);
    utils::benchmark::printRowRelativ("MyMatrix.get<float>(i, colNum)",         (double)utils::benchmark::avg_mymat_index_inorder(data_mat, 0) / baseRuntime);
    utils::benchmark::printRowRelativ("MyColumn.get<float>(i)",                 (double)utils::benchmark::avg_mycol_inorder(data_mat->column<float>("value")) / baseRuntime);
    utils::benchmark::printRowRelativ("MyColumnV2.get<float>(i)",               (double)utils::benchmark::avg_mycolv2_inorder(data_colv2) / baseRuntime);
    utils::benchmark::printRowRelativ("MyColumnV3.get_float(i)",                (double)utils::benchmark::avg_mycolv3_inorder(data_colv3) / baseRuntime);
    utils::benchmark::printRowRelativ("Eigen::VectorXf(i)",                     (double)utils::benchmark::avg_eigen_inorder(eigenv) / baseRuntime);
    utils::benchmark::printRowRelativ("Eigen::VectorXf.data()[i]",              (double)utils::benchmark::avg_eigenraw_inorder(eigenv) / baseRuntime);
    utils::benchmark::printRowRelativ("std::vector<mysqlx::SqlRowView>.get<float>(i)", (double)utils::benchmark::avg_mysql_inorder(data_sql) / baseRuntime);
}


