#ifndef SQLXEIGEN_LIB_H
#define SQLXEIGEN_LIB_H

#include <iostream>

#include "utils.h"
#include "mysql/connection_pool.h"
#include "datatype/datetime.h"
#include "datastruct/matrix/icolumn.h"
#include "datastruct/matrix/column.h"
#include "datastruct/matrix/imatrix.h"
#include "datastruct/matrix/matrix.h"
#include "datastruct/sqlmatrix/sqlmatrix.h"
#include "datastruct/irowview.h"
#include "view/baseview.h"
#include "view/queryview.h"
#include "view/tableview.h"
#include "datastruct/column-v2.h"
#include "datastruct/column-v3.h"


namespace sqlxeigen {


    bool printBuildMode() {
    #ifdef NDEBUG
        std::cout << "[mysql-interface] Running DEBUG build\n";
        return true;
    #else
        std::cout << "[mysql-interface] Running RELEASE build\n";
        return false;
    #endif
    }

    bool _inDebug = printBuildMode();

};


#endif