//============================================================================
// Name        : TestOne.cpp
// Author      : Boubacar Diarra
// Version     :
// Copyright   : boubadiarra@gmail.com
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "../statdataset/importdata.h"
/////////////////////////////
#include "../intrasqlite/statdatamanager.h"
//////////////////////////////
using namespace std;
using namespace intra;
using namespace intrasqlite;
///////////////////////////////////
const char *TEST_FILENAME = "./data/source.csv";
const wchar_t *TEST_WFILENAME = L"./data/source.csv";
const char *TEST_DATABASE_FILE = "./data/testbase.db";
const char *TEST_DATASET_SIGLE = "APB2013";
/////////////////////////////////////////
extern bool import_data(const std::string &srcfilename,
const std::string &databaseFilename, const std::string &datasetSigle);
extern bool load_test_data(const std::string &databaseFilename,
		const std::string &datasetSigle);
////////////////////////////////////////
#ifdef MYTOTO
void mytest_db(void){
	std::string filename(TEST_DATABASE_FILE);
	StatDataManager oMan(filename);
	bool bRet = oMan.is_valid();
	if (!bRet){
		return;
	}
	std::string sigle("TestSigle");
	StatDataset<std::string> oSet;
	bRet = oMan.get_dataset_by_sigle(sigle,oSet);
	if ((!bRet) || (oSet.id() == 0)){
		oSet.sigle(sigle);
		oSet.name("Name of test database");
		oSet.description("Bla bla bla");
		bRet = oMan.insert_dataset(oSet);
		if (!bRet){
			return;
		}
		bRet = oMan.get_dataset_by_sigle(sigle,oSet);
		if (!bRet){
			return;
		}
	}
	int nId = oSet.id();
	if (nId == 0){
		return;
	}
	bRet = oMan.get_dataset_by_id(nId,oSet);
	std::vector<StatDataset<std::string>  > oVec;
	bRet = oMan.get_all_datasets(oVec);
	oSet.name("Génie civil");
	bRet = oMan.update_dataset(oSet);
	if (!bRet){
		return;
	}
	bRet = oMan.remove_dataset(oSet);
	bRet = oMan.get_all_datasets(oVec);
	bRet = oMan.close();
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
	typedef std::wstring StringType;
	//
	ImportData<StringType> oData;
	typedef ImportData<StringType>::AnyVectorType AnyVectorType;
	wifstream in(TEST_FILENAME);
	wstring na = L"na";
	wchar_t delim = L'\t';
	oData.open(in, delim, na);
	size_t ncols = oData.cols();
	set<IndexType> colindexes;
	vector<StringType> names;
	for (size_t icol = 0; icol < ncols; ++icol) {
		AnyVectorType v;
		set<IndexType> oIndexes;
		StringType name = oData.colname(icol);
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
#endif // MYTOTO
/////////////////////////////////
int main(int argc, char *argv[]) {
	wcout << L"!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	//mytest_one();
	//mytest_two();
	// mytest_three();
	//mytest_db();
	//
	std::string src(TEST_FILENAME);
	std::string dest(TEST_DATABASE_FILE);
	std::string sigle(TEST_DATASET_SIGLE);
	//
	// bool bRet = import_data(src,dest, sigle);
	bool bRet = load_test_data(dest,sigle);
	//
	wcout << std::endl << std::endl;
	wcout << L"Entrez un nombre pour quitter. :" << std::endl;
	int x;
	wcin >> x;
	wcout << x << endl;
	return 0;
}
