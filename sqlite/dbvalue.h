/*
 * dbvalue.h
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#ifndef DBVALUE_H_
#define DBVALUE_H_
//////////////////////////////
#include <cassert>
#include <memory>
#include <boost/any.hpp>
////////////////////////////
namespace sqlite {
//////////////////////////////////
typedef unsigned char byte;
///////////////////////////////////
class Blob {
private:
	size_t m_size;
	std::unique_ptr<byte> m_data;
public:
	Blob();
	Blob(size_t nSize);
	Blob(const byte *pData, size_t nSize);
	Blob(const Blob &other);
	Blob & operator=(const Blob &other);
	virtual ~Blob();
public:
	inline size_t size(void) const {
		return (this->m_size);
	}
	inline const byte *data(void) const {
		return (this->m_data.get());
	}
	inline byte *data(void) {
		return (this->m_data.get());
	}
	void data(const byte *pData, size_t nSize);
};
///////////////////////////////////////
typedef Blob *pBlob;
///////////////////////////////////////////
// class Blob
//////////////////////////////////
class DbValue {
private:
	boost::any m_val;
public:
	DbValue();
	DbValue(const DbValue &other);
	DbValue & operator=(const DbValue &other);
	virtual ~DbValue();
};
//////////////////////////////
} /* namespace sqlite */
#endif /* DBVALUE_H_ */
