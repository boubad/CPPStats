/*
 * dbvalue.cpp
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#include "dbvalue.h"

namespace sqlite {
///////////////////////////////////////////////
Blob::Blob() :
		m_size(0) {
} // Blob
Blob::Blob(size_t nSize) :
		m_size(0) {
	if (nSize > 0) {
		this->m_data.reset(new byte[nSize]);
		assert(this->m_data.get() != nullptr);
		this->m_size = nSize;
	} // nSize
} // Blob
Blob::Blob(const byte *pData, size_t nSize) :
		m_size(0) {
	if (nSize > 0) {
		this->m_data.reset(new byte[nSize]);
		byte *pDest = this->m_data.get();
		assert(pDest != nullptr);
		this->m_size = nSize;
		if (pData != nullptr) {
			const byte *pSrc = pData;
			while (nSize > 0) {
				*pDest++ = *pSrc++;
				--nSize;
			}
		} // pData
	} // nSize
} // Blob
Blob::Blob(const Blob &other) :
		m_size(0) {
	size_t nSize = other.m_size;
	const byte *pData = other.m_data.get();
	if (nSize > 0) {
		this->m_data.reset(new byte[nSize]);
		byte *pDest = this->m_data.get();
		assert(pDest != nullptr);
		this->m_size = nSize;
		if (pData != nullptr) {
			const byte *pSrc = pData;
			while (nSize > 0) {
				*pDest++ = *pSrc++;
				--nSize;
			}
		} // pData
	} // nSi
} // Blob
Blob & Blob::operator=(const Blob &other) {
	if (this != &other) {
		this->m_data.reset();
		this->m_size = 0;
		size_t nSize = other.m_size;
		const byte *pData = other.m_data.get();
		if (nSize > 0) {
			this->m_data.reset(new byte[nSize]);
			byte *pDest = this->m_data.get();
			assert(pDest != nullptr);
			this->m_size = nSize;
			if (pData != nullptr) {
				const byte *pSrc = pData;
				while (nSize > 0) {
					*pDest++ = *pSrc++;
					--nSize;
				}
			} // pData
		} // nSi
	} // copy
	return (*this);
} // operator=
Blob::~Blob() {
} // ~Blob
void Blob::data(const byte *pData, size_t nSize) {
	byte *pOld = this->m_data.get();
	if (pOld != pData) {
		this->m_data.reset();
		this->m_size = 0;
		if (nSize > 0) {
			this->m_data.reset(new byte[nSize]);
			byte *pDest = this->m_data.get();
			assert(pDest != nullptr);
			this->m_size = nSize;
			if (pData != nullptr) {
				const byte *pSrc = pData;
				while (nSize > 0) {
					*pDest++ = *pSrc++;
					--nSize;
				}
			} // pData
		} // nSize
	} // copy
} // data
////////////////////////////////////////////
DbValue::DbValue() {
}
DbValue::DbValue(const DbValue &other):m_val(other.m_val){

}// DbValue
DbValue & DbValue::operator=(const DbValue &other){
	if (this != &other){
		this->m_val = other.m_val;
	}
	return (*this);
}// DbValue
DbValue::~DbValue() {
}
/////////////////////////////////////////////
} /* namespace sqlite */
