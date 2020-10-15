/** ****************************************************************************
 * @file main.cpp
 * @author Trevor Horst
 * @copyright
 * @brief Entry point for the Fawkes Library example application
 * ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fawkes/core/json/cjson.h"

int main( /* int argc, char *argv[] */ )
{
    cJSON *test = cJSON_CreateObject();
    cJSON_AddStringToObject( test, "hello", "world" );

    char *hello = cJSON_Print( test );
    printf( "%s\n", hello );

    free( hello );
    cJSON_Delete( test );

    return 0;
}
