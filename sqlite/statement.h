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
////////////////////////////
#include <vector>
///////////////////////////////
namespace sqlite {
//////////////////////////////////
class Statement {
	friend class Database;
private:
	Database *m_pBase;
	::sqlite3_stmt *m_pstmt;
	int m_lastcode;
	std::vector<std::string> m_vals;
	std::vector<int> m_types;
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
	bool set_parameter_null(int iParam);
	bool set_parameter(int iParam, int ivalue);
	bool set_parameter(int iParam, double dval);
	bool set_parameter(int iParam, const char *pszVal);
	bool set_parameter(int iParam, const wchar_t *pwszVal);
	bool set_parameter(int iParam, const std::string &sVal);
	bool set_parameter(int iParam, const std::wstring &sVal);
	bool exec(bool &bDone);
	bool next(bool &bDone);
	int cols(void) const {
		return ((int) this->m_vals.size());
	}
	int col_type(int icol) const {
		assert((icol >= 0) && ((size_t )icol < this->m_types.size()));
		return (this->m_types[icol]);
	} // coltype
	void col_value(int icol, std::string &s) const {
		assert((icol >= 0) && ((size_t )icol < this->m_vals.size()));
		s = this->m_vals[icol];
	} // col_value
	void col_value(int icol, std::wstring &ss) const {
		assert((icol >= 0) && ((size_t )icol < this->m_vals.size()));
		std::string s = this->m_vals[icol];
		std::wstring wx(ss.length(), L' ');
		std::copy(s.begin(), s.end(), wx.begin());
		ss = wx;
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
