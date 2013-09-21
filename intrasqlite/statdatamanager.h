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
	void convert_value(const intra::StatValue &s, const std::string &vartype,
			boost::any &v) {
		std::string sx = boost::to_lower_copy(boost::trim_copy(vartype));
		if (sx == "bool") {
			v = boost::any(s.bool_value());
		} else if (sx == "short") {
			v = boost::any(s.short_value());
		} else if (sx == "integer") {
			v = boost::any(s.int_value());
		} else if (sx == "float") {
			v = boost::any(s.float_value());
		} else if (sx == "double") {
			v = boost::any(s.double_value());
		} else if (sx == "string") {
			std::string ss;
			s.string_value(ss);
			v = boost::any(ss);
		}
	} //convert_value
	template<class ALLOCVAL>
	bool remove_values(const std::vector<intra::StatValue, ALLOCVAL> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_VALUE);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::StatValue &cur = *it;
			int nId = cur.id();
			if (nId != 0) {
				stmt.reset();
				stmt.set_parameter(1, nId);
				if (!stmt.exec()) {
					this->rollback_transaction();
					return (false);
				}
			}
		} // it
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // remove_values
	template<class ALLOCVAL>
	bool maintains_values(const std::vector<intra::StatValue, ALLOCVAL> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VALUE);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_VALUE);
		if ((!stmtUpdate.is_valid()) || (!stmtInsert.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::StatValue &cur = *it;
			int nId = cur.id();
			int nVarId = cur.variable_id();
			int nIndivId = cur.indiv_id();
			std::string sval;
			cur.string_value(sval);
			if (cur.is_updateable()) {
				stmtUpdate.reset();
				stmtUpdate.set_parameter(1, sval);
				stmtUpdate.set_parameter(2, nId);
				if (!stmtUpdate.exec()) {
					this->rollback_transaction();
					return (false);
				}
			} else if (cur.is_valid()) {
				stmtInsert.reset();
				stmtInsert.set_parameter(1, nVarId);
				stmtInsert.set_parameter(2, nIndivId);
				stmtInsert.set_parameter(3, sval);
				if (!stmtInsert.exec()) {
					this->rollback_transaction();
					return (false);
				}
			}
		} // it
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // maintains_values
	bool get_value_by_variable_indiv(int nVarId, int nIndivId,
			intra::StatValue &cur) {
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_VALUES_BY_VARIABLE_INDIV);
		sqlite::Statement stmtVarType(pBase, SQL_FIND_VARIABLE_TYPE);
		if ((!stmt.is_valid()) || (!stmtVarType.is_valid())) {
			return (false);
		}
		stmt.set_parameter(1, nVarId);
		stmt.set_parameter(2, nIndivId);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		std::string vartype;
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
				int nx = v.int_value();
				cur.variable_id(nx);
				stmtVarType.set_parameter(1, nx);
				if (!stmtVarType.exec()) {
					return (false);
				}
				if (!stmtVarType.has_values()) {
					return (false);
				}
				sqlite::DbValue vv;
				stmtVarType.col_value(0, vv);
				vv.string_value(vartype);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(3, v)) {
				int nx = v.int_value();
				cur.indiv_id(nx);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(4, v)) {
				boost::any vx = v.value();
				intra::StatValue x(vx);
				boost::any vy;
				this->convert_value(x, vartype, vy);
				cur.value(vy);
			}
		}
		return (true);
	} // get_dataset_indivs
	template<class ALLOCVEC>
	bool get_dataset_values(int nDatasetId,
			std::vector<intra::StatValue, ALLOCVEC> &oVec, int skip,
			int taken) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_VALUES);
		sqlite::Statement stmtVarType(pBase, SQL_FIND_VARIABLE_TYPE);
		std::map<int, std::string> oMap;
		if ((!stmt.is_valid()) || (!stmtVarType.is_valid())) {
			return (false);
		}
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, taken);
		stmt.set_parameter(3, skip);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			intra::StatValue cur;
			std::string vartype;
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
					int nx = v.int_value();
					cur.variable_id(nx);
					if (oMap.find(nx) != oMap.end()) {
						vartype = oMap[nx];
					} else {
						stmtVarType.reset();
						stmtVarType.set_parameter(1, nx);
						if (!stmtVarType.exec()) {
							return (false);
						}
						if (!stmtVarType.has_values()) {
							return (false);
						}
						sqlite::DbValue vv;
						stmtVarType.col_value(0, vv);
						vv.string_value(vartype);
						if (!vartype.empty()) {
							oMap[nx] = vartype;
						}
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					int nx = v.int_value();
					cur.indiv_id(nx);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					boost::any vx = v.value();
					intra::StatValue x(vx);
					boost::any vy;
					this->convert_value(x, vartype, vy);
					cur.value(vy);
				}
			}
			oVec.push_back(cur);
			if (!stmt.next()) {
				break;
			}
		} // values
		return (true);
	} // get_dataset_indivs
	bool get_dataset_values_count(int nId, int &nCount) {
		assert(this->is_valid());
		nCount = 0;
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_VALUES_COUNT);
		if (!stmt.is_valid()) {
			return false;
		}
		stmt.set_parameter(1, nId);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		sqlite::DbValue v;
		if (stmt.col_value(0, v)) {
			nCount = v.int_value();
		}
		return (true);
	} // get_dataset_values_count
	  //
	template<class TSTRING, class ALLOCIND>
	bool remove_indivs(
			const std::vector<intra::StatIndiv<TSTRING>, ALLOCIND> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_INDIV);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::StatIndiv<TSTRING> &cur = *it;
			int nId = cur.id();
			if (nId != 0) {
				stmt.reset();
				stmt.set_parameter(1, nId);
				if (!stmt.exec()) {
					this->rollback_transaction();
					return (false);
				}
			}
		} // it
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // remove_indivs
	template<class TSTRING, class ALLOCIND>
	bool maintains_indivs(
			const std::vector<intra::StatIndiv<TSTRING>, ALLOCIND> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_INDIV);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_INDIV);
		if ((!stmtUpdate.is_valid()) || (!stmtInsert.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::StatIndiv<TSTRING> &cur = *it;
			int nId = cur.id();
			int nDatasetId = cur.dataset_id();
			TSTRING sigle = cur.sigle();
			if (cur.is_updateable()) {
				stmtUpdate.reset();
				stmtUpdate.set_parameter(1, sigle);
				stmtUpdate.set_parameter(2, cur.name());
				stmtUpdate.set_parameter(3, cur.description());
				stmtUpdate.set_parameter(4, nId);
				if (!stmtUpdate.exec()) {
					this->rollback_transaction();
					return (false);
				}
			} else if (cur.is_valid()) {
				stmtInsert.reset();
				stmtInsert.set_parameter(1, nDatasetId);
				stmtInsert.set_parameter(2, sigle);
				stmtInsert.set_parameter(3, cur.name());
				stmtInsert.set_parameter(4, cur.description());
				if (!stmtInsert.exec()) {
					this->rollback_transaction();
					return (false);
				}
			}
		} // it
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // maintains_indivs
	template<class TSTRING>
	bool get_indiv_by_dataset_sigle(int nDatasetId, const TSTRING &xSigle,
			intra::StatIndiv<TSTRING> &cur) {
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_INDIV_BY_DATASET_AND_SIGLE);
		if (!stmt.is_valid()) {
			return (false);
		}
		TSTRING sigle = boost::to_upper_copy(boost::trim_copy(xSigle));
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, sigle);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
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
				int nx = v.int_value();
				cur.dataset_id(nx);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(3, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.sigle(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(4, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.name(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(5, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.description(s);
				}
			}
		}
		return (true);
	} // get_indiv_by_dataset_sigle
	template<class TSTRING, class ALLOCVEC>
	bool get_dataset_indivs(int nDatasetId,
			std::vector<intra::StatIndiv<TSTRING>, ALLOCVEC> &oVec) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_INDIVS);
		if (!stmt.is_valid()) {
			return (false);
		}
		stmt.set_parameter(1, nDatasetId);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			intra::StatIndiv<TSTRING> cur;
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
					int nx = v.int_value();
					cur.dataset_id(nx);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(5, v)) {
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
	} // get_dataset_indivs
	bool get_dataset_indivs_count(int nId, int &nCount) {
		assert(this->is_valid());
		nCount = 0;
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_INDIVS_COUNT);
		if (!stmt.is_valid()) {
			return false;
		}
		stmt.set_parameter(1, nId);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
		sqlite::DbValue v;
		if (stmt.col_value(0, v)) {
			nCount = v.int_value();
		}
		return (true);
	} // get_dataset_indivs_count
	  //
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVAR>
	bool remove_variables(
			const std::vector<intra::StatVariable<TSTRING, ALLOCANYPAIR>,
					ALLOCVAR> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_REMOVE_VARIABLE);
		if (!stmt.is_valid()) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::StatVariable<TSTRING, ALLOCANYPAIR> &cur = *it;
			int nId = cur.id();
			if (nId != 0) {
				stmt.reset();
				stmt.set_parameter(1, nId);
				if (!stmt.exec()) {
					this->rollback_transaction();
					return (false);
				}
			}
		} // it
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // remove_variables
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVAR>
	bool maintains_variables(
			const std::vector<intra::StatVariable<TSTRING, ALLOCANYPAIR>,
					ALLOCVAR> &oVec) {
		assert(this->is_valid());
		if (!this->begin_transaction()) {
			return (false);
		}
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmtInsert(pBase, SQL_INSERT_VARIABLE);
		sqlite::Statement stmtUpdate(pBase, SQL_UPDATE_VARIABLE);
		if ((!stmtUpdate.is_valid()) || (!stmtInsert.is_valid())) {
			this->rollback_transaction();
			return (false);
		}
		for (auto it = oVec.begin(); it != oVec.end(); ++it) {
			const intra::StatVariable<TSTRING, ALLOCANYPAIR> &cur = *it;
			int nId = cur.id();
			int nDatasetId = cur.dataset_id();
			TSTRING sigle = cur.sigle();
			int nz = (cur.is_categvar()) ? 1 : 0;
			if (cur.is_updateable()) {
				stmtUpdate.reset();
				stmtUpdate.set_parameter(1, sigle);
				stmtUpdate.set_parameter(2, cur.var_type());
				stmtUpdate.set_parameter(3, nz);
				stmtUpdate.set_parameter(4, cur.name());
				stmtUpdate.set_parameter(5, cur.description());
				stmtUpdate.set_parameter(6, nId);
				if (!stmtUpdate.exec()) {
					this->rollback_transaction();
					return (false);
				}
			} else if (cur.is_valid()) {
				stmtInsert.reset();
				stmtInsert.set_parameter(1, nDatasetId);
				stmtInsert.set_parameter(2, sigle);
				stmtInsert.set_parameter(3, cur.var_type());
				stmtInsert.set_parameter(4, nz);
				stmtInsert.set_parameter(5, cur.name());
				stmtInsert.set_parameter(6, cur.description());
				if (!stmtInsert.exec()) {
					this->rollback_transaction();
					return (false);
				}
			}
		} // it
		if (!this->commit_transaction()) {
			this->rollback_transaction();
			return (false);
		}
		return (true);
	} // maintains_variables
	template<class TSTRING, class ALLOCANYPAIR>
	bool get_variable_by_dataset_and_sigle(int nDatasetId,
			const TSTRING &xSigle,
			intra::StatVariable<TSTRING, ALLOCANYPAIR> &cur) {
		assert(this->is_valid());
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_VARIABLE_BY_DATASET_AND_SIGLE);
		if (!stmt.is_valid()) {
			return (false);
		}
		TSTRING sx = boost::to_upper_copy(boost::trim_copy(xSigle));
		stmt.set_parameter(1, nDatasetId);
		stmt.set_parameter(2, sx);
		if (!stmt.exec()) {
			return (false);
		}
		if (!stmt.has_values()) {
			return (false);
		}
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
				int nx = v.int_value();
				cur.dataset_id(nx);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(3, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.sigle(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(4, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.var_type(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(5, v)) {
				int nx = v.int_value();
				bool b = (nx != 0) ? true : false;
				cur.is_categvar(b);
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(6, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.name(s);
				}
			}
		}
		{
			sqlite::DbValue v;
			if (stmt.col_value(7, v)) {
				TSTRING s;
				if (v.string_value(s)) {
					cur.description(s);
				}
			}
		}
		return (true);
	} // get_variable_by_dataset_and_sigle
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVAR>
	bool get_dataset_variables(int nDatasetId,
			std::vector<intra::StatVariable<TSTRING, ALLOCANYPAIR>, ALLOCVAR> &oVec) {
		assert(this->is_valid());
		oVec.clear();
		sqlite::Database *pBase = this->m_database.get();
		sqlite::Statement stmt(pBase, SQL_FIND_DATASET_VARIABLES);
		if (!stmt.is_valid()) {
			return (false);
		}
		stmt.set_parameter(1, nDatasetId);
		if (!stmt.exec()) {
			return (false);
		}
		while (stmt.has_values()) {
			intra::StatVariable<TSTRING, ALLOCANYPAIR> cur;
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
					int nx = v.int_value();
					cur.dataset_id(nx);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(3, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.sigle(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(4, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.var_type(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(5, v)) {
					int nx = v.int_value();
					bool b = (nx != 0) ? true : false;
					cur.is_categvar(b);
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(6, v)) {
					TSTRING s;
					if (v.string_value(s)) {
						cur.name(s);
					}
				}
			}
			{
				sqlite::DbValue v;
				if (stmt.col_value(7, v)) {
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
	} // get_dataset_variables
	  //
	template<class TSTRING, class ALLOCANYPAIR, class ALLOCVARPAIR,
			class ALLOCINDIVPAIR>
	bool load_dataset(
			intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
					ALLOCINDIVPAIR> &cur) {
		//
		typedef intra::StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR,
				ALLOCINDIVPAIR> DatasetType;
		typedef intra::StatVariable<TSTRING, ALLOCANYPAIR> VariableType;
		typedef intra::StatIndiv<TSTRING> IndivType;
		typedef std::map<int, VariableType, std::less<int>, ALLOCVARPAIR> VariablesMapType;
		typedef std::map<int, IndivType, std::less<int>, ALLOCINDIVPAIR> IndivsMapType;
		typedef intra::StatValue ValueType;
		typedef std::map<int, intra::StatValue, std::less<int>, ALLOCANYPAIR> ValuesMapType;
		typedef std::shared_ptr<ValueType> ValuePtr;
		typedef std::shared_ptr<IndivType> IndivTypePtr;
		typedef std::shared_ptr<VariableType> VariableTypePtr;
		//
		int nId = cur.id();
		bool bOk = false;
		if (nId != 0) {
			bOk = this->get_dataset_by_id(nId, cur);
		}
		if (!bOk) {
			const TSTRING sigle = cur.sigle();
			if (!this->get_dataset_by_sigle(sigle, cur)) {
				return (false);
			}
			nId = cur.id();
		}
		std::vector<VariableType> vecVars;
		if (!this->get_dataset_variables(nId, vecVars)) {
			return (false);
		}
		std::vector<IndivType> vecInds;
		if (!this->get_dataset_indivs(nId, vecInds)) {
			return (false);
		}
		cur.clear_data();
		std::for_each(vecInds.begin(), vecInds.end(), [&](const IndivType &v) {
			cur.add_indiv(v);
		});
		std::for_each(vecVars.begin(), vecVars.end(),
				[&](const VariableType &v) {
					cur.add_variable(v);
				});
		int nCount = 0;
		if (!this->get_dataset_values_count(nId, nCount)) {
			return (false);
		}
		if (nCount < 1) {
			return (true);
		}
		int skip = 0;
		int taken = 100;
		while (skip < nCount) {
			std::vector<ValueType> vecVals;
			if (!this->get_dataset_values(nId, vecVals, skip, taken)) {
				return (false);
			}
			if (vecVals.empty()) {
				break;
			}
			std::for_each(vecVals.begin(), vecVals.end(),
					[&](const ValueType &val) {
						if (!val.is_empty()) {
							int nVarId = val.variable_id();
							const VariableType *pVar = cur.find_variable_by_id(nVarId);
							if (pVar != nullptr) {
								int nIndivId = val.indiv_id();
								const IndivType *pInd = cur.find_indiv_by_id(nIndivId);
								if (pInd != nullptr) {
									VariableType *p = const_cast<VariableType *>(pVar);
									p->add_value(val);
								}	  // pInd
							}	  // pVar
						}	  // not_empty
					});
			skip += taken;
		}	  // while skip
		return (true);
	}	  // load_dataset
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
	static const char *SQL_INSERT_VARIABLE;
	static const char *SQL_UPDATE_VARIABLE;
	static const char *SQL_REMOVE_VARIABLE;
	////////////////////////////////////////////
	static const char *SQL_FIND_VARIABLE_TYPE;
	static const char *SQL_FIND_DATASET_INDIVS_COUNT;
	static const char *SQL_FIND_DATASET_INDIVS;
	static const char *SQL_INDIV_BY_DATASET_AND_SIGLE;
	static const char *SQL_INSERT_INDIV;
	static const char *SQL_UPDATE_INDIV;
	static const char *SQL_REMOVE_INDIV;
	///////////////////////////////////////
	static const char *SQL_FIND_DATASET_VALUES_COUNT;
	static const char *SQL_FIND_DATASET_VALUES;
	static const char *SQL_VALUES_BY_VARIABLE_INDIV;
	static const char *SQL_INSERT_VALUE;
	static const char *SQL_UPDATE_VALUE;
	static const char *SQL_REMOVE_VALUE;
}
;
////////////////////////////////////
} /* namespace intrasqlite */
#endif /* STATDATAMANAGER_H_ */
