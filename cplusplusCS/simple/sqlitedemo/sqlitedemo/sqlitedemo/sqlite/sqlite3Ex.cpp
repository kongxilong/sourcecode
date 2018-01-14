
//
// SQLite Database


#include "..\StdAfx.h"
#include "sqlite3Ex.h"
#include <assert.h>

static const char kNULL[] = "\0\0\0";



//////////////////////////////////////////////////////////////////////////////
// SQLiteDB
SQLiteEx::SQLiteEx()
{
	sqlite3_ = NULL;
}

SQLiteEx::SQLiteEx(SQLiteEx& src)
{
	sqlite3_     = src.sqlite3_;
	src.sqlite3_ = NULL;
}

SQLiteEx::~SQLiteEx()
{
	Interrupt();
	Close();
}

SQLiteEx& SQLiteEx::operator=(SQLiteEx& src)
{
	Interrupt();
	Close();

	sqlite3_     = src.sqlite3_;
	src.sqlite3_ = NULL;
	return *this;
}

bool SQLiteEx::Open(int flags)
{
	return Open(":memory:", flags);
}

bool SQLiteEx::Open(const char* filename, int flags)
{
	Interrupt();
	//先把
	Close();

	if (filename == NULL)
		return false;

	int r = sqlite3_open_v2(filename, &sqlite3_, flags, NULL);
	if (r != SQLITE_OK)
		Close();

	return r == SQLITE_OK;
}

//bool SQLiteDB::OpenEncrypt(const char* filename, int flags,const std::string &key)
//{
//	Interrupt();
//	Close();
//
//	if (filename == NULL)
//		return false;
//
//	int r = sqlite3_open_v2(filename, &sqlite3_, flags, NULL);
//	if (r != SQLITE_OK)
//	{
//		Close();
//	}
//	else
//	{
//		if (key.length())
//		{
//			sqlite3_key(sqlite3_,key.c_str(),key.length());
//		}
//	}
//	return r == SQLITE_OK;
//}

bool SQLiteEx::Close()
{
	if (sqlite3_ == NULL)
		return true;

	/*
	* If sqlite3_close() is called on a database connection that still has
	* outstanding prepared statements or BLOB handles, then it returns SQLITE_BUSY
	*/
	if (SQLITE_OK != sqlite3_close(sqlite3_))
		return false;

	sqlite3_ = NULL;
	return true;
}

bool SQLiteEx::Compact()
{
	if (SQLITE_OK != Query("VACUUM"))
		return false;

	return true;
}

bool SQLiteEx::IsValid() const
{
	return sqlite3_ != NULL;
}

int SQLiteEx::GetLastErrorCode() const
{
	if (sqlite3_ == NULL)
		return SQLITE_OK;
	return sqlite3_errcode(sqlite3_);
}

int SQLiteEx::GetLastExtendedErrorCode() const
{
	if (sqlite3_ == NULL)
		return SQLITE_OK;
	return sqlite3_extended_errcode(sqlite3_);
}

const char* SQLiteEx::GetLastErrorMessage() const
{
	assert(sqlite3_);
	if (sqlite3_ == NULL)
		return kNULL;
	return sqlite3_errmsg(sqlite3_);
}

int SQLiteEx::GetChanges() const
{
	assert(sqlite3_);
	if (sqlite3_ == NULL)
		return -1;
	return sqlite3_changes(sqlite3_);
}

int SQLiteEx::GetTotalChanges() const
{
	assert(sqlite3_);
	if (sqlite3_ == NULL)
		return -1;
	return sqlite3_total_changes(sqlite3_);
}

sqlite3_int64 SQLiteEx::GetLastInsertRowId() const
{
	assert(sqlite3_);
	if (sqlite3_ == NULL)
		return -1;
	return sqlite3_last_insert_rowid(sqlite3_);
}

int SQLiteEx::Query(SQLiteStatement& statement, const char* sql_text, int length)
{
	statement.Finalize();

	if (!sqlite3_ || !sql_text)
		return SQLITE_MISUSE;

	/*
	* SQLite documents say:
	* If the caller knows that the supplied string is nul-terminated,
	* then there is a small performance advantage to be gained by passing an nByte parameter
	* that is equal to the number of bytes in the input string including the nul-terminator bytes.
	*/
	if (length < 0)
		length = (int)strlen(sql_text) + 1;

	return sqlite3_prepare_v2(sqlite3_, sql_text, length, &statement.stmt_, NULL);
}

int SQLiteEx::TransactionQuery(SQLiteStatement& statement, const char* sql_text, int length)
{
	if (!sqlite3_ || !sql_text)
		return SQLITE_MISUSE;
	if (length < 0)
		length = (int)strlen(sql_text) + 1;

	return sqlite3_prepare_v2(sqlite3_, sql_text, length, &statement.stmt_, NULL);
}
void SQLiteEx::TransactionBegin()
{
	sqlite3_exec(sqlite3_,"begin;",0,0,0); 
}
void SQLiteEx::TransactionCommit()
{
	sqlite3_exec(sqlite3_,"commit;",0,0,0); 
}
void SQLiteEx::TransactionStep(SQLiteStatement& statement)
{
	sqlite3_step(statement.stmt_);
}
void SQLiteEx::TransactionReset(SQLiteStatement& statement)
{
	sqlite3_reset(statement.stmt_);
}
void SQLiteEx::TransactionFinalize(SQLiteStatement& statement)
{
	sqlite3_finalize(statement.stmt_);
}
int SQLiteEx::Query(const char* sql_text, SQLiteQueryCallback callback, void* param)
{
	if (!sqlite3_ || !sql_text)
		return SQLITE_MISUSE;
	//通过回调进行查询
	return sqlite3_exec(sqlite3_, sql_text,callback, param, NULL);
}

int SQLiteEx::Query(const char* sql_text, SQLiteResTable& table)
{
	if (!sqlite3_ || !sql_text)
		return SQLITE_MISUSE;
	// 先把原表清空
	table.Free();
	int r = sqlite3_get_table(sqlite3_, sql_text,
		&table.result_column_, &table.row_, &table.col_, NULL);
	if (r == SQLITE_OK)
		table.result_ = table.result_column_ + table.col_;
	return r;
}

void SQLiteEx::Interrupt()
{
	if (!sqlite3_)
		return;
	sqlite3_interrupt(sqlite3_);
}

int SQLiteEx::SetBusyTimeout(int ms)
{
	if (!sqlite3_)
		return SQLITE_MISUSE;
	return sqlite3_busy_timeout(sqlite3_, ms);
}

int SQLiteEx::GetVersion()
{
	return sqlite3_libversion_number();
}


//////////////////////////////////////////////////////////////////////////////
// SQLiteResultTable
SQLiteResTable::SQLiteResTable()
{
	Reset();
}

SQLiteResTable::SQLiteResTable(SQLiteResTable& src)
{
	row_              = src.row_;
	col_              = src.col_;
	result_                 = src.result_;
	result_column_    = src.result_column_;

	src.Reset();
}

SQLiteResTable::~SQLiteResTable()
{
	Free();
}

SQLiteResTable& SQLiteResTable::operator=(SQLiteResTable& src)
{
	row_              = src.row_;
	col_              = src.col_;
	result_                 = src.result_;
	result_column_    = src.result_column_;

	src.Reset();

	return *this;
}

void SQLiteResTable::Free()
{
	if (result_column_ != NULL)
	{
		sqlite3_free_table(result_column_);
		Reset();
	}
}

void SQLiteResTable::Reset()
{
	row_			= 0;
	col_			= 0;
	result_			= NULL;
	result_column_	= NULL;
}

bool SQLiteResTable::IsEmpty() const
{
	return result_column_ == 0;
}

int SQLiteResTable::GetRowCount() const
{
	return row_;
}

int SQLiteResTable::GetColumnCount() const
{
	return col_;
}

const char* SQLiteResTable::GetValue(int row, int col) const
{
	if (!result_ || row >= row_ ||
		col >= col_ || row < 0 || col < 0)
	{
		return 0;
	}

	return result_[row * col_ + col];
}

const char** const SQLiteResTable::operator[](int row) const
{	
	return ((const char**)result_ + row * col_);
}

const char* SQLiteResTable::GetColumnName(int col) const
{
	if (!result_column_ || col < 0 || col >= col_)
		return 0;

	return result_column_[col];
}


//////////////////////////////////////////////////////////////////////////////
// SQLiteStatement
SQLiteStatement::SQLiteStatement()
{
	stmt_ = NULL;
	eof_  = true;
}

SQLiteStatement::SQLiteStatement(SQLiteStatement& src)
{
	stmt_     = src.stmt_;
	eof_      = src.eof_;
	src.stmt_ = NULL;
	src.eof_  = true;
}

SQLiteStatement::~SQLiteStatement()
{
	Finalize();
}

SQLiteStatement& SQLiteStatement::operator=(SQLiteStatement& src)
{
	Finalize();

	stmt_     = src.stmt_;
	eof_      = src.eof_;
	src.stmt_ = NULL;
	src.eof_  = true;

	return *this;
}

int SQLiteStatement::Finalize()
{
	int r = SQLITE_OK;
	if (stmt_ != NULL)
	{
		r = sqlite3_finalize(stmt_);
		if (r == SQLITE_OK)
		{
			stmt_ = NULL;
			eof_  = true;
		}
	}
	return r;
}

int SQLiteStatement::Rewind()
{
	int r = SQLITE_MISUSE;
	if (stmt_ != NULL)
	{
		r = sqlite3_reset(stmt_);
		if (SQLITE_OK == r)
		{
			eof_ = false;
			return SQLITE_OK;
		}
	}
	return r;
}

int SQLiteStatement::NextRow()
{
	if (stmt_ == NULL)
		return SQLITE_MISUSE;

	int r = sqlite3_step(stmt_);

	if (r == SQLITE_DONE)
		eof_ = true;

	return r;
}

bool SQLiteStatement::IsEof()
{
	return eof_;
}

bool SQLiteStatement::IsNullField(int col)
{
	return GetFieldValue(col) != NULL;
}

const void* SQLiteStatement::GetBlobField(int col)
{
	if (stmt_ == NULL)
		return NULL;
	return sqlite3_column_blob(stmt_, col);
}

int   SQLiteStatement::GetBytesField(int col)
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_bytes(stmt_, col);
}

int   SQLiteStatement::GetBytes16Field(int col)
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_bytes16(stmt_, col);
}

double SQLiteStatement::GetDoubleField(int col)
{
	if (stmt_ == NULL)
		return 0.0;
	return sqlite3_column_double(stmt_, col);
}

int SQLiteStatement::GetIntField(int col)
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_int(stmt_, col);
}

sqlite3_int64 SQLiteStatement::GetInt64Field(int col)
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_int64(stmt_, col);
}

const char* SQLiteStatement::GetTextField(int col)
{
	if (stmt_ == NULL)
		return NULL;
	return (const char*)sqlite3_column_text(stmt_, col);
}

const void* SQLiteStatement::GetText16Field(int col)
{
	if (stmt_ == NULL)
		return NULL;
	return sqlite3_column_text16(stmt_, col);
}

int SQLiteStatement::GetTypeField(int col)
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_type(stmt_, col);
}

sqlite3_value* SQLiteStatement::GetFieldValue(int col)
{
	if (stmt_ == NULL)
		return NULL;
	return sqlite3_column_value(stmt_, col);
}

int SQLiteStatement::GetColumnCount()
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_count(stmt_);
}

int	SQLiteStatement::GetFieldBytes(int col)
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_bytes(stmt_, col);
}

int	SQLiteStatement::GetFieldBytes16(int col)
{
	if (stmt_ == NULL)
		return 0;
	return sqlite3_column_bytes16(stmt_, col);
}

int SQLiteStatement::BindBlob(int index, const void* data, int size, sqlite3_destructor_type destructor)
{
	if (stmt_ != NULL)
		return sqlite3_bind_blob(stmt_, index, data, size, destructor);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindDouble(int index, double data)
{
	if (stmt_ != NULL)
		return sqlite3_bind_double(stmt_, index, data);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindInt(int index, int data)
{
	if (stmt_ != NULL)
		return sqlite3_bind_int(stmt_, index, data);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindInt64(int index, int64_t data)
{
	if (stmt_ != NULL)
		return sqlite3_bind_int64(stmt_, index, data);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindNull(int index)
{
	if (stmt_ != NULL)
		return sqlite3_bind_null(stmt_, index);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindText(int index, const char* data, int size, sqlite3_destructor_type destructor)
{
	if (stmt_ != NULL)
		return sqlite3_bind_text(stmt_, index, data, size, destructor);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindText16(int index, const void* data, int size, sqlite3_destructor_type destructor)
{
	if (stmt_ != NULL)
		return sqlite3_bind_text16(stmt_, index, data, size, destructor);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindValue(int index, const sqlite3_value* data)
{
	if (stmt_ != NULL && data != NULL)
		return sqlite3_bind_value(stmt_, index, data);
	return SQLITE_MISUSE;
}

int SQLiteStatement::BindZeroBlob(int index, int size)
{
	if (stmt_ != NULL)
		return sqlite3_bind_zeroblob(stmt_, index, size);
	return SQLITE_MISUSE;
}


//////////////////////////////////////////////////////////////////////////////
// SQLiteAutoTransaction
SQLiteAutoTransaction::SQLiteAutoTransaction(SQLiteEx* db)
: db_(db), need_commit_(false)
{
	Begin();
}

SQLiteAutoTransaction::~SQLiteAutoTransaction()
{
	if (need_commit_)
		Commit();
}

bool SQLiteAutoTransaction::Begin()
{
	bool result = false;
	if (db_ && !need_commit_)
	{
		if (db_->Query("BEGIN") == SQLITE_OK)
			result = true;
		need_commit_ = true;
	}
	return result;
}

bool SQLiteAutoTransaction::Commit()
{
	bool result = 0;
	if (db_ != NULL)
	{
		if (db_->Query("END") == SQLITE_OK)
			result = true;
		need_commit_ = false;
	}
	return result;
}

bool SQLiteAutoTransaction::Rollback()
{
	bool result = false;
	if (db_ != NULL)
	{
		if (db_->Query("ROLLBACK") == SQLITE_OK)
			result = true;
		need_commit_ = false;
	}
	return result;
}