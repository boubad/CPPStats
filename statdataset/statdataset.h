/*
 * Statdataset.h
 *
 *  Created on: 17 sept. 2013
 *      Author: Boubacar
 */

#ifndef STATDATASET_H_
#define STATDATASET_H_
///////////////////////////////////////
#include <cassert>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
/////////////////////////////////////
namespace intra {
//////////////////////////////////////
template<class TSTRING = std::string>
class StatDataset {
public:
	typedef TSTRING StringType;
	typedef StatDataset<TSTRING> DatasetType;
private:
	int m_id;
	int m_optlock;
	StringType m_sigle;
	StringType m_name;
	StringType m_desc;
public:
	StatDataset() :
			m_id(0), m_optlock(1) {
	}
	StatDataset(const DatasetType &other) :
			m_id(other.m_id), m_optlock(other.m_optlock), m_sigle(
					other.m_sigle), m_name(other.m_name), m_desc(other.m_desc) {
	} // StatDataset
	DatasetType & operator=(const DatasetType &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_optlock = other.m_optlock;
			this->m_sigle = other.m_sigle;
			this->m_name = other.m_name;
			this->m_desc = other.m_desc;
		}
		return (*this);
	} // operator=
	virtual ~StatDataset() {
	}
	bool operator==(const DatasetType &other) const {
		if ((this->m_id != 0) && (other.m_id != 0)) {
			return (this->m_id == other.m_id);
		}
		return (this->m_sigle == other.m_sigle);
	} // operator==
	bool operator<(const DatasetType &other) const {
		return (this->m_sigle < other.m_sigle);
	} // operator==
	  ////////////////////////////
public:
	inline int id(void) const {
		return (this->m_id);
	}
	inline void id(int n){
		this->m_id = n;
	}
	inline int version(void) const {
		return (this->m_optlock);
	}
	inline void version(int n){
		this->m_optlock = n;
	}
	inline const StringType & sigle(void) const {
		return (this->m_sigle);
	}
	inline void sigle(const StringType &s){
		this->m_sigle = s;
	}
	inline const StringType &name(void) const {
		return (this->m_name);
	}
	inline void name(const StringType &s){
		this->m_name = s;
	}
	inline const StringType & description(void) const {
		return (this->m_desc);
	}
	inline void description(const StringType &s){
		this->m_desc = s;
	}
};
/////////////////////////////////////
} /* namespace intra */
#endif /* STATDATASET_H_ */
