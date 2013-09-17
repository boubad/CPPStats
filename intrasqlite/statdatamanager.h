/*
 * statdatamanager.h
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#ifndef STATDATAMANAGER_H_
#define STATDATAMANAGER_H_
/////////////////////////////////
#include <memory>
#include <string>
#include <sstream>
////////////////////////////////////////
#include <boost/algorithm/string.hpp>
///////////////////////////////////////
#include "../sqlite/database.h"
#include "../sqlite/statement.h"
//////////////////////////////////
#include "../statdataset/statdataset.h"
//////////////////////////////////
namespace intrasqlite {
/////////////////////////////////
class StatDataManager {
private:
	bool m_intransaction;
	std::unique_ptr<sqlite::Database> m_database;
public:
	StatDataManager();
	StatDataManager(const std::string &s);
	StatDataManager(const std::wstring &ss);
	virtual ~StatDataManager();
public:
	bool begin_transaction(void);
	bool commit_transaction(void);
	bool rollback_transaction(void);
public:
	template<class TSTRING>
	bool remove_dataset(const intra::StatDataset<TSTRING> &cur) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_DATASET);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(1, cur.id())) {
			this->rollback_transaction();
			return (false);
		}
		bool bDone = false;
		if (!stmt.exec(bDone)) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // update_dataset
	template<class TSTRING>
	bool update_dataset(const intra::StatDataset<TSTRING> &cur) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_UPDATE_DATASET);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(1, cur.sigle())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(2, cur.name())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(3, cur.description())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(4, cur.id())) {
			this->rollback_transaction();
			return (false);
		}
		bool bDone = false;
		if (!stmt.exec(bDone)) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // update_dataset
	template<class TSTRING>
	bool insert_dataset(const intra::StatDataset<TSTRING> &cur) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_INSERT_DATASET);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(1, cur.sigle())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(2, cur.name())) {
			this->rollback_transaction();
			return (false);
		}
		if (!stmt.set_parameter(3, cur.description())) {
			this->rollback_transaction();
			return (false);
		}
		bool bDone = false;
		if (!stmt.exec(bDone)) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // insert_dataset
	template<class TSTRING, class SSTREAM>
	bool get_dataset_by_sigle(const TSTRING &xSigle,
			intra::StatDataset<TSTRING> &cur, SSTREAM &instream) {
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_SIGLE);
		if (!stmt.is_valid()) {
			return (false);
		}
		TSTRING sx = boost::to_upper_copy(boost::trim_copy(xSigle));
		if (!stmt.set_parameter(1, sx.c_str())) {
			return (false);
		}
		bool bDone = false;
		if (!stmt.exec(bDone)) {
			return (false);
		}
		int n = stmt.cols();
		assert(n >= 5);
		{
			TSTRING s;
			stmt.col_value(0, s);
			int nId = 0;
			SSTREAM in(s);
			in >> nId;
			cur.id(nId);
		}
		{
			TSTRING s;
			stmt.col_value(1, s);
			int nId = 0;
			SSTREAM in(s);
			in >> nId;
			cur.version(nId);
		}
		{
			TSTRING s;
			stmt.col_value(2, s);
			cur.sigle(s);
		}
		{
			TSTRING s;
			stmt.col_value(3, s);
			cur.name(s);
		}
		{
			TSTRING s;
			stmt.col_value(4, s);
			cur.description(s);
		}
		return (true);
	} // get_dataset_by_sigle
	template<class TSTRING, class SSTREAM>
	bool get_dataset_by_id(int xId, intra::StatDataset<TSTRING> &cur,
			SSTREAM &instream) {
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_ID);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (!stmt.set_parameter(1, xId)) {
			return (false);
		}
		bool bDone = false;
		if (!stmt.exec(bDone)) {
			return (false);
		}
		int n = stmt.cols();
		assert(n >= 5);
		{
			TSTRING s;
			stmt.col_value(0, s);
			int nId = 0;
			SSTREAM in(s);
			in >> nId;
			cur.id(nId);
		}
		{
			TSTRING s;
			stmt.col_value(1, s);
			int nId = 0;
			SSTREAM in(s);
			in >> nId;
			cur.version(nId);
		}
		{
			TSTRING s;
			stmt.col_value(2, s);
			cur.sigle(s);
		}
		{
			TSTRING s;
			stmt.col_value(3, s);
			cur.name(s);
		}
		{
			TSTRING s;
			stmt.col_value(4, s);
			cur.description(s);
		}
		return (true);
	} // get_dataset_by_id
	template<class TSTRING, class ALLOCVEC, class SSTREAM>
	bool get_all_datasets(
			std::vector<intra::StatDataset<TSTRING>, ALLOCVEC> &oVec,
			SSTREAM &inStream) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_ALL_DATASETS);
		if (!stmt.is_valid()) {
			return (false);
		}
		bool bDone = false;
		if (!stmt.exec(bDone)) {
			return (false);
		}
		int n = stmt.cols();
		if (n < 1) {
			return (true);
		}
		do {
			intra::StatDataset<TSTRING> cur;
			int n = stmt.cols();
			assert(n >= 5);
			{
				TSTRING s;
				stmt.col_value(0, s);
				int nId = 0;
				SSTREAM in(s);
				in >> nId;
				cur.id(nId);
			}
			{
				TSTRING s;
				stmt.col_value(1, s);
				int nId = 0;
				SSTREAM in(s);
				in >> nId;
				cur.version(nId);
			}
			{
				TSTRING s;
				stmt.col_value(2, s);
				cur.sigle(s);
			}
			{
				TSTRING s;
				stmt.col_value(3, s);
				cur.name(s);
			}
			{
				TSTRING s;
				stmt.col_value(4, s);
				cur.description(s);
			}
			oVec.push_back(cur);
			if (!stmt.next(bDone)){
				break;
			}
		} while (!bDone);
		return (true);
	} // get_all_datasets
public:
	bool open(const std::string &filename);
	bool open(const std::wstring &wfilename);
	bool is_valid(void) const;
	bool close(void);
private:
	bool check_schema(void);
private:
	static const char *SQL_BEGIN_TRANSACTION;
	static const char *SQL_COMMIT_TRANSACTION;
	static const char *SQL_ROLLBACK_TRANSACTION;
	////////////////////////////////////////
	static const char *SQL_CREATE_DATASET;
	static const char *SQL_CREATE_VARIABLE;
	static const char *SQL_CREATE_INDIV;
	static const char *SQL_CREATE_VALUE;
	static const char *CREATE_SQL[];
	/////////////////////////////////////
	static const char *SQL_FIND_ALL_DATASETS;
	static const char *SQL_FIND_DATASET_BY_ID;
	static const char *SQL_FIND_DATASET_BY_SIGLE;
	static const char *SQL_INSERT_DATASET;
	static const char *SQL_UPDATE_DATASET;
	static const char *SQL_REMOVE_DATASET;
};
////////////////////////////////////
} /* namespace intrasqlite */
#endif /* STATDATAMANAGER_H_ */
