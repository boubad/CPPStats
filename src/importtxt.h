/*
 * importtxt.h
 *
 *  Created on: 13 sept. 2013
 *      Author: Boubacar
 */

#ifndef IMPORTTXT_H_
#define IMPORTTXT_H_
////////////////////////////
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
/////////////////////////////
namespace intra {
///////////////////////////////////
template<class ALLOCVECTSTRING, class TSTRING = std::string,
		class ALLOCTSTRING = std::allocator<TSTRING>,
		class TSTREAM = std::istream, typename TCHAR = char>
void intra_read_txt_stream(TSTREAM &in,
		std::vector<TSTRING, ALLOCTSTRING> &header,
		std::vector<std::vector<TSTRING, ALLOCTSTRING>, ALLOCVECTSTRING> &oVec,
		const TCHAR &delim, const TSTRING &strNA) {
	header.clear();
	oVec.clear();
	TSTRING strDelim;
	strDelim += delim;
	const int LINE_SIZE = 4095;
	const TCHAR ZERO = (TCHAR) 0;
	std::unique_ptr < TCHAR > oBuf(new TCHAR[LINE_SIZE + 1]);
	TCHAR *pBuf = oBuf.get();
	assert(pBuf != nullptr);
	size_t ncols = 0;
	if (!in.eof()) {
		pBuf[0] = ZERO;
		in.getline(pBuf, LINE_SIZE);
		TSTRING line = pBuf;
		std::vector<TSTRING> cur;
		boost::split(cur, line, boost::is_any_of(strDelim),
				boost::token_compress_off);
		ncols = cur.size();
		header.resize(ncols);
		for (size_t i = 0; i < ncols; ++i) {
			TSTRING s = boost::trim_copy(cur[i]);
			TSTRING ss = boost::to_lower_copy(s);
			if (ss == strNA) {
				s.clear();
			}
			header[i] = s;
		}
	} // not eof
	while (!in.eof()) {
		pBuf[0] = ZERO;
		in.getline(pBuf, LINE_SIZE);
		TSTRING line = pBuf;
		std::vector<TSTRING> cur;
		boost::split(cur, line, boost::is_any_of(strDelim),
				boost::token_compress_off);
		if (cur.size() == ncols) {
			std::vector<TSTRING, ALLOCTSTRING> oCur(ncols);
			for (size_t i = 0; i < ncols; ++i) {
				TSTRING s = boost::trim_copy(cur[i]);
				TSTRING ss = boost::to_lower_copy(s);
				if (ss == strNA) {
					s.clear();
				}
				oCur[i] = s;
			} // i
			oVec.push_back(oCur);
		}
	} // not eof
} // intra_read_txt_stream
///////////////////////////////////////
template<class ALLOCVECTSTRING, class ALLOCTSTRING = std::allocator<std::string> >
void intra_read_tsv(const std::string &filename,
		std::vector<std::string, ALLOCTSTRING> &header,
		std::vector<std::vector<std::string, ALLOCTSTRING>, ALLOCVECTSTRING> &oVec) {
	std::ifstream in(filename);
	oVec.clear();
	header.clear();
	const char delim = '\t';
	const std::string strNA("na");
	if (in.is_open()) {
		intra_read_txt_stream(in, header, oVec, delim, strNA);
	}
} // intra_read_tsv
template<class ALLOCVECTSTRING,
		class ALLOCTSTRING = std::allocator<std::wstring> >
void intra_read_tsv(const std::wstring &filename,
		std::vector<std::wstring, ALLOCTSTRING> &header,
		std::vector<std::vector<std::wstring, ALLOCTSTRING>, ALLOCVECTSTRING> &oVec) {
	std::string fname(filename.length(), ' ');
	std::copy(filename.begin(), filename.end(), fname.begin());
	std::wifstream in(fname.c_str());
	oVec.clear();
	header.clear();
	const wchar_t delim = L'\t';
	const std::wstring strNA(L"na");
	if (in.is_open()) {
		intra_read_txt_stream(in, header, oVec, delim, strNA);
	}
} // intra_read_tsv
//////////////////////////////////////
inline void intra_read_value(const std::string &s, boost::any &v) {
	v = boost::any();
	std::string ss = boost::trim_copy(s);
	if (ss.empty()) {
		return;
	}
	char c = *(ss.begin());
	if (std::isalpha(c)) {
		v = boost::any(ss);
	} else {
		std::stringstream in(ss);
		double d = 0.0;
		in >> d;
		v = boost::any(d);
	}
} // intra_read_value
inline void intra_read_value(const std::wstring &s, boost::any &v) {
	v = boost::any();
	std::wstring ss = boost::trim_copy(s);
	if (ss.empty()) {
		return;
	}
	wchar_t c = *(ss.begin());
	if (std::isalpha(c)) {
		v = boost::any(ss);
	} else {
		std::wstringstream in(ss);
		double d = 0.0;
		in >> d;
		v = boost::any(d);
	}
} // intra_read_value
/////////////////////////////////////////
template<class TSTRING,
		class ALLOCANY,
		class ALLOCTSTRING,
		class ALLOCVECTSTRING,
		class ALLOCVECANY>
void intra_convert_dataarray(
		const std::vector<std::vector<TSTRING, ALLOCTSTRING>, ALLOCVECTSTRING> &oVec,
		std::vector<std::vector<boost::any, ALLOCANY>, ALLOCVECANY> &oData) {
	const size_t nrows = oVec.size();
	oData.resize(nrows);
	for (size_t irow = 0; irow < nrows; ++irow){
		const std::vector<TSTRING,ALLOCTSTRING> &cur = oVec[irow];
		const size_t ncols = cur.size();
		std::vector<boost::any,ALLOCANY> xxcur(ncols);
		for (size_t j = 0; j < ncols; ++j){
			const TSTRING &s = cur[j];
			boost::any vz;
			intra_read_value(s,vz);
			xxcur[j] = vz;
		}// j
		oData[irow] = xxcur;
	}// iros
}// intra_convert_dataarray
////////////////////////////////////////
template< class ALLOCBOOL,class ALLOCANY,class ALLOCVECANY>
void intra_get_columns_categs(std::vector<std::vector<boost::any, ALLOCANY>, ALLOCVECANY> &oData,
		std::vector<bool,ALLOCBOOL> &oCategs){
	const size_t nrows = oData.size();
	size_t ncols = 0;
	for (size_t i = 0; i < nrows; ++i){
		const size_t n = (oData[i]).size();
		if (ncols < 1){
			ncols = n;
		} else if (n < ncols){
			ncols = n;
		}
	}// i
	oCategs.resize(ncols);
	for (size_t icol = 0; icol < ncols; ++icol){
		bool b = false;
		for (size_t i = 0; i < nrows; ++i){
			const std::vector<boost::any,ALLOCANY> &cur = oData[i];
			const boost::any &v = cur[icol];
			if (!v.empty()){
				if ((v.type() == typeid(std::string)) || (v.type() == typeid(std::wstring))){
					b = true;
				}
				break;
			}// not empty;
		}// i
		oCategs[icol] = b;
	}// icol
}//intra_get_columns_categs
////////////////////////////////////
}
// namespace intre
////////////////////////////

#endif /* IMPORTTXT_H_ */
