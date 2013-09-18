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
//////////////////////
#ifndef __MY_BOOST_INC__
#define __MYBOOST_INC__
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>
#endif // __MY_BOOST_UNC__
/////////////////////////////////////
namespace intra {
///////////////////////////////////////
class StatValue {
private:
	int m_id;
	int m_optlock;
	int m_variableid;
	int m_individ;
	boost::any m_val;
public:
	StatValue() :
			m_id(0), m_optlock(1), m_variableid(0), m_individ(0) {
	}
	StatValue(const StatValue &other) :
			m_id(other.m_id), m_optlock(other.m_optlock), m_variableid(
					other.m_variableid), m_individ(other.m_individ), m_val(
					other.m_val) {
	} // StatIndiv
	StatValue & operator=(const StatValue &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_optlock = other.m_optlock;
			this->m_variableid = other.m_variableid;
			this->m_individ = other.m_individ;
		}
		return (*this);
	} // operator=
	virtual ~StatValue() {
	}
	bool operator==(const StatValue &other) const {
		if ((this->m_id != 0) && (other.m_id != 0)) {
			return (this->m_id == other.m_id);
		}
		return ((this->m_variableid == other.m_variableid)
				&& (this->m_individ == other.m_individ));
	} // operator==
	bool operator<(const StatValue &other) const {
		if (this->m_variableid < other.m_variableid) {
			return (true);
		} else if (m_variableid > other.m_variableid) {
			return (false);
		}
		return (this->m_individ < other.m_individ);
	} // operator==
	  ////////////////////////////
public:
	inline int id(void) const {
		return (this->m_id);
	}
	inline void id(int n) {
		this->m_id = n;
	}
	inline int variable_id(void) const {
		return (this->m_variableid);
	}
	inline void variable_id(int n) {
		this->m_variableid = n;
	}
	inline int indiv_id(void) const {
		return (this->m_individ);
	}
	inline void indiv_id(int n) {
		this->m_individ = n;
	}
	inline int version(void) const {
		return (this->m_optlock);
	}
	inline void version(int n) {
		this->m_optlock = n;
	}
	inline const boost::any & value(void) const {
		return (this->m_val);
	}
	inline void value(const boost::any &s) {
		this->m_val = s;
	}
	inline void value(bool bVal) {
		this->m_val = boost::any(bVal);
	}
	inline void value(int v) {
		this->m_val = boost::any(v);
	}
	inline void value(float v) {
		this->m_val = boost::any(v);
	}
	inline void value(double v) {
		this->m_val = boost::any(v);
	}
	inline void value(const std::string &v) {
		this->m_val = boost::any(v);
	}
	inline void value(const std::wstring &v) {
		this->m_val = boost::any(v);
	}
	inline bool is_valid(void) const {
		return ((this->m_variableid > 0) && (this->m_individ > 0));
	}
	inline bool is_empty(void) const {
		return (this->m_val.empty());
	}
	inline bool is_updateable(void) const {
		return (this->m_id > 0);
	}
	inline bool is_removeable(void) const {
		return (this->m_id > 0);
	}
public:
	bool bool_value(void) const {
		bool oRet = false;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				oRet = boost::any_cast<bool>(v);
			} else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (x != 0) ? true : false;
			} else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (x != 0) ? true : false;
			} else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (x != 0.0f) ? true : false;
			} else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (x != 0.0) ? true : false;
			} else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					auto c = *(xx.begin());
					oRet = (c == 't') || (c == '1') || (c == 'v') || (c == 'o');
				} // not empty
			} else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					auto c = *(xx.begin());
					oRet = (c == L't') || (c == L'1') || (c == L'v')
							|| (c == L'o');
				} // not empty
			}
		} // not empty
		return (oRet);
	} // bool_value
	short short_value(void) const {
		short oRet = -1;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			} else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = x;
			} else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (short) x;
			} else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (short) x;
			} else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (short) x;
			} else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			} else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
		} // not empty
		return (oRet);
	} // short_value
	int int_value(void) const {
		int oRet = -1;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			} else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (int) x;
			} else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = x;
			} else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (int) x;
			} else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (int) x;
			} else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			} else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
		} // not empty
		return (oRet);
	} // int_value
	float float_value(void) const {
		float oRet = -1.0f;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1.0f : 0.0f;
			} else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (float) x;
			} else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (float) x;
			} else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = x;
			} else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (float) x;
			} else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			} else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
		} // not empty
		return (oRet);
	} // float_value
	double double_value(void) const {
		double oRet = -1.0;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1.0 : 0.0;
			} else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (double) x;
			} else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (double) x;
			} else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (double) x;
			} else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = x;
			} else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			} else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
		} // not empty
		return (oRet);
	} // double_value
	bool string_value(std::string &s) const {
		bool bRet = false;
		s.clear();
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				s = (b) ? "True" : "False";
				bRet = true;
			} else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				s = x;
				bRet = true;
			} else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::string sx(x.length(), ' ');
				std::copy(x.begin(), x.end(), sx.begin());
				s = sx;
				bRet = true;
			}
		} // not empty
		return (bRet);
	} // string_value
	bool string_value(std::wstring &s) const {
		bool bRet = false;
		s.clear();
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				s = (b) ? L"True" : L"False";
				bRet = true;
			} else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			} else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				s = x;
				bRet = true;
			} else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::wstring sx(x.length(), L' ');
				std::copy(x.begin(), x.end(), sx.begin());
				s = sx;
				bRet = true;
			}
		} // not empty
		return (bRet);
	} // string_value
};
/////////////////////////////////////
template<class TSTRING = std::string>
class StatIndiv {
public:
	typedef TSTRING StringType;
	typedef StatIndiv<TSTRING> IndivType;
private:
	int m_id;
	int m_datasetid;
	int m_optlock;
	StringType m_sigle;
	StringType m_name;
	StringType m_desc;
public:
	StatIndiv() :
			m_id(0), m_datasetid(0), m_optlock(1) {
	}
	StatIndiv(const IndivType &other) :
			m_id(other.m_id), m_datasetid(other.m_datasetid), m_optlock(
					other.m_optlock), m_sigle(other.m_sigle), m_name(
					other.m_name), m_desc(other.m_desc) {
	} // StatIndiv
	IndivType & operator=(const IndivType &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_datasetid = other.m_datasetid;
			this->m_optlock = other.m_optlock;
			this->m_sigle = other.m_sigle;
			this->m_name = other.m_name;
			this->m_desc = other.m_desc;
		}
		return (*this);
	} // operator=
	virtual ~StatIndiv() {
	}
	bool operator==(const IndivType &other) const {
		if ((this->m_id != 0) && (other.m_id != 0)) {
			return (this->m_id == other.m_id);
		}
		return ((this->m_datasetid == other.m_datasetid)
				&& (this->m_sigle == other.m_sigle));
	} // operator==
	bool operator<(const IndivType &other) const {
		if (this->m_datasetid < other.m_datasetid) {
			return (true);
		} else if (m_datasetid > other.m_datasetid) {
			return (false);
		}
		return (this->m_sigle < other.m_sigle);
	} // operator==
	  ////////////////////////////
public:
	inline int id(void) const {
		return (this->m_id);
	}
	inline void id(int n) {
		this->m_id = n;
	}
	inline int dataset_id(void) const {
		return (this->m_datasetid);
	}
	inline void dataset_id(int n) {
		this->m_datasetid = n;
	}
	inline int version(void) const {
		return (this->m_optlock);
	}
	inline void version(int n) {
		this->m_optlock = n;
	}
	inline const StringType & sigle(void) const {
		return (this->m_sigle);
	}
	inline void sigle(const StringType &s) {
		this->m_sigle = boost::trim_copy(s);
	}
	inline const StringType &name(void) const {
		return (this->m_name);
	}
	inline void name(const StringType &s) {
		this->m_name = boost::trim_copy(s);
	}
	inline const StringType & description(void) const {
		return (this->m_desc);
	}
	inline void description(const StringType &s) {
		this->m_desc = boost::trim_copy(s);
	}
	inline bool is_valid(void) const {
		return ((this->m_datasetid > 0) && (!this->m_sigle.empty()));
	}
	inline bool is_updateable(void) const {
		return ((this->m_id > 0) && (!this->m_sigle.empty()));
	}
	inline bool is_removeable(void) const {
		return (this->m_id > 0);
	}
};
/////////////////////////////////////
template<class TSTRING = std::string, class ALLOCANYPAIR = std::allocator<
		std::pair<int, boost::any> > >
class StatVariable {
public:
	typedef TSTRING StringType;
	typedef StatVariable<TSTRING> VariableType;
	typedef std::map<int, boost::any, std::less<int>, ALLOCANYPAIR> ValuesMapType;
private:
	int m_id;
	int m_datasetid;
	int m_optlock;
	int m_categvar;
	StringType m_type;
	StringType m_sigle;
	StringType m_name;
	StringType m_desc;
	ValuesMapType m_valuesmap;
public:
	StatVariable() :
			m_id(0), m_datasetid(0), m_optlock(1), m_categvar(0) {
	}
	StatVariable(const VariableType &other) :
			m_id(other.m_id), m_datasetid(other.m_datasetid), m_optlock(
					other.m_optlock), m_categvar(other.m_categvar), m_type(
					other.m_type), m_sigle(other.m_sigle), m_name(other.m_name), m_desc(
					other.m_desc), m_valuesmap(other.m_valuesmap) {
	} // StatDataset
	VariableType & operator=(const VariableType &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_datasetid = other.m_datasetid;
			this->m_optlock = other.m_optlock;
			this->m_categvar = other.m_categvar;
			this->m_type = other.m_type;
			this->m_sigle = other.m_sigle;
			this->m_name = other.m_name;
			this->m_desc = other.m_desc;
			this->m_valuesmap = other.m_valuesmap;
		}
		return (*this);
	} // operator=
	virtual ~StatVariable() {
	}
	bool operator==(const VariableType &other) const {
		if ((this->m_id != 0) && (other.m_id != 0)) {
			return (this->m_id == other.m_id);
		}
		return ((this->m_datasetid == other.m_datasetid)
				&& (this->m_sigle == other.m_sigle));
	} // operator==
	bool operator<(const VariableType &other) const {
		if (this->m_datasetid < other.m_datasetid) {
			return (true);
		} else if (m_datasetid > other.m_datasetid) {
			return (false);
		}
		if (this->m_categvar > other.m_categvar) {
			return (true);
		} else if (this->m_categvar < other.m_categvar) {
			return (false);
		}
		return (this->m_sigle < other.m_sigle);
	} // operator==
	  ////////////////////////////
public:
	inline bool has_indiv_value(int indivId) const {
		auto oMap = this->m_valuesmap;
		return (oMap.find(indivId) != oMap.end());
	}	  // has_indiv_value
	inline bool get_indiv_value(int indivId, boost::any &v) const {
		auto oMap = this->m_valuesmap;
		auto it = oMap.find(indivId);
		if (it != oMap.end()) {
			v = (*it).second;
			return (true);
		}
		return (false);
	}	  // get_indiv_value
	inline void set_indiv_value(int indivId, const boost::any &v) {
		boost::any aa(v);
		(this->m_valuesmap)[indivId] = aa;
	}	  // get_indiv_value
	inline const ValuesMapType & values_map(void) const {
		return (this->m_valuesmap);
	}
	inline ValuesMapType & values_map(void) {
		return (this->m_valuesmap);
	}
	template<class ALLOCINT>
	void get_indivs_ids(std::set<int, std::less<int>, ALLOCINT> &oSet) const {
		oSet.clear();
		auto oMap = this->m_valuesmap;
		auto iend = oMap.end();
		for (auto it = oMap.begin(); it != iend; ++it) {
			oSet.insert((*it).first);
		}	  // it
	}	  // get_indivs_ids
		  //
	inline int id(void) const {
		return (this->m_id);
	}
	inline void id(int n) {
		this->m_id = n;
	}
	inline int dataset_id(void) const {
		return (this->m_datasetid);
	}
	inline void dataset_id(int n) {
		this->m_datasetid = n;
	}
	inline int version(void) const {
		return (this->m_optlock);
	}
	inline void version(int n) {
		this->m_optlock = n;
	}
	inline bool is_categvar(void) const {
		return (this->m_categvar != 0);
	}
	inline void is_categvar(bool b) {
		this->m_categvar = (b) ? 1 : 0;
	}
	inline const StringType & var_type(void) const {
		return (this->m_type);
	}
	inline void var_type(const StringType &s) {
		this->m_type = boost::trim_copy(s);
	}
	inline const StringType & sigle(void) const {
		return (this->m_sigle);
	}
	inline void sigle(const StringType &s) {
		this->m_sigle = boost::trim_copy(s);
	}
	inline const StringType &name(void) const {
		return (this->m_name);
	}
	inline void name(const StringType &s) {
		this->m_name = boost::trim_copy(s);
	}
	inline const StringType & description(void) const {
		return (this->m_desc);
	}
	inline void description(const StringType &s) {
		this->m_desc = boost::trim_copy(s);
	}
	inline bool is_valid(void) const {
		return ((this->m_datasetid > 0) && (!this->m_sigle.empty())
				&& (!this->m_type.empty()));
	}
	inline bool is_updateable(void) const {
		return ((this->m_id > 0) && (!this->m_sigle.empty())
				&& (!this->m_type.empty()));
	}
	inline bool is_removeable(void) const {
		return (this->m_id > 0);
	}
};
/////////////////////////////////////
template<class TSTRING = std::string, class ALLOCANYPAIR = std::allocator<
		std::pair<int, boost::any> >, class ALLOCVARPAIR = std::allocator<
		std::pair<int, StatVariable<TSTRING, ALLOCANYPAIR> > >,
		class ALLOCINDIVPAIR = std::allocator<
				std::pair<int, StatIndiv<TSTRING> > > >
class StatDataset {
public:
	typedef TSTRING StringType;
	typedef StatVariable<TSTRING, ALLOCANYPAIR> VariableType;
	typedef StatIndiv<TSTRING> IndivType;
	typedef std::map<int, VariableType, std::less<int>, ALLOCVARPAIR> VariablesMapType;
	typedef std::map<int, IndivType, std::less<int>, ALLOCINDIVPAIR> IndivsMapType;
	typedef StatDataset<TSTRING, ALLOCANYPAIR, ALLOCVARPAIR, ALLOCINDIVPAIR> DatasetType;
private:
	int m_id;
	int m_optlock;
	StringType m_sigle;
	StringType m_name;
	StringType m_desc;
	IndivsMapType m_indivs;
	VariablesMapType m_variables;
public:
	StatDataset() :
			m_id(0), m_optlock(1) {
	}
	StatDataset(const DatasetType &other) :
			m_id(other.m_id), m_optlock(other.m_optlock), m_sigle(
					other.m_sigle), m_name(other.m_name), m_desc(other.m_desc), m_indivs(
					other.m_indivs), m_variables(other.m_variables) {
	} // StatDataset
	DatasetType & operator=(const DatasetType &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_optlock = other.m_optlock;
			this->m_sigle = other.m_sigle;
			this->m_name = other.m_name;
			this->m_desc = other.m_desc;
			this->m_indivs = other.m_indivs;
			this->m_variables = other.m_variables;
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
	  /////////////////////////////////
public:
	inline const VariablesMapType & variables(void) const {
		return (this->m_variables);
	}
	inline VariablesMapType & variables(void) {
		return (this->m_variables);
	}
	inline const IndivsMapType & indivs(void) const {
		return (this->m_indivs);
	}
	inline IndivsMapType & indivs(void) {
		return (this->m_indivs);
	}
	////////////////////////////
public:
	inline int id(void) const {
		return (this->m_id);
	}
	inline void id(int n) {
		this->m_id = n;
	}
	inline int version(void) const {
		return (this->m_optlock);
	}
	inline void version(int n) {
		this->m_optlock = n;
	}
	inline const StringType & sigle(void) const {
		return (this->m_sigle);
	}
	inline void sigle(const StringType &s) {
		this->m_sigle = s;
	}
	inline const StringType &name(void) const {
		return (this->m_name);
	}
	inline void name(const StringType &s) {
		this->m_name = s;
	}
	inline const StringType & description(void) const {
		return (this->m_desc);
	}
	inline void description(const StringType &s) {
		this->m_desc = s;
	}
	inline bool is_valid(void) const {
		return (!this->m_sigle.empty());
	}
	inline bool is_updateable(void) const {
		return ((this->m_id > 0) && (!this->m_sigle.empty()));
	}
	inline bool is_removeable(void) const {
		return (this->m_id > 0);
	}
};
////////////////////////////////////////

/////////////////////////////////////
} /* namespace intra */
#endif /* STATDATASET_H_ */
