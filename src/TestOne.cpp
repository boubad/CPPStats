//============================================================================
// Name        : TestOne.cpp
// Author      : Boubacar Diarra
// Version     :
// Copyright   : boubadiarra@gmail.com
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "importdata.h"
/////////////////////////////
#include "../sqlite/database.h"
//////////////////////////////
using namespace std;
using namespace intra;
using namespace sqlite;
///////////////////////////////////
const char *TEST_FILENAME = "./data/source.csv";
const wchar_t *TEST_WFILENAME = L"./data/source.csv";
const char *TEST_DATABASE_FILE = "./data/testbase.db";
////////////////////////////////////////
const char *SQL_CREATE_DATASET = "CREATE TABLE IF NOT EXISTS dbdataset("
		" datasetid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" sigle TEXT NOT NULL UNIQUE,"
		" nom TEXT DEFAULT NULL,"
		" description TEXT DEFAULT NULL"
		" )";
const char *SQL_CREATE_VARIABLE = "CREATE TABLE IF NOT EXISTS dbvariable("
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
const char *SQL_CREATE_INDIV = "CREATE TABLE IF NOT EXISTS dbindiv("
		" individ INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" datasetid INTEGER NOT NULL,"
		" sigle TEXT NOT NULL,"
		" nom TEXT DEFAULT NULL,"
		" description TEXT DEFAULT NULL,"
		" CONSTRAINT uc_indiv UNIQUE (datasetid, sigle),"
		" CONSTRAINT fk_indiv_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
		" )";
const char *SQL_CREATE_VALUE = "CREATE TABLE IF NOT EXISTS dbvalue("
		" valueid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" variableid INTEGER NOT NULL,"
		" individ INTEGER NOT NULL,"
		" stringval TEXT NULL,"
		" CONSTRAINT uc_value UNIQUE (variableid, individ),"
		" CONSTRAINT fk_value_variable FOREIGN KEY (variableid) REFERENCES dbvariable (variableid) ON DELETE CASCADE,"
		" CONSTRAINT fk_value_indiv FOREIGN KEY (individ) REFERENCES dbindiv (individ) ON DELETE CASCADE"
		" )";
/////////////////////////////////////
void mytest_db(void){
	Database oBase;
	bool bRet = oBase.open(TEST_DATABASE_FILE);
	bRet = bRet && oBase.exec_sql(SQL_CREATE_DATASET);
	bRet = bRet && oBase.exec_sql(SQL_CREATE_VARIABLE);
	bRet = bRet && oBase.exec_sql(SQL_CREATE_INDIV);
	bRet = bRet && oBase.exec_sql(SQL_CREATE_VALUE);
	if (!bRet){
		std::string err;
		oBase.get_last_error(err);
		cout << endl << "ERROR...!!!" << endl << err << endl;
	}
	bRet = oBase.close();
}// mytest_db
////////////////////////////////
void mytest_one() {
	cout << std::endl << "STRING" << std::endl;
	vector<string> header;
	vector<vector<string> > oList;
	string filename(TEST_FILENAME);
	intra_read_tsv(filename, header, oList);
	size_t ncols = header.size();
	cout << ncols << endl;
	for (auto it = header.begin(); it != header.end(); ++it) {
		if (it != header.begin()) {
			cout << "\t";
		}
		cout << *it;
	} // it
	cout << endl;
	cout << oList.size() << std::endl;
	for (auto it = oList.begin(); it != oList.end(); ++it) {
		size_t nx = (*it).size();
		if (nx != ncols) {
			cout << "ERROR...\t" << nx << endl;
		}
	}
	vector<vector<boost::any> > oData;
	vector<bool> oRes;
	intra_convert_dataarray(oList, oData);
	intra_get_columns_categs(oData, oRes);
} // mytest_one
void mytest_two() {
	wcout << std::endl << L"WSTRING" << std::endl;
	vector<wstring> header;
	vector<vector<wstring> > oList;
	wstring filename(TEST_WFILENAME);
	intra_read_tsv(filename, header, oList);
	size_t ncols = header.size();
	wcout << ncols << endl;
	for (auto it = header.begin(); it != header.end(); ++it) {
		if (it != header.begin()) {
			wcout << L"\t";
		}
		wcout << *it;
	} // it
	wcout << endl;
	wcout << oList.size() << std::endl;
	for (auto it = oList.begin(); it != oList.end(); ++it) {
		size_t nx = (*it).size();
		if (nx != ncols) {
			cout << L"ERROR...\t" << nx << endl;
		}
	}
} // mytest_two
void mytest_three() {
	typedef int IndexType;
	ImportData<> oData;
	typedef ImportData<>::AnyVectorType AnyVectorType;
	wifstream in(TEST_FILENAME);
	wstring na = L"na";
	wchar_t delim = L'\t';
	oData.open(in, delim, na);
	size_t ncols = oData.cols();
	set<IndexType> colindexes;
	vector<wstring> names;
	for (size_t icol = 0; icol < ncols; ++icol) {
		AnyVectorType v;
		set<IndexType> oIndexes;
		wstring name = oData.colname(icol);
		names.push_back(name);
		colindexes.insert((IndexType)icol);
		oData.get_indexes(colindexes, oIndexes);
		if (oIndexes.empty()){
			break;
		}
		vector<boost::any> vx;
		oData.get_col_values(icol,oIndexes,vx);
		wcout << std::endl;
		for (auto it = names.begin(); it != names.end(); ++it){
			if (it != names.begin()){
				wcout << L"\t";
			}
			wcout << (*it);
		}
		wcout << std::endl << L"Nb. vals: " << oIndexes.size() << L"/" << vx.size() << std::endl;
	} // icol
} // mytest_three
/////////////////////////////////
int main(int argc, char *argv[]) {
	wcout << L"!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	//mytest_one();
	//mytest_two();
	//mytest_three();
	mytest_db();
	wcout << std::endl << std::endl;
	wcout << L"Entrez un nombre pour quitter. :" << std::endl;
	int x;
	wcin >> x;
	wcout << x << endl;
	return 0;
}
