/*
 * statement.h
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_
//////////////////////////////
#include "database.h"
#include "dbvalue.h"
////////////////////////////
#include <vector>
#include <algorithm>
///////////////////////////////
namespace sqlite {
//////////////////////////////////
class Statement {
	friend class Database;
private:
	Database *m_pBase;
	::sqlite3_stmt *m_pstmt;
	int m_lastcode;
	std::vector<DbValue> m_vals;
	std::vector<std::string> m_names;
public:
	Statement(Database &oBase, const char *pszSQL);
	Statement(Database *pBase, const char *pszSQL);
	Statement(Database &oBase, const wchar_t *pszSQL);
	Statement(Database *pBase, const wchar_t *pszSQL);
	Statement(Database &oBase, const std::string &s);
	Statement(Database *pBase, const std::string &s);
	Statement(Database &oBase, const std::wstring &s);
	Statement(Database *pBase, const std::wstring &s);
	virtual ~Statement();
public:
	bool is_valid(void) const;
	bool close(void);

	bool reset(void);
	//
	bool set_parameter_null(int iParam);
	bool set_parameter(int iParam, int ivalue);
	bool set_parameter(int iParam, double dval);
	bool set_parameter(int iParam, const char *pszVal);
	bool set_parameter(int iParam, const wchar_t *pwszVal);
	bool set_parameter(int iParam, const std::string &sVal);
	bool set_parameter(int iParam, const std::wstring &sVal);
	bool set_parameter(int iParam, const Blob &oBlob);
	//
	bool set_parameter_null(const std::string &sname);
	bool set_parameter_null(const std::wstring &sname);
	bool set_parameter(const std::string &sname, int ivalue);
	bool set_parameter(const std::wstring &sname, int ivalue);
	bool set_parameter(const std::string &sname, double dval);
	bool set_parameter(const std::wstring &sname, double dval);
	bool set_parameter(const std::string &sname, const std::string &sval);
	bool set_parameter(const std::wstring &sname, const std::wstring &sval);
	bool set_parameter(const std::string &sname, const Blob &oBlob);
	bool set_parameter(const std::wstring &sname, const Blob &oBlob);
	//
	bool exec(void);
	bool next(void);
	int cols(void) const {
		return ((int) this->m_vals.size());
	}
	bool has_values(void) const {
		return (this->m_vals.size() > 0);
	}
	bool col_name(int icol, std::string &sRet) const {
		bool bRet = false;
		const std::vector<std::string> &vv = this->m_names;
		if ((icol >= 0) && ((size_t) icol < vv.size())) {
			sRet = vv[icol];
			bRet = true;
		}
		return (bRet);
	} // col_name
	bool col_name(int icol, std::wstring &sRet) const {
		bool bRet = false;
		const std::vector<std::string> &vv = this->m_names;
		if ((icol >= 0) && ((size_t) icol < vv.size())) {
			std::string sx = vv[icol];
			std::wstring ss(sx.length(), L' ');
			std::copy(sx.begin(), sx.end(), ss.begin());
			sRet = ss;
			bRet = true;
		}
		return (bRet);
	} // col_name
	bool col_value(int icol, DbValue &val) const {
		bool bRet = false;
		const std::vector<DbValue> &vv = this->m_vals;
		if ((icol >= 0) && ((size_t) icol < vv.size())) {
			val = vv[icol];
			bRet = true;
		}
		return (bRet);
	} // col_value
	bool col_value(const std::string &sname, DbValue &val) const {
		bool bRet = false;
		const std::vector<std::string> &names = this->m_names;
		const int n = (int) names.size();
		for (int i = 0; i < n; ++i) {
			std::string s = names[i];
			if (s == sname) {
				return (this->col_value(i, val));
			}
		} // i
		return (bRet);
	} // col_value
	bool col_value(const std::wstring &sname, DbValue &val) const {
			bool bRet = false;
			std::string ws(sname.length(),' ');
			std::copy(sname.begin(),sname.end(),ws.begin());
			const std::vector<std::string> &names = this->m_names;
			const int n = (int) names.size();
			for (int i = 0; i < n; ++i) {
				std::string s = names[i];
				if (ws == s) {
					return (this->col_value(i, val));
				}
			} // i
			return (bRet);
		} // col_value
private:
	void force_close(void);
private:
	// no implementation
	Statement(const Statement &other);
	Statement & operator=(const Statement &other);
};

} /* namespace sqlite */
#endif /* STATEMENT_H_ */
