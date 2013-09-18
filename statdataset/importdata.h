/*
 * importdata.h
 *
 *  Created on: 13 sept. 2013
 *      Author: Boubacar
 */

#ifndef IMPORTDATA_H_
#define IMPORTDATA_H_
///////////////////////////
#include "intra_vector.h"
#include "importtxt.h"
/////////////////////////////////
namespace intra {
////////////////////////////////////
template<class TSTRING = std::wstring, typename CHARTYPE = wchar_t,
		class ALLOCTSTRING = std::allocator<TSTRING>,
		class ALLOCVECTSTRING = std::allocator<
				std::vector<TSTRING, ALLOCTSTRING> >,
		class ALLOCANY = std::allocator<boost::any>,
		class ALLOCANYVEC = std::allocator<std::vector<boost::any, ALLOCANY> >,
		class ALLOCBOOL = std::allocator<bool> >
class ImportData {
public:
	typedef CHARTYPE CharType;
	typedef TSTRING StringType;
	typedef std::vector<StringType, ALLOCTSTRING> StringVectorType;
	typedef std::vector<StringVectorType, ALLOCVECTSTRING> StringArrayType;
	typedef std::vector<boost::any, ALLOCANY> AnyVectorType;
	typedef std::vector<AnyVectorType, ALLOCANYVEC> AnyArrayType;
	typedef std::vector<bool, ALLOCBOOL> BoolVectorType;
	typedef ImportData<CHARTYPE, TSTRING, ALLOCTSTRING, ALLOCVECTSTRING,
			ALLOCANY, ALLOCANYVEC, ALLOCBOOL> ImportDataType;
public:
	ImportData() {
	} // ImportData
	ImportData(const ImportDataType &other) :
			m_header(other.m_header), m_stringdata(other.m_stringdata), m_data(
					other.m_data), m_categs(other.m_categs) {
	}
	ImportDataType & operator=(const ImportDataType &other) {
		if (this != &other) {
			this->m_header = other.m_header;
			this->m_stringdata = other.m_stringdata;
			this->m_data = other.m_data;
			this->m_categs = other.m_categs;
		}
		return (*this);
	} // operator=
	virtual ~ImportData() {
	}
public:
	template<class INPUTSTREAM>
	void open(INPUTSTREAM &in, const CharType &delim, const StringType &na) {
		intra_read_txt_stream(in, this->m_header, this->m_stringdata, delim,
				na);
		intra_convert_dataarray(this->m_stringdata, this->m_data);
		intra_get_columns_categs(this->m_data, this->m_categs);
	} // open
	inline size_t cols(void) const {
		return (this->m_categs.size());
	}
	inline size_t rows(void) const {
		return (this->m_data.size());
	}
	inline const StringVectorType & header(void) const {
		return (this->m_header);
	}
	inline const BoolVectorType & types(void) const {
		return (this->m_categs);
	}
	inline const AnyArrayType & data(void) const {
		return (this->m_data);
	}
	inline const StringArrayType & string_data(void) const {
		return (this->m_stringdata);
	}
	inline const StringType & colname(size_t icol) const {
		assert(icol < this->m_categs.size());
		return (this->m_header[icol]);
	}
	inline bool is_categ_var(size_t icol) const {
		assert(icol < this->m_categs.size());
		return (this->m_categs[icol]);
	}
	inline bool is_num_var(size_t icol) const {
		assert(icol < this->m_categs.size());
		return (!(this->m_categs[icol]));
	}
	inline bool get_value(size_t irow, size_t icol, boost::any &v) const {
		assert(irow < this->m_data.size());
		const AnyVectorType &vv = (this->m_data)[irow];
		assert(icol < vv.size());
		v = vv[icol];
		return (!v.empty());
	}
	bool get_string_value(size_t irow, size_t icol, StringType &v) const {
		assert(irow < this->m_data.size());
		const AnyVectorType &vv = (this->m_data)[irow];
		assert(icol < vv.size());
		boost::any vx = vv[icol];
		v.clear();
		if (vx.empty()) {
			return (false);
		}
		auto t = vx.type();
		if (t == typeid(StringType)) {
			v = boost::any_cast<StringType>(vx);
			return (true);
		}
		return (false);
	}
	bool get_num_value(size_t irow, size_t icol, double &v) const {
		assert(irow < this->m_data.size());
		const AnyVectorType &vv = (this->m_data)[irow];
		assert(icol < vv.size());
		boost::any vx = vv[icol];
		if (vx.empty()) {
			return (false);
		}
		auto t = vx.type();
		if (t == typeid(double)) {
			v = boost::any_cast<double>(vx);
			return (true);
		}
		return (false);
	}
	const AnyVectorType & get_row_value(size_t irow) const {
		const AnyArrayType & aa = this->m_data;
		assert(irow < aa.size());
		return (aa[irow]);
	} // get_row_value
public:
	template<class ALLOCA>
	void get_col_values(size_t icol,
			std::vector<boost::any, ALLOCA> &vv) const {
		assert(icol < this->m_categs.size());
		const AnyArrayType & aa = this->m_data;
		const size_t n = aa.size();
		vv.resize(n);
		for (size_t i = 0; i < n; ++i) {
			vv[i] = (aa[i])[icol];
		} // i
	} // get_col_values
	template<class INPUTITERATOR, class ALLOCA>
	void get_col_values(size_t icol, INPUTITERATOR start, INPUTITERATOR last,
			std::vector<boost::any, ALLOCA> &oRes) const {
		assert(icol < this->m_categs.size());
		oRes.clear();
		const AnyArrayType & aa = this->m_data;
		for (auto it = start; it != last; ++it) {
			const size_t irow = (size_t)(*it);
			assert(irow < aa.size());
			const AnyVectorType &vv = aa[irow];
			assert(icol < vv.size());
			const boost::any v = vv[icol];
			oRes.push_back(v);
		} // it
	} // get_col_values
	template<class CONTAINER, class ALLOCA>
	void get_col_values(size_t icol, const CONTAINER &cont,
			std::vector<boost::any, ALLOCA> &oRes) const {
		this->get_col_values(icol, cont.begin(), cont.end(), oRes);
	} // get_col_values
	template<typename T, class ALLOCT>
	void get_col_valid_indexes(size_t icol,
			std::set<T, std::less<T>, ALLOCT> &oIndexes) const {
		assert(icol < this->m_categs.size());
		const AnyArrayType & aa = this->m_data;
		const size_t n = aa.size();
		oIndexes.clear();
		for (size_t i = 0; i < n; ++i) {
			const boost::any &v = (aa[i])[icol];
			if (!v.empty()) {
				T ii = (T)i;
				oIndexes.insert(ii);
			}
		} // i
	} // get_col_valid_indexes
	template<class CONTAINER,typename T, class ALLOCT>
	void get_indexes(const CONTAINER &oCols,
			std::set<T, std::less<T>, ALLOCT> &oIndexes) const {
		oIndexes.clear();
		auto istart = oCols.begin();
		for (auto it = istart; it != oCols.end(); ++it) {
			std::set<T, std::less<T>, ALLOCT> cur;
			const size_t icol = (size_t)(*it);
			this->get_col_valid_indexes(icol, cur);
			if (it == istart) {
				oIndexes = cur;
			} else {
				if (oIndexes.empty()){
					break;
				}
				std::set<T, std::less<T>, ALLOCT> oDel;
				auto jlast = oIndexes.end();
				for (auto jt = oIndexes.begin(); jt != jlast; ++jt) {
					T jindex = *jt;
					if (cur.find(jindex) == cur.end()) {
						oDel.insert(jindex);
					}
				} // jt
				std::for_each(oDel.begin(), oDel.end(), [&](T jdel) {
					auto rindex = oIndexes.find(jdel);
					if (rindex != oIndexes.end()) {
						oIndexes.erase(rindex);
					}
				});
			}
		} // it
	} // get_indexes

	template<class ALLOCA, typename Z, class ALLOCZ>
	bool get_col_norm_values(size_t icol,
			std::vector<boost::any, ALLOCA> &oNorm,
			std::vector<Z, ALLOCZ> &oClasses) const {
		oNorm.clear();
		oClasses.clear();
		if (!this->is_num_var(icol)) {
			return (false);
		}
		std::set<size_t> oIndexes;
		this->get_col_valid_indexes(icol, oIndexes);
		const size_t n = oIndexes.size();
		if (n < 2) {
			return (false);
		}
		double s = 0.0;
		size_t nz = 0;
		const AnyArrayType &oAr = this->m_data;
		std::for_each(oIndexes.begin(), oIndexes.end(), [&](size_t i) {
			const AnyVectorType &vv = oAr[i];
			const boost::any &v = vv[icol];
			if (v.type() == typeid(double)) {
				double x = boost::any_cast<double>(v);
				s += x;
				++nz;
			}
		});
		if (nz < 2) {
			return (false);
		}
		const double moy = s / nz;
		s = 0.0;
		nz = 0;
		std::for_each(oIndexes.begin(), oIndexes.end(), [&](size_t i) {
			const AnyVectorType &vv = oAr[i];
			const boost::any &v = vv[icol];
			if (v.type() == typeid(double)) {
				double x = boost::any_cast<double>(v);
				double d = x - moy;
				s += d * d;
				++nz;
			}
		});
		if (nz < 2) {
			return (false);
		}
		const double ec = std::sqrt(((double) nz / (nz - 1)) * (s / nz));
		if (ec < 0.000001) {
			return (false);
		}
		const size_t nr = oAr.size();
		oNorm.resize(nr);
		oClasses.resize(nr, (Z) -1);
		for (size_t i = 0; i < nr; ++i) {
			const AnyVectorType &vv = oAr[i];
			const boost::any &v = vv[icol];
			if (v.empty()) {
				oNorm[i] = boost::any();
			} else if (v.type() == typeid(double)) {
				double x = boost::any_cast<double>(v);
				double xx = (x - moy) / ec;
				oNorm[i] = boost::any(xx);
				if (xx <= -3.0) {
					oClasses[i] = (Z) 1;
				} else if (xx >= 3.0) {
					oClasses[i] = (Z) 13;
				} else {
					int nx = (int) ((xx + 3.0) / 0.5) + 1;
					if (nx > 13) {
						nx = 13;
					}
					oClasses[i] = (Z) nx;
				}
			}
		} // i
		return (true);
	} //get_col_norm_values
	template <typename T, class ALLOCT,class ALLOCPAIR>
	void get_values(const std::vector<T,ALLOCT> &oCols,
			std::map<T,std::vector<boost::any,ALLOCANY>,std::less<T>,ALLOCPAIR > &oMap) const{
		oMap.clear();
		const AnyArrayType &oAr = this->m_data;
		std::set<T,std::less<T>,ALLOCT> oIndexes;
		this->get_indexes(oCols,oIndexes);
		size_t nc = oCols.size();
		auto jstart = oCols.begin();
		auto jend = oCols.end();
		for (auto it = oIndexes.begin();it != oIndexes.end();++it){
			size_t irow = (size_t)(*it);
			assert(irow < oAr.size());
			const AnyVectorType & vx = oAr[irow];
			std::vector<boost::any,ALLOCANY> vv(nc);
			T icur = 0;
			for (auto jt = jstart; jt != jend; ++jt){
				size_t icol = (size_t)(*jt);
				assert(icol < vx.size());
				boost::any xa = vx[icol];
				vv[icur++] = xa;
			}// jt
			oMap[(T)irow] = vv;
		}// it
	}// getValues
	template<class ALLOCA, typename Z, class ALLOCZ>
	bool get_col_factor_values(size_t icol,
			std::vector<boost::any, ALLOCA> &oNorm,
			std::vector<Z, ALLOCZ> &oClasses) const {
		oNorm.clear();
		oClasses.clear();
		if (!this->is_categ_var(icol)) {
			return (false);
		}
		std::set<size_t> oIndexes;
		this->get_col_valid_indexes(icol, oIndexes);
		const size_t n = oIndexes.size();
		if (n < 2) {
			return (false);
		}
		std::map<StringType, Z> oMap;
		const AnyArrayType &oAr = this->m_data;
		Z icur = (Z) 1;
		std::for_each(oIndexes.begin(), oIndexes.end(), [&](size_t i) {
			const AnyVectorType &vv = oAr[i];
			const boost::any &v = vv[icol];
			if ((!v.empty()) && (v.type() == typeid(StringType))) {
				StringType s = boost::any_cast<StringType>(v);
				StringType ss = boost::trim_copy(s);
				if (!ss.empty()) {
					auto it = oMap.find(ss);
					if (it == oMap.end()) {
						oMap[ss] = icur++;
					}
				} // not empty
			}
		});
		const size_t nr = oAr.size();
		oNorm.resize(nr);
		oClasses.resize(nr, (Z) -1);
		for (size_t i = 0; i < nr; ++i) {
			const AnyVectorType &vv = oAr[i];
			const boost::any &v = vv[icol];
			if (v.empty()) {
				oNorm[i] = boost::any();
			} else if (v.type() == typeid(StringType)) {
				StringType s = boost::any_cast<StringType>(v);
				StringType ss = boost::trim_copy(s);
				if (!ss.empty()) {
					oNorm[i] = boost::any(ss);
					auto it = oMap.find(ss);
					if (it != oMap.end()) {
						Z nx = (*it).second;
						oClasses[i] = nx;
					}
				} // not empty
			}
		} // i
		return (true);
	} //get_col_factor_values
	template<class ALLOCS>
	bool get_col_valid_stringvector(size_t icol,
			std::vector<StringType, ALLOCS> &oRes) const {
		assert(icol < this->m_categs.size());
		oRes.clear();
		bool bRet = false;
		if (this->m_categs[icol]) {
			bRet = true;
			const AnyArrayType & aa = this->m_data;
			const size_t nrows = aa.size();
			for (size_t irow = 0; irow < nrows; ++irow) {
				const AnyVectorType &vv = aa[irow];
				assert(icol < vv.size());
				const boost::any &v = vv[icol];
				if ((!v.empty()) && (v.type() == typeid(StringType))) {
					StringType sx = boost::any_cast<StringType>(v);
					if (!sx.empty()) {
						oRes.push_back(sx);
					}
				}
			} // irow
		} // ok string
		return bRet;
	} // get_col_valid_stringvector
	template<typename T, class ALLOCT>
	bool get_col_valid_numvector(size_t icol,
			std::vector<T, ALLOCT> &oRes) const {
		assert(icol < this->m_categs.size());
		oRes.clear();
		bool bRet = false;
		if (!this->m_categs[icol]) {
			bRet = true;
			const AnyArrayType & aa = this->m_data;
			const size_t nrows = aa.size();
			for (size_t irow = 0; irow < nrows; ++irow) {
				const AnyVectorType &vv = aa[irow];
				assert(icol < vv.size());
				const boost::any &v = vv[icol];
				if (!v.empty()) {
					double sx = boost::any_cast<double>(v);
					T vr = (T) sx;
					oRes.push_back(vr);
				}
			} // irow
		} // ok string
		return bRet;
	} // get_col_valid_numvector
	template<typename T, class ALLOCT>
	bool get_col_valid_numvector(size_t icol,
			IntraVector<T, ALLOCT> &oRes) const {
		std::vector<T, ALLOCT> oVec;
		if (!this->get_col_valid_numvector(icol, oVec)) {
			return (false);
		}
		const size_t n = oVec.size();
		if (n < 1) {
			oRes.clear();
			return (true);
		}
		T * pTemp = ALLOCT().allocate(n);
		for (size_t i = 0; i < n; ++i) {
			pTemp[i] = oVec[i];
		}
		oRes.resize(n, pTemp);
		ALLOCT().deallocate(pTemp, 0);
		return (true);
	} // get_col_valid_numvector
private:
	StringVectorType m_header;
	StringArrayType m_stringdata;
	AnyArrayType m_data;
	BoolVectorType m_categs;
private:
	CharType m_chardummy;
};
// class ImportData
///////////////////////////////////////
}// namespace intra
/////////////////////////////
#endif /* IMPORTDATA_H_ */
