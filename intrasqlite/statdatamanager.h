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
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVARPAIR,
			class ALLOCINDIVPAIR>
	bool remove_dataset(
			const intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
					ALLOCINDIVPAIR> &cur) {
		assert(this->is_valid());
		if (!cur.is_removeable()) {
			return (false);
		}
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
		if (!stmt.exec()) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // update_dataset
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVARPAIR,
			class ALLOCINDIVPAIR>
	bool update_dataset(
			const intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
					ALLOCINDIVPAIR> &cur) {
		assert(this->is_valid());
		if (!cur.is_updateable()) {
			return (false);
		}
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
		if (!stmt.exec()) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // update_dataset
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVARPAIR,
			class ALLOCINDIVPAIR>
	bool insert_dataset(
			const intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
					ALLOCINDIVPAIR> &cur) {
		assert(this->is_valid());
		if (!cur.is_valid()) {
			return (false);
		}
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
		if (!stmt.exec()) {
			this->rollback_transaction();
			return (false);
		}
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // insert_dataset
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVARPAIR,
			class ALLOCINDIVPAIR>
	bool get_dataset_by_sigle(const TSTRING &xSigle,
			intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
					ALLOCINDIVPAIR> &cur) {
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
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		int n = stmt.cols();
		assert(n >= 5);
		{
			sqlite::DbValue v;
			if (stmt.col_value(0, v)) {
				int nId = v.int_value();
				cur.id(nId);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(1, v)) {
				int nVersion = v.int_value();
				cur.version(nVersion);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(2, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.sigle(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(3, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.name(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(4, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.description(s);
				}
			}
		}
		return (true);
	} // get_dataset_by_sigle
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVARPAIR,
			class ALLOCINDIVPAIR>
	bool get_dataset_by_id(int xId,
			intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
					ALLOCINDIVPAIR> &cur) {
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_BY_ID);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (!stmt.set_parameter(1, xId)) {
			return (false);
		}
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		int n = stmt.cols();
		assert(n >= 5);
		{
			sqlite::DbValue v;
			if (stmt.col_value(0, v)) {
				int nId = v.int_value();
				cur.id(nId);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(1, v)) {
				int nVersion = v.int_value();
				cur.version(nVersion);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(2, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.sigle(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(3, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.name(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(4, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.description(s);
				}
			}
		}
		return (true);
	} // get_dataset_by_id
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVARPAIR,
			class ALLOCINDIVPAIR, class ALLOCVEC>
	bool get_all_datasets(
			std::vector<
					intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
							ALLOCINDIVPAIR>, ALLOCVEC> &oVec) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_ALL_DATASETS);
		if (!stmt.is_valid()) {
			return (false);
		}
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			intra::StatDataset<TSTRING> cur;
			{
				sqlite::DbValue v;
				if (stmt.col_value(0, v)) {
					int nId = v.int_value();
					cur.id(nId);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(1, v)) {
					int nVersion = v.int_value();
					cur.version(nVersion);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(2, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.description(s);
					}
				}
			}
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
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
	/////////////////////////////////////
	static const char *SQL_FIND_DATASET_VARIABLES;
	static const char *SQL_VARIABLE_BY_DATASET_AND_SIGLE;
};
////////////////////////////////////
} /* namespace intrasqlite */
#endif /* STATDATAMANAGER_H_ */
