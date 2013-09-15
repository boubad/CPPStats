/*
 * intra_vector.h
 *
 *  Created on: 14 sept. 2013
 *      Author: Boubacar
 */

#ifndef INTRA_VECTOR_H_
#define INTRA_VECTOR_H_
//////////////////////////////
#if defined(_MSC_VER)
#ifndef _SCL_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#pragma warning(disable: 4996)
#pragma warning(disable: 4305)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
#endif // _MSC_VER
//////////////////////////////
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <valarray>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <boost/any.hpp>
/////////////////////////////////
namespace intra {
///////////////////////////////////
template<typename T>
inline bool are_equals(T t1, T t2, T epsilon = (T) 0) {
	T delta = (t1 > t2) ? t1 - t2 : t2 - t1;
	return (delta <= epsilon);
} // are_equals
////////////////////////////////////
template<typename T = double, class ALLOCT = std::allocator<T> >
class IntraVector {
public:
	typedef T DataType;
	typedef ALLOCT DataTypeAllocator;
	typedef DataType * DataTypePtr;
	typedef IntraVector<T, ALLOCT> IntraVectorType;
public:
	IntraVector() :
			m_n(0), m_pdata(nullptr) {
	} // IntraVector
	template<typename W>
	IntraVector(size_t n, const W *pSrc = nullptr) :
			m_n(0), m_pdata(nullptr) {
		if (n > 0) {
			DataTypePtr pDest = this->allocate(n);
			assert(pDest != nullptr);
			this->m_n = n;
			if (pSrc != nullptr) {
				for (size_t i = 0; i < n; ++i) {
					pDest[i] = (DataType) pSrc[i];
				} // i
			} // pSrc
			this->m_pdata = pDest;
		} // n
	} // IntraVector
	IntraVector(const IntraVectorType &other) :
			m_n(0), m_pdata(nullptr) {
		const size_t n = other.m_n;
		if (n > 0) {
			DataTypePtr pDest = this->allocate(n);
			assert(pDest != nullptr);
			this->m_n = n;
			this->m_pdata = pDest;
			const DataTypePtr pSrc = other.m_pdata;
			if (pSrc != nullptr) {
				for (size_t i = 0; i < n; ++i) {
					pDest[i] = (DataType) pSrc[i];
				} // i
			} // pSrc
		} // n
	} // IntraVector
	IntraVectorType & operator=(const IntraVectorType &other) {
		if (this != &other) {
			const size_t n = other.m_n;
			if (n > 0) {
				DataTypePtr pDest = DataTypeAllocator().allocate(n);
				assert(pDest != nullptr);
				this->deallocate();
				this->m_pdata = pDest;
				this->m_n = n;
				const DataTypePtr pSrc = other.m_pdata;
				if (pSrc != nullptr) {
					for (size_t i = 0; i < n; ++i) {
						pDest[i] = (DataType) pSrc[i];
					} // i
				} // pSrc
			} else {
				this->deallocate();
				this->m_n = 0;
			}
		} // copy
		return (*this);
	} // operator=
	virtual ~IntraVector() {
		this->deallocate();
	}
public:
	inline bool empty(void) const {
		return ((this->m_n < 1) || (this->m_pdata == nullptr));
	} // empty
	inline bool is_valid(void) const {
		return ((this->m_n > 0) && (this->m_pdata != nullptr));
	} // empty
	inline size_t size(void) const {
		return (this->m_n);
	}
	inline DataType & operator[](size_t i) {
		assert((i < this->m_n) && (this->m_pdata != nullptr));
		return (this->m_pdata[i]);
	}
	inline DataType operator[](size_t i) const {
		assert((i < this->m_n) && (this->m_pdata != nullptr));
		return (this->m_pdata[i]);
	}
	inline operator const DataTypePtr() const {
		return (this->m_pdata);
	}
	inline operator DataTypePtr() {
		return (this->m_pdata);
	}
	inline const DataTypePtr data() const {
		return (this->m_pdata);
	}
	inline DataTypePtr data() {
		return (this->m_pdata);
	}
	inline const DataTypePtr get() const {
		return (this->m_pdata);
	}
	inline DataTypePtr get() {
		return (this->m_pdata);
	}
	void reset(void) {
		if (this->m_pdata != nullptr) {
			DataTypeAllocator().deallocate(this->m_pdata, 0);
			this->m_pdata = nullptr;
		}
		this->m_n = 0;
	}
	void clear(void) {
		if (this->m_pdata != nullptr) {
			DataTypeAllocator().deallocate(this->m_pdata, 0);
			this->m_pdata = nullptr;
		}
		this->m_n = 0;
	}
	template<typename W>
	void resize(size_t n, const W * pSrc) {
		if (pSrc != nullptr) {
			DataTypePtr pOld = this->m_pdata;
			if ((pOld != nullptr) && (pSrc == pOld)) {
				return;
			}
		}
		this->deallocate();
		this->m_n = 0;
		if (n > 0) {
			DataTypePtr pDest = DataTypeAllocator().allocate(n);
			assert(pDest != nullptr);
			this->m_n = n;
			this->m_pdata = pDest;
			if (pSrc != nullptr) {
				for (size_t i = 0; i < n; ++i) {
					pDest[i] = (DataType) pSrc[i];
				} // i
			} // pSrc
		}
	} // resize
	void resize(size_t n) {
		this->deallocate();
		this->m_n = 0;
		if (n > 0) {
			DataTypePtr pDest = DataTypeAllocator().allocate(n);
			assert(pDest != nullptr);
			this->m_n = n;
			this->m_pdata = pDest;
		}
	} // resize
	std::ostream & writeTo(std::ostream &os) const {
		const size_t n = this->m_n;
		const DataTypePtr p = this->m_pdata;
		if ((n > 0) && (p != nullptr)) {
			for (size_t i = 0; i < n; ++i) {
				if (i > 0) {
					os << "\t";
				}
				os << p[i];
			} // i
		} // n
		return (os);
	} // writeTo
	std::wostream & writeTo(std::wostream &os) const {
		const size_t n = this->m_n;
		const DataTypePtr p = this->m_pdata;
		if ((n > 0) && (p != nullptr)) {
			for (size_t i = 0; i < n; ++i) {
				if (i > 0) {
					os << L"\t";
				}
				os << p[i];
			} // i
		} // n
		return (os);
	} // writeTo
	double mean(void) const {
		double s = 0.0;
		const size_t n = this->m_n;
		const DataTypePtr p = this->m_pdata;
		if ((n > 0) && (p != nullptr)) {
			for (size_t i = 0; i < n; ++i) {
				s += (double) p[i];
			} // i
			s /= n;
		} // n
		return (s);
	} // mean
	double standard_deviation(void) const {
		double s = 0.0;
		const size_t n = this->m_n;
		const DataTypePtr p = this->m_pdata;
		if ((n > 1) && (p != nullptr)) {
			for (size_t i = 0; i < n; ++i) {
				s += (double) p[i];
			} // i
			const double moy = s / n;
			s = 0.0;
			for (size_t i = 0; i < n; ++i) {
				double dx = p[i] - moy;
				s += dx * dx;
			} // i
			s = std::sqrt(s / n);
			if (n > 1) {
				s *= std::sqrt((double) n / (n - 1));
			}
		} // n
		return (s);
	} // mean
	template<typename W, class ALLOCW>
	bool normalize(IntraVector<W, ALLOCW> &oRes) const {
		oRes.clear();
		const size_t n = this->m_n;
		const DataTypePtr p = this->m_pdata;
		if ((n > 1) && (p != nullptr)) {
			oRes.resize(n);
			W *pDest = oRes.get();
			assert(pDest != nullptr);
			double s = 0.0;
			for (size_t i = 0; i < n; ++i) {
				s += (double) p[i];
			} // i
			const double moy = s / n;
			s = 0.0;
			for (size_t i = 0; i < n; ++i) {
				double dx = p[i] - moy;
				s += dx * dx;
			} // i
			s = std::sqrt(s / n);
			if (n > 1) {
				s *= std::sqrt((double) n / (n - 1));
			}
			const double ec = s;
			if (ec <= 0.000001) {
				return (false);
			}
			for (size_t i = 0; i < n; ++i) {
				pDest[i] = (W) ((p[i] - moy) / ec);
			} // i
		} // n
		return (false);
	} // normalize
	template<typename W, class ALLOCW>
	bool recode_normale_classes(IntraVector<W, ALLOCW> &oRes) const {
		oRes.clear();
		const size_t n = this->m_n;
		const DataTypePtr p = this->m_pdata;
		if ((n > 1) && (p != nullptr)) {
			oRes.resize(n);
			W *pDest = oRes.get();
			assert(pDest != nullptr);
			double s = 0.0;
			for (size_t i = 0; i < n; ++i) {
				s += (double) p[i];
			} // i
			const double moy = s / n;
			s = 0.0;
			for (size_t i = 0; i < n; ++i) {
				double dx = p[i] - moy;
				s += dx * dx;
			} // i
			s = std::sqrt(s / n);
			if (n > 1) {
				s *= std::sqrt((double) n / (n - 1));
			}
			const double ec = s;
			if (ec <= 0.000001) {
				return (false);
			}
			for (size_t i = 0; i < n; ++i) {
				double x = (p[i] - moy) / ec;
				if (x <= -3.0) {
					pDest[i] = (W) 1;
				} else if (x >= 3.0) {
					pDest[i] = (W) 13;
				} else {
					int nx = (int) ((x + 3.0) / 0.5) + 1;
					if (nx > 12) {
						nx = 12;
					}
					pDest[i] = (W) nx;
				}
			} // i
			return (true);
		} // n
		return (false);
	} // recode_normale_classes
private:
	inline DataTypePtr allocate(size_t n) {
		return (DataTypeAllocator().allocate(n));
	} // allocate
	inline void deallocate(void) {
		DataTypePtr p = this->m_pdata;
		if (p != nullptr) {
			DataTypeAllocator().deallocate(p, 0);
			this->m_pdata = nullptr;
		}
	} // deallocate
private:
	size_t m_n;
	DataTypePtr m_pdata;
};
// class IntraVector
/////////////////////////////////////
}// namespace intra
////////////////////////////////////////
template<class ALLOCS>
std::ostream & operator<<(std::ostream &os,
		const std::vector<boost::any, ALLOCS> &data) {
	const size_t n = data.size();
	if (n > 0) {
		for (size_t i = 0; i < n; ++i) {
			if (i > 0) {
				os << "\t";
			}
			const boost::any &v = data[i];
			if (!v.empty()){
				if (v.type() == typeid(std::string)){
					std::string s = boost::any_cast<std::string>(v);
					os << s;
				} else if (v.type() == typeid(double)){
					double x = boost::any_cast<double>(v);
					os << x;
				}
			}// not empty
		} // i
	} // n
	return os;
} // operator<<
template<class ALLOCS>
std::wostream & operator<<(std::wostream &os,
		const std::vector<boost::any, ALLOCS> &data) {
	const size_t n = data.size();
	if (n > 0) {
		for (size_t i = 0; i < n; ++i) {
			if (i > 0) {
				os << L"\t";
			}
			const boost::any &v = data[i];
			if (!v.empty()){
				if (v.type() == typeid(std::wstring)){
					std::wstring s = boost::any_cast<std::wstring>(v);
					os << s;
				} else if (v.type() == typeid(double)) {
					double x = boost::any_cast<double>(v);
					os << x;
				}
			}// not empty
		} // i
	} // n
	return os;
} // operator<<
//////////////////////////////////////
template<class S,class ALLOCS>
std::ostream & operator<<(std::ostream &os,
		const std::vector<S, ALLOCS> &data) {
	const size_t n = data.size();
	if (n > 0) {
		for (size_t i = 0; i < n; ++i) {
			if (i > 0) {
				os << "\t";
			}
			os << data[i];
		} // i
	} // n
	return os;
} // operator<<
template<class S, class ALLOCS>
std::wostream & operator<<(std::wostream &os,
		const std::vector<S, ALLOCS> &data) {
	const size_t n = data.size();
	if (n > 0) {
		for (size_t i = 0; i < n; ++i) {
			if (i > 0) {
				os << L"\t";
			}
			os << data[i];
		} // i
	} // n
	return os;
} // operator<<
///////////////////////////////////////////////////
template<class OUTPUT, typename T, class ALLOCT>
OUTPUT & operator<<(OUTPUT &os, const intra::IntraVector<T, ALLOCT> &data) {
	return data.writeTo(os);
} // operator<<
/////////////////////////////////
#endif /* INTRA_VECTOR_H_ */
