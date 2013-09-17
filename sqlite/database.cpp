/*
 * database.cpp
 *
 *  Created on: 16 sept. 2013
 *      Author: Boubacar
 */

#include "database.h"
#include "statement.h"
////////////////////////////////
namespace sqlite {
////////////////////////////////////////////
Database::Database() :
		m_errorcode(SQLITE_OK), m_pDb(nullptr) {
}
Database::Database(const char *pszFilename) :
		m_errorcode(SQLITE_OK), m_pDb(nullptr) {
	this->open(pszFilename);
} // Database
Database::Database(const std::string &filename) :
		m_errorcode(SQLITE_OK), m_pDb(nullptr) {
	this->open(filename);
} // Database
Database::Database(const wchar_t *pwszFilename) :
		m_errorcode(SQLITE_OK), m_pDb(nullptr) {
	this->open(pwszFilename);
} // Database
Database::Database(const std::wstring &wfilename) :
		m_errorcode(SQLITE_OK), m_pDb(nullptr) {
	this->open(wfilename);
} // Database
void Database::internal_clear_error(void) {
	this->m_errorcode = SQLITE_OK;
	this->m_errorstring.clear();
} // internal_clear_error
void Database::internal_get_error(void) {
	::sqlite3 *p = this->m_pDb;
	if (p != nullptr) {
		this->m_errorcode = ::sqlite3_errcode(p);
		const char *pSrc = ::sqlite3_errmsg(p);
		if (pSrc != nullptr) {
			this->m_errorstring = pSrc;
		}
	}
} // internal_get_error
bool Database::open(const char *pszFilename) {
	assert(pszFilename != nullptr);
	this->internal_clear_error();
	::sqlite3 *p = this->m_pDb;
	if (p != nullptr) {
		if (!this->close()) {
			return (false);
		}
	}
	int rc = ::sqlite3_open_v2(pszFilename, &p,
	SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
	if (rc != SQLITE_OK) {
		this->internal_get_error();
	}
	this->m_pDb = p;
	return (rc == SQLITE_OK);
} // open
bool Database::open(const wchar_t *pszFilename) {
	assert(pszFilename != nullptr);
	std::wstring ss(pszFilename);
	std::string s(ss.length(), ' ');
	std::copy(ss.begin(), ss.end(), s.begin());
	const char *pp = s.c_str();
	return this->open(pp);
} // open
bool Database::open(const std::string &filename) {
	return this->open(filename.c_str());
} // open
bool Database::open(const std::wstring &wfilename) {
	std::string s(wfilename.length(), ' ');
	std::copy(wfilename.begin(), wfilename.end(), s.begin());
	const char *pp = s.c_str();
	return this->open(pp);
} // open
Database::~Database() {
	this->close();
}
void Database::prepare_close(void) {
	std::list<PStatement> &oList = this->m_stmts;
	for (auto it = oList.begin(); it != oList.end(); ++it) {
		PStatement px = *it;
		if (px != nullptr) {
			px->force_close();
		}
	} // it
	oList.clear();
} // prepare_close
bool Database::close(void) {
	this->internal_clear_error();
	bool bRet = true;
	::sqlite3 *p = this->m_pDb;
	if (p != nullptr) {
		this->prepare_close();
		if (::sqlite3_close_v2(p) != SQLITE_OK) {
			this->internal_get_error();
			bRet = false;
		} else {
			this->m_pDb = nullptr;
		}
	}
	return (bRet);
} // close
///////////////////////////////
bool Database::exec_sql(const char *pszSQL) {
	assert(pszSQL != nullptr);
	::sqlite3 *p = this->m_pDb;
	if (p == nullptr) {
		return (false);
	}
	char *error = nullptr;
	int rc = ::sqlite3_exec(p, pszSQL, nullptr, nullptr, &error);
	if (error != nullptr) {
		this->m_errorcode = rc;
		this->m_errorstring = error;
		::sqlite3_free(error);
		return (false);
	}
	return (rc == SQLITE_OK);
} // exec_sql
bool Database::exec_sql(const wchar_t *pwszSQL) {
	assert(pwszSQL != nullptr);
	std::wstring ss(pwszSQL);
	std::string s(ss.length(), ' ');
	std::copy(ss.begin(), ss.end(), s.begin());
	return this->exec_sql(s.c_str());
} // exex_sql
bool Database::exec_sql(const std::string &s) {
	return this->exec_sql(s.c_str());
} // exec_sql
bool Database::exec_sql(const std::wstring &ss) {
	return this->exec_sql(ss.c_str());
} // exec_sql
////////////////////////////
} /* namespace sqlite */
