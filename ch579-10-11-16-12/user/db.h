#ifndef DB_H_
#define DB_H_

#ifndef DB_C_//如果没有定义
#define DB_Cx_ extern
#else
#define DB_Cx_
#endif

#include "CH57x_common.h"
#include <flashdb.h>
#define FDB_LOG_TAG 

#ifdef FDB_USING_TIMESTAMP_64BIT
#define __PRITS "ld"
#else
#define __PRITS "d"
#endif

/* error code */
typedef enum {
		ADD_SUCCESS,
		DELETE_SUCCESS,
		APPEND_SUCCESS,
} db_res;

/* KVDB object */
DB_Cx_ struct fdb_kvdb kvdb ;
/* TSDB object */
DB_Cx_ struct fdb_tsdb tsdb ;



db_res db_append_log(void * buf, size_t len);
db_res db_manage_mac(const char *mac, size_t len);

void tsdb_iterator(fdb_tsdb_t tsdb);
static bool query_cb1(fdb_tsl_t tsl, void *arg);

void db_set_current_tsl_status(fdb_time_t time_stamp);
static bool query_by_time_cb(fdb_tsl_t tsl, void *arg);

void db_query_send(char flag);

fdb_err_t db_init(void);


db_res db_add_mac(const char *mac, size_t len) ;
db_res db_delete_mac(const char *mac) ;





#endif










