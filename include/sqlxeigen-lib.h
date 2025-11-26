#ifndef SQLXEIGEN_H
#define SQLXEIGEN_H

#include <iostream>

#include "mysql/connection_pool.h"
#include "datatype/datetime.h"
#include "datastruct/column.h"
#include "datastruct/column-v2.h"
#include "datastruct/column-v3.h"
#include "datastruct/matrix.h"
#include "datastruct/row.h"
#include "view/baseview.h"
#include "view/query.h"
#include "view/table.h"


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