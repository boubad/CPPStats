/*
 * statdatamanager.cpp
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#include "statdatamanager.h"
///////////////////////////////////////
using namespace sqlite;
//////////////////////////////
namespace intrasqlite {
//////////////////////////////////////////
const char *StatDataManager::SQL_COMMIT_TRANSACTION = "COMMIT TRANSACTION";
const char *StatDataManager::SQL_BEGIN_TRANSACTION = "BEGIN TRANSACTION";
const char *StatDataManager::SQL_ROLLBACK_TRANSACTION = "ROLLBACK TRANSACTION";
////////////////////////////////////
const char *StatDataManager::SQL_CREATE_DATASET =
		"CREATE TABLE IF NOT EXISTS dbdataset("
				" datasetid INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" sigle TEXT NOT NULL UNIQUE,"
				" nom TEXT DEFAULT NULL,"
				" description TEXT DEFAULT NULL"
				" )";
//////////////////////////////////////
const char *StatDataManager::SQL_FIND_ALL_DATASETS =
		"SELECT datasetid,optlock,sigle,nom,description"
				" FROM dbdataset ORDER BY sigle";
const char *StatDataManager::SQL_FIND_DATASET_BY_ID =
		"SELECT datasetid,optlock,sigle,nom,description"
				" FROM dbdataset WHERE datasetid = ?1";
const char *StatDataManager::SQL_FIND_DATASET_BY_SIGLE =
		"SELECT datasetid,optlock,sigle,nom,description"
				" FROM dbdataset WHERE UPPER(LTRIM(RTRIM(sigle))) = ?1";
const char *StatDataManager::SQL_INSERT_DATASET =
		"INSERT INTO dbdataset (sigle,nom,description)"
				" VALUES (?1,?2,?3)";
const char *StatDataManager::SQL_UPDATE_DATASET =
		"UPDATE dbdataset SET optlock = optlock + 1,"
				" sigle = ?1, nom = ?2, description = ?3 WHERE datasetid = ?4";
const char *StatDataManager::SQL_REMOVE_DATASET =
		"DELETE FROM dbdataset WHERE datasetid = ?1";
///////////////////////////////////////
const char *StatDataManager::SQL_CREATE_VARIABLE =
		"CREATE TABLE IF NOT EXISTS dbvariable("
				" variableid INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" datasetid INTEGER NOT NULL,"
				" sigle TEXT NOT NULL,"
				" vartype TEXT NOT NULL,"
				" categvar INTEGER NOT NULL DEFAULT 1,"
				" nom TEXT DEFAULT NULL,"
				" description TEXT DEFAULT NULL,"
				" CONSTRAINT uc_variable UNIQUE (datasetid, sigle),"
				" CONSTRAINT fk_variable_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
				" )";
/////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
const char *StatDataManager::SQL_CREATE_INDIV =
		"CREATE TABLE IF NOT EXISTS dbindiv("
				" individ INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" datasetid INTEGER NOT NULL,"
				" sigle TEXT NOT NULL,"
				" nom TEXT DEFAULT NULL,"
				" description TEXT DEFAULT NULL,"
				" CONSTRAINT uc_indiv UNIQUE (datasetid, sigle),"
				" CONSTRAINT fk_indiv_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
				" )";
const char *StatDataManager::SQL_CREATE_VALUE =
		"CREATE TABLE IF NOT EXISTS dbvalue("
				" valueid INTEGER PRIMARY KEY AUTOINCREMENT,"
				" optlock INTEGER NOT NULL DEFAULT 1,"
				" variableid INTEGER NOT NULL,"
				" individ INTEGER NOT NULL,"
				" stringval TEXT NULL,"
				" CONSTRAINT uc_value UNIQUE (variableid, individ),"
				" CONSTRAINT fk_value_variable FOREIGN KEY (variableid) REFERENCES dbvariable (variableid) ON DELETE CASCADE,"
				" CONSTRAINT fk_value_indiv FOREIGN KEY (individ) REFERENCES dbindiv (individ) ON DELETE CASCADE"
				" )";
const char *StatDataManager::CREATE_SQL[] = {
		StatDataManager::SQL_CREATE_DATASET,
		StatDataManager::SQL_CREATE_VARIABLE, StatDataManager::SQL_CREATE_INDIV,
		StatDataManager::SQL_CREATE_VALUE, nullptr };
/////////////////////////////////////
/////////////////////////////////////
StatDataManager::StatDataManager() :
		m_intransaction(false) {
} // StatDataManager
StatDataManager::StatDataManager(const std::string &s) :
		m_intransaction(false) {
	this->open(s);
} // StatDataManager
StatDataManager::StatDataManager(const std::wstring &ss) :
		m_intransaction(false) {
	this->open(ss);
} // StatDataManager
bool StatDataManager::begin_transaction(void) {
	if (this->m_intransaction) {
		return (true);
	}
	assert(this->is_valid());
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_BEGIN_TRANSACTION);
	if (!stmt.is_valid()) {
		return (false);
	}
	if (stmt.exec()) {
		this->m_intransaction = true;
		return (true);
	}
	return (false);
} // begin_transaction
bool StatDataManager::commit_transaction(void) {
	if (!this->m_intransaction) {
		return (false);
	}
	assert(this->is_valid());
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_COMMIT_TRANSACTION);
	if (!stmt.is_valid()) {
		return (false);
	}
	if (stmt.exec()) {
		this->m_intransaction = false;
		return (true);
	}
	return (false);
} // commit_transaction
bool StatDataManager::rollback_transaction(void) {
	if (!this->m_intransaction) {
		return (false);
	}
	assert(this->is_valid());
	sqlite::Database *pBase = this->m_database.get();
	sqlite::Statement stmt(pBase, SQL_ROLLBACK_TRANSACTION);
	if (!stmt.is_valid()) {
		return (false);
	}
	if (stmt.exec()) {
		this->m_intransaction = false;
		return (true);
	}
	return (false);
} // rollback_transaction
bool StatDataManager::close(void) {
	bool bRet = true;
	Database *pbase = this->m_database.get();
	if (pbase != nullptr) {
		if (pbase->close()) {
			this->m_database.reset();
		} else {
			bRet = false;
		}
	}
	return (bRet);
} // close
StatDataManager::~StatDataManager() {
}
bool StatDataManager::open(const std::string &filename) {
	this->m_database.reset(new Database());
	Database *pBase = this->m_database.get();
	assert(pBase != nullptr);
	if (!pBase->open(filename)) {
		this->m_database.reset();
		return (false);
	}
	if (!this->check_schema()) {
		this->m_database.reset();
		return (false);
	}
	return (true);
} // open
bool StatDataManager::open(const std::wstring &filename) {
	this->m_database.reset(new Database());
	Database *pBase = this->m_database.get();
	assert(pBase != nullptr);
	if (!pBase->open(filename)) {
		this->m_database.reset();
		return (false);
	}
	if (!this->check_schema()) {
		this->m_database.reset();
		return (false);
	}
	return (true);
} // open
bool StatDataManager::is_valid(void) const {
	Database *pbase = this->m_database.get();
	return ((pbase != nullptr) && pbase->is_open());
} // is_valid
bool StatDataManager::check_schema(void) {
	Database *pbase = this->m_database.get();
	assert(pbase != nullptr);
	assert(pbase->is_open());
	int i = 0;
	while (StatDataManager::CREATE_SQL[i] != nullptr) {
		const char *pszSQL = CREATE_SQL[i];
		if (!pbase->exec_sql(pszSQL)) {
			return (false);
		}
		++i;
	} // ok
	return (true);
} // check_schema
///////////////////////////
} /* namespace intrasqlite */
