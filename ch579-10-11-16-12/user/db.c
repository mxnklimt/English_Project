#define DB_C_

#include <db.h>

#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif


/* default KV nodes */
struct fdb_default_kv_node default_kv_table[] = {0};
/* KVDB object */
struct fdb_kvdb kvdb = {0};
/* TSDB object */
struct fdb_tsdb tsdb = {0};

void lock(fdb_db_t db) { __disable_irq(); }

void unlock(fdb_db_t db) { __enable_irq(); }

//获取数据库需要的时间戳
fdb_time_t get_time(void)
 {
  uint32_t i = 0;
  StTime.tm_year = GET_TIME.Year;
  StTime.tm_mon = GET_TIME.Month;
  StTime.tm_day = GET_TIME.Date;
  StTime.tm_hour = GET_TIME.Hours;
  StTime.tm_min = GET_TIME.Minutes;
  StTime.tm_sec = GET_TIME.Seconds;
  i = GetUnixSecond(&StTime, timeZone);
  RTC_GetTime(&GET_TIME.Year, &GET_TIME.Month, &GET_TIME.Date, &GET_TIME.Hours,&GET_TIME.Minutes, &GET_TIME.Seconds);

  return i;
}

//节点MAC地址管理
db_res db_manage_mac(const char *mac, size_t len) 
{
  size_t size;
//  FDB_ASSERT(&kvdb);
  struct fdb_blob blob;
  const char mac_temp[6] = {0};
  size = fdb_kv_get_blob(&kvdb, mac, fdb_blob_make(&blob, mac_temp, len));
  if(size > 0) 
	 {
    return DELETE_SUCCESS;
   } 
	else
	 {
    return ADD_SUCCESS;
   }
}

db_res db_add_mac(const char *mac, size_t len) 
{
  struct fdb_blob blob;
  fdb_kv_set_blob(&kvdb, mac, fdb_blob_make(&blob, mac, len));
	FDB_INFO("add to kvdb \r\n");
	return ADD_SUCCESS;
}

db_res db_delete_mac(const char *mac) 
{
    fdb_kv_del(&kvdb, mac);
    FDB_INFO("delete from db \r\n");
    log_hex((char *)mac, 6);	
	return ADD_SUCCESS;
}

//节点数据管理：添加一条新的数据到数据库
db_res db_append_log(void *buf, size_t len) 
{
  fdb_err_t result;
  struct fdb_blob blob;
  /* append new log to TSDB */
  result = fdb_tsl_append(&tsdb, fdb_blob_make(&blob, buf, len));
  FDB_INFO("append the new log \r\n");
  log_hex((char *)buf, len);
  if (result != FDB_NO_ERR)     return -1;
  else                          return APPEND_SUCCESS;
  
}

#ifdef FDB_USING_TSDB
//打印数据库所有数据
void tsdb_iterator(fdb_tsdb_t tsdb)
{
  FDB_INFO("==================== tsdb_iterator ====================\r\n");
  { 
    fdb_tsl_iter(tsdb, query_cb1, tsdb);		/* QUERY the TSDB */     /* query all TSL in TSDB by iterator */
  }
  FDB_INFO("===========================================================\r\n");
}

//遍历数据库回调函数
static bool query_cb1(fdb_tsl_t tsl, void *arg)
{
  struct fdb_blob blob;
  struTCP_LORA_DATA_UPLOAD  LORA_DATA_UPLOAD;
  fdb_tsdb_t db = arg;
  fdb_blob_read((fdb_db_t)db,fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, &LORA_DATA_UPLOAD, sizeof(LORA_DATA_UPLOAD))));
  FDB_INFO("[query_cb] queried a TSL: time: %" __PRITS", timeStamp: %ld, status: %d\r\n",tsl->time, (long)Yntohs(LORA_DATA_UPLOAD.LORA_DATA_TIME), tsl->status);

  return false;
}


//WRITE -> USER_STATUS1  标记上传数据状态
void db_set_current_tsl_status(fdb_time_t time_stamp)
{
	fdb_tsl_iter_by_time(&tsdb, time_stamp, time_stamp, query_by_time_cb, &tsdb);
}
//回调函数
static bool query_by_time_cb(fdb_tsl_t tsl, void *arg)
{
  struct fdb_blob blob;
  struTCP_LORA_DATA_UPLOAD  LORA_DATA_UPLOAD;
  fdb_tsdb_t db = arg;

  fdb_blob_read((fdb_db_t)db,fdb_tsl_to_blob(tsl, fdb_blob_make(&blob, &LORA_DATA_UPLOAD, sizeof(LORA_DATA_UPLOAD))));
	FDB_INFO("[query_by_time_cb] queried a TSL: time: %" __PRITS", \r\n",tsl->time);
	fdb_tsl_set_status(db, tsl, FDB_TSL_USER_STATUS1);
	//上传之后FDB_TSL_USER_STATUS1
	
	//8.21修
	fdb_tsl_set_status(db, tsl, FDB_TSL_WRITE);
	
  return false;
}

//发送数据库数据
void db_query_send(char flag)
{
	if(flag == 0)  
	{
		fdb_tsl_iter(&tsdb, send_not_upload_data_cb, &tsdb);
	}
	else if(flag == 1)
	{
		fdb_tsl_iter(&tsdb, send_all_data_cb, &tsdb);
	}
}
#endif /* FDB_USING_TSDB */



//两种数据库初始化，并打印数据库中的所有节点数据
fdb_err_t db_init(void) 
{
  fdb_err_t result;
  struct fdb_default_kv default_kv;
  default_kv.kvs = default_kv_table;
  default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
  /* set the lock and unlock function if you want */
  fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
  fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);
  /* Key-Value database initialization
   *
   *       &kvdb: database object
   *       "env": database name
   * "fdb_kvdb1": The flash partition name base on FAL. Please make sure it's in
   * FAL partition table. Please change to YOUR partition name. &default_kv: The
   * default KV nodes. It will auto add to KVDB when first initialize
   * successfully. NULL: The user data if you need, now is empty.
   */
  result = fdb_kvdb_init(&kvdb, "env", "KVDB", &default_kv, NULL);
  if (result != FDB_NO_ERR) {
    return -1;
  }
  /* set the lock and unlock function if you want */
  fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, (void *)lock);
  fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, (void *)unlock);
  /* Time series database initialization
   *
   *       &tsdb: database object
   *       "log": database name
   * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in
   * FAL partition table. Please change to YOUR partition name. get_time: The
   * get current timestamp function. 128: maximum length of each log NULL: The
   * user data if you need, now is empty.
   */
  result = fdb_tsdb_init(&tsdb, "log", "TSDB", get_time, 128, NULL);

  if (result != FDB_NO_ERR) {
    return -1;
  }
  tsdb_iterator(&tsdb);                   //打印所有mac
  if (result != FDB_NO_ERR) {
    return -1;
  }
}

