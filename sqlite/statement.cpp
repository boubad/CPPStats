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
	std::vector<DbValue> &vals = this->m_vals;
	vals.clear();
	std::vector<std::string> &names = this->m_names;
	names.clear();
	::sqlite3_stmt *p = this->m_pstmt;
	this->m_lastcode = SQLITE_OK;
	int rc = ::sqlite3_step(p);
	this->m_lastcode = rc;
	if (rc == SQLITE_DONE) {
		return (true);
	} else if (rc != SQLITE_ROW) {
		this->m_pBase->internal_get_error();
		return (false);
	} else {
		int nCount = ::sqlite3_column_count(p);
		if (nCount < 1) {
			return (true);
		}
		names.resize(nCount);
		vals.resize(nCount);
		for (int icol = 0; icol < nCount; ++icol) {
			const char *pn = ::sqlite3_column_name(p, icol);
			if (pn != nullptr) {
				std::string s1 = pn;
				names[icol] = s1;
			}
			int ntype = ::sqlite3_column_type(p, icol);
			switch (ntype) {
			case SQLITE_INTEGER: {
				int ival = ::sqlite3_column_int(p, icol);
				vals[icol] = DbValue(ival);
			}
				break;
			case SQLITE_FLOAT: {
				double dval = ::sqlite3_column_double(p, icol);
				vals[icol] = DbValue(dval);
			}
				break;
			case SQLITE_TEXT: {
				std::string sval;
				const unsigned char *ptext = ::sqlite3_column_text(p, icol);
				if (ptext != nullptr) {
					const char *ps = reinterpret_cast<const char *>(ptext);
					size_t n = ::strlen(ps);
					std::string ss(n, ' ');
					std::copy(ps, ps + n, ss.begin());
					sval = ss;
				}
				vals[icol] = DbValue(sval);
			}
				break;
			case SQLITE_BLOB: {
				Blob oBlob;
				const void *p0 = ::sqlite3_column_blob(p, icol);
				int nSize = ::sqlite3_column_bytes(p, icol);
				if ((p0 != nullptr) && (nSize > 0)) {
					const byte *pData = reinterpret_cast<const byte *>(p0);
					oBlob.data(pData, nSize);
				} //nSize
				vals[icol] = DbValue(oBlob);
			}
				break;
			default:
				break;
			} // nType
		} // icol
	}
	return (true);
} // exec
bool Statement::next(void) {
	assert(this->is_valid());
	int rc = this->m_lastcode;
	if (rc != SQLITE_ROW) {
		return (false);
	}
	::sqlite3_stmt *p = this->m_pstmt;
	rc = ::sqlite3_step(p);
	this->m_lastcode = rc;
	if (rc != SQLITE_ROW) {
		return (false);
	}
	int nCount = ::sqlite3_column_count(p);
	if (nCount < 1) {
		return (true);
	}
	std::vector<DbValue> &vals = this->m_vals;
	if (vals.size() != (size_t) nCount) {
		vals.resize(nCount);
	}
	for (int icol = 0; icol < nCount; ++icol) {
		DbValue v;
		vals[icol] = v;
		int ntype = ::sqlite3_column_type(p, icol);
		switch (ntype) {
		case SQLITE_INTEGER: {
			int ival = ::sqlite3_column_int(p, icol);
			vals[icol] = DbValue(ival);
		}
			break;
		case SQLITE_FLOAT: {
			double dval = ::sqlite3_column_double(p, icol);
			vals[icol] = DbValue(dval);
		}
			break;
		case SQLITE_TEXT: {
			std::string sval;
			const unsigned char *ptext = ::sqlite3_column_text(p, icol);
			if (ptext != nullptr) {
				const char *ps = reinterpret_cast<const char *>(ptext);
				size_t n = ::strlen(ps);
				std::string ss(n, ' ');
				std::copy(ps, ps + n, ss.begin());
				sval = ss;
			}
			vals[icol] = DbValue(sval);
		}
			break;
		case SQLITE_BLOB: {
			Blob oBlob;
			const void *p0 = ::sqlite3_column_blob(p, icol);
			int nSize = ::sqlite3_column_bytes(p, icol);
			if ((p0 != nullptr) && (nSize > 0)) {
				const byte *pData = reinterpret_cast<const byte *>(p0);
				oBlob.data(pData, nSize);
			} //nSize
			vals[icol] = DbValue(oBlob);
		}
			break;
		default:
			break;
		} // nType
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
	this->m_names.clear();
	this->m_vals.clear();
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
bool Statement::set_parameter(int iParam, const Blob &oBlob) {
	size_t nSize = oBlob.size();
	const void *pData = (const void *)oBlob.data();
	if ((nSize < 1) || (pData == nullptr)){
		return (false);
	}
	assert(this->m_pBase != nullptr);
	assert(this->m_pBase->is_open());
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	assert(iParam > 0);
	int nbytes = (size_t)(nSize * sizeof(byte));
	if (::sqlite3_bind_blob(p, iParam, pData, nbytes,
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
///////////////////////////////
bool Statement::set_parameter_null(const std::string &sname) {
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
	if (iParam < 1) {
		return (false);
	}
	return this->set_parameter_null(iParam);
} // set_paraemter_null
bool Statement::set_parameter_null(const std::wstring &sname) {
	std::string ss(sname.length(), ' ');
	std::copy(sname.begin(), sname.end(), ss.begin());
	return this->set_parameter_null(ss);
} // set_parameter_null
bool Statement::set_parameter(const std::string &sname, int ivalue) {
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
	if (iParam < 1) {
		return (false);
	}
	return this->set_parameter(iParam, ivalue);
} // set_parameter
bool Statement::set_parameter(const std::wstring &sname, int ivalue) {
	std::string ss(sname.length(), ' ');
	std::copy(sname.begin(), sname.end(), ss.begin());
	return this->set_parameter(ss, ivalue);
}
bool Statement::set_parameter(const std::string &sname, double dval) {
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
	if (iParam < 1) {
		return (false);
	}
	return this->set_parameter(iParam, dval);
}
bool Statement::set_parameter(const std::string &sname, const Blob &oBlob) {
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
	if (iParam < 1) {
		return (false);
	}
	return this->set_parameter(iParam, oBlob);
}
bool Statement::set_parameter(const std::wstring &sname, double dval) {
	std::string ss(sname.length(), ' ');
	std::copy(sname.begin(), sname.end(), ss.begin());
	return this->set_parameter(ss, dval);
}
bool Statement::set_parameter(const std::wstring &sname, const Blob &oBlob) {
	std::string ss(sname.length(), ' ');
	std::copy(sname.begin(), sname.end(), ss.begin());
	return this->set_parameter(ss, oBlob);
}
bool Statement::set_parameter(const std::string &sname,
		const std::string &sval) {
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	int iParam = ::sqlite3_bind_parameter_index(p, sname.c_str());
	if (iParam < 1) {
		return (false);
	}
	return this->set_parameter(iParam, sval);
}
bool Statement::set_parameter(const std::wstring &sname,
		const std::wstring &sval) {
	::sqlite3_stmt *p = this->m_pstmt;
	assert(p != nullptr);
	std::string ss(sname.length(), ' ');
	std::copy(sname.begin(), sname.end(), ss.begin());
	int iParam = ::sqlite3_bind_parameter_index(p, ss.c_str());
	if (iParam < 1) {
		return (false);
	}
	return this->set_parameter(iParam, sval);
}
///////////////////////////////////
Statement::~Statement() {
	this->close();
}

} /* namespace sqlite */
