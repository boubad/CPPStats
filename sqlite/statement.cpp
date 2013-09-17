/*
 * statement.cpp
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#include "statement.h"
////////////////////////////
#include <string.h>
///////////////////////////////
namespace sqlite {

Statement::Statement(Database &oBase, const char *pszSQL) :
		m_pBase(&oBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(pszSQL != nullptr);
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	int nbytes = ::strlen(pszSQL) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, pszSQL, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
		this->m_pBase->m_stmts.push_back(this);
	}
} // Statement
Statement::Statement(Database *pBase, const char *pszSQL) :
		m_pBase(pBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(pszSQL != nullptr);
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	int nbytes = ::strlen(pszSQL) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, pszSQL, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
		this->m_pBase->m_stmts.push_back(this);
	}
} // Statement
Statement::Statement(Database &oBase, const wchar_t *pszSQL) :
		m_pBase(&oBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(pszSQL != nullptr);
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	std::wstring ss(pszSQL);
	std::string s(ss.length(), ' ');
	std::copy(ss.begin(), ss.end(), s.begin());
	const char *sql = s.c_str();
	int nbytes = ::strlen(sql) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, sql, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
		this->m_pBase->m_stmts.push_back(this);
	}
} // Statement
Statement::Statement(Database *pBase, const wchar_t *pszSQL) :
		m_pBase(pBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(pszSQL != nullptr);
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	std::wstring ss(pszSQL);
	std::string s(ss.length(), ' ');
	std::copy(ss.begin(), ss.end(), s.begin());
	const char *sql = s.c_str();
	int nbytes = ::strlen(sql) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, sql, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
		this->m_pBase->m_stmts.push_back(this);
	}
} // Statement
Statement::Statement(Database &oBase, const std::string &s) :
		m_pBase(&oBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	const char *sql = s.c_str();
	int nbytes = ::strlen(sql) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, sql, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
		this->m_pBase->m_stmts.push_back(this);
	}
} // Statement
Statement::Statement(Database *pBase, const std::string &s) :
		m_pBase(pBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	const char *sql = s.c_str();
	int nbytes = ::strlen(sql) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, sql, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
	}
} // Statement
Statement::Statement(Database &oBase, const std::wstring &ss) :
		m_pBase(&oBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	std::string s(ss.length(), L' ');
	std::copy(ss.begin(), ss.end(), s.begin());
	const char *sql = s.c_str();
	int nbytes = ::strlen(sql) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, sql, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
		this->m_pBase->m_stmts.push_back(this);
	}
} // Statement
Statement::Statement(Database *pBase, const std::wstring &ss) :
		m_pBase(pBase), m_pstmt(nullptr), m_lastcode(SQLITE_OK) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3 *pdb = this->m_pBase->m_pDb;
	assert(pdb != nullptr);
	const char *pszTail = nullptr;
	std::string s(ss.length(), L' ');
	std::copy(ss.begin(), ss.end(), s.begin());
	const char *sql = s.c_str();
	int nbytes = ::strlen(sql) + 1;
	::sqlite3_stmt *pRes = nullptr;
	int rc = ::sqlite3_prepare_v2(pdb, sql, nbytes, &pRes, &pszTail);
	if ((rc == SQLITE_OK) && (pRes != nullptr)) {
		this->m_pstmt = pRes;
		this->m_pBase->m_stmts.push_back(this);
	}
} // Statement
bool Statement::is_valid(void) const {
	return ((this->m_pBase != nullptr) && m_pBase->is_open()
			&& (this->m_pstmt != nullptr));
} // is_valid
bool Statement::exec(void) {
	assert(this->is_valid());
	this->m_lastcode = SQLITE_OK;
	::sqlite3_stmt *p = this->m_pstmt;
	int rc = ::sqlite3_step(p);
	this->m_lastcode = rc;
	if (rc == SQLITE_DONE) {
		return (true);
	} else if (rc != SQLITE_ROW) {
		this->m_pBase->internal_get_error();
		return (false);
	}
	return (true);
} // exec
bool Statement::next(void) {
	assert(this->is_valid());
	int rc = this->m_lastcode;
	if (rc == SQLITE_DONE) {
		return (false);
	} else if ((rc != SQLITE_OK) && (rc != SQLITE_ROW)) {
		return (false);
	}
	std::vector<std::string> &vals = this->m_vals;
	std::vector<int> & types = this->m_types;
	::sqlite3_stmt *p = this->m_pstmt;
	vals.clear();
	types.clear();
	if (this->m_lastcode == SQLITE_OK) {
		rc = ::sqlite3_step(p);
		this->m_lastcode = rc;
		if ((rc != SQLITE_DONE) && (rc != SQLITE_ROW)) {
			this->m_pBase->internal_get_error();
			return (false);
		}
		if (rc == SQLITE_DONE) {
			return (false);
		}
	}
	int nCount = ::sqlite3_column_count(p);
	if (nCount < 1) {
		return (true);
	}
	vals.resize(nCount);
	types.resize(nCount);
	for (int icol = 0; icol < nCount; ++icol) {
		int ntype = ::sqlite3_column_type(p, icol);
		types[icol] = ntype;
		if (ntype != SQLITE_NULL) {
			const unsigned char *ptext = ::sqlite3_column_text(p, icol);
			std::string sval;
			if (ptext != nullptr) {
				const char *ps = reinterpret_cast<const char *>(ptext);
				size_t n = ::strlen(ps);
				std::string ss(n, ' ');
				std::copy(ps, ps + n, ss.begin());
				sval = ss;
			}
			vals[icol] = sval;
		} // not null
	} // icol
	return (true);
} // next
void Statement::force_close(void) {
	::sqlite3_stmt *p = this->m_pstmt;
	if (p != nullptr) {
		::sqlite3_finalize(p);
		this->m_pstmt = nullptr;
	} // p
} // force_close
bool Statement::close(void) {
	bool bRet = true;
	::sqlite3_stmt *p = this->m_pstmt;
	if (p != nullptr) {
		int rc = ::sqlite3_finalize(p);
		if (rc != SQLITE_OK) {
			bRet = false;
			this->m_pBase->internal_get_error();
		} else {
			this->m_pstmt = nullptr;
		}
	}
	Database *pBase = this->m_pBase;
	if (pBase != nullptr) {
		std::list<PStatement> &olist = pBase->m_stmts;
		for (auto it = olist.begin(); it != olist.end(); ++it) {
			PStatement p0 = *it;
			if (p0 == this) {
				olist.erase(it);
				return (true);
			}
		} // it
	} // pBase
	return (bRet);
} // close
bool Statement::reset(void) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	if (::sqlite3_reset(p) != SQLITE_OK) {
		this->m_pBase->internal_get_error();
		return (false);
	}
	return true;
} // reset
bool Statement::set_parameter_null(int iParam) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	assert(iParam > 0);
	if (::sqlite3_bind_null(p, iParam) != SQLITE_OK) {
		this->m_pBase->internal_get_error();
		return (false);
	}
	return (true);
} // set_parameter_null
bool Statement::set_parameter(int iParam, int ivalue) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	assert(iParam > 0);
	if (::sqlite3_bind_int(p, iParam, ivalue) != SQLITE_OK) {
		this->m_pBase->internal_get_error();
		return (false);
	}
	return (true);
} // set_parameter
bool Statement::set_parameter(int iParam, double dval) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	assert(iParam > 0);
	if (::sqlite3_bind_double(p, iParam, dval) != SQLITE_OK) {
		this->m_pBase->internal_get_error();
		return (false);
	}
	return (true);
} // set_parameter
bool Statement::set_parameter(int iParam, const char *pszVal) {
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	assert(iParam > 0);
	assert(pszVal != nullptr);
	int nbytes = -1;
	if (::sqlite3_bind_text(p, iParam, pszVal, nbytes,
	SQLITE_TRANSIENT) != SQLITE_OK) {
		this->m_pBase->internal_get_error();
		return (false);
	}
	return (true);
} // set_parameter
bool Statement::set_parameter(int iParam, const wchar_t *pwszVal) {
	assert(pwszVal != nullptr);
	std::wstring ss(pwszVal);
	std::string s(ss.length(), ' ');
	std::copy(ss.begin(), ss.end(), s.begin());
	const char *px = s.c_str();
	return this->set_parameter(iParam, px);
} // set_parameter
bool Statement::set_parameter(int iParam, const std::string &sVal) {
	return this->set_parameter(iParam, sVal.c_str());
} // set_parameter
bool Statement::set_parameter(int iParam, const std::wstring &sVal) {
	return this->set_parameter(iParam, sVal.c_str());
} // set_parameter
Statement::~Statement() {
	this->close();
}

} /* namespace sqlite */
