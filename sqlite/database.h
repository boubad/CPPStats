/*
 * database.h
 *
 *  Created on: 16 sept. 2013
 *      Author: Boubacar
 */

#ifndef DATABASE_H_
#define DATABASE_H_
///////////////////////
#include "sqlite3.h"
///////////////////////////
#include <cassert>
#include <string>
#include <list>
#include <algorithm>
//////////////////////////
namespace sqlite {
////////////////////////////////////////
class Statement;
typedef Statement *PStatement;
////////////////////////////////////////
class Database {
	friend class Statement;
private:
	int m_errorcode;
	::sqlite3 *m_pDb;
	std::string m_errorstring;
	std::list<PStatement> m_stmts;
public:
	Database();
	Database(const char *pszFilename);
	Database(const std::string &filename);
	Database(const wchar_t *pwszFilename);
	Database(const std::wstring &wfilename);
	virtual ~Database();
public:
	bool open(const char *pszFilename);
	bool open(const wchar_t *pszFilename);
	bool open(const std::string &filename);
	bool open(const std::wstring &wfilename);
	inline bool is_open(void) const {
		return (this->m_pDb != nullptr);
	} // is_open
	inline bool has_error(void) const {
		return (this->m_errorcode != SQLITE_OK);
	} // has_error
	inline bool get_last_error(std::string &sErr) const {
		if (this->m_errorcode != SQLITE_OK) {
			sErr = this->m_errorstring;
			return (true);
		}
		return (false);
	} // get_last_error
	inline bool get_last_error(std::wstring &sErr) const {
		if (this->m_errorcode != SQLITE_OK) {
			const std::string &s = this->m_errorstring;
			std::wstring ss(s.length(), L' ');
			std::copy(s.begin(), s.end(), ss.begin());
			sErr = ss;
			return (true);
		}
		return (false);
	} // get_last_error
	bool close(void);
	bool exec_sql(const char *pszSQL);
protected:
	virtual void prepare_close(void);
	void internal_clear_error(void);
	void internal_get_error(void);
private:
	// no implementation
	Database(const Database &other);
	Database & operator=(const Database &other);
};
///////////////////////////////////
} /* namespace sqlite */
#endif /* DATABASE_H_ */
