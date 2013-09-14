//============================================================================
// Name        : TestOne.cpp
// Author      : Boubacar Diarra
// Version     :
// Copyright   : boubadiarra@gmail.com
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "importdata.h"

using namespace std;
using namespace intra;
///////////////////////////////////
const char *TEST_FILENAME = "./data/source.csv";
const wchar_t *TEST_WFILENAME = L"./data/source.csv";
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
	ImportData<> oData;
	typedef ImportData<>::AnyVectorType AnyVectorType;
	wifstream in(TEST_FILENAME);
	wstring na = L"na";
	wchar_t delim = L'\t';
	oData.open(in, delim, na);
	size_t ncols = oData.cols();
	for (size_t icol = 0; icol < ncols; ++icol) {
		AnyVectorType v;
		vector<short> oIndexes;
		wstring name = oData.colname(icol);
		if (oData.is_num_var(icol)) {
			oData.get_col_norm_values(icol, v, oIndexes);
		} else {
			oData.get_col_factor_values(icol, v, oIndexes);
		}
		wcout << std::endl << name << std::endl;
		wcout << v << std::endl << oIndexes << std::endl;
	} // icol
} // mytest_three
/////////////////////////////////
int main(int argc, char *argv[]) {
	wcout << L"!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	//mytest_one();
	//mytest_two();
	mytest_three();
	wcout << std::endl << std::endl;
	wcout << L"Entrez un nombre pour quitter. :" << std::endl;
	int x;
	wcin >> x;
	wcout << x << endl;
	return 0;
}