/** ****************************************************************************
 * @file main.cpp
 * @author Trevor Horst
 * @copyright
 * @brief Entry point for the Fawkes Library example application
 * ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define LOG_USE_COLOR

#include "fawkes/core/json/cjson.h"
#include "fawkes/core/logger/log.h"

int main( /* int argc, char *argv[] */ )
{
    cJSON *test = cJSON_CreateObject();
    cJSON_AddStringToObject( test, "hello", "world" );

    char *hello = cJSON_PrintUnformatted( test );
    LOG_INFO( "%s", hello );

    free( hello );
    cJSON_Delete( test );

    return 0;
}
