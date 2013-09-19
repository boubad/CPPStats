/*
 * workdatabase.cpp
 //////////
 *
 *  Created on: 19 sept. 2013
 *      Author: Boubacar
 */
////////////////////////////////////////
#include "../statdataset/importdata.h"
/////////////////////////////
#include "../intrasqlite/statdatamanager.h"
//////////////////////////////
using namespace std;
using namespace intra;
using namespace intrasqlite;
///////////////////////////////////
extern bool import_data(const std::string &srcfilename,
		const std::string &databaseFilename, const std::string &datasetSigle) {
	typedef int IndexType;
	typedef std::string StringType;
	//
	typedef ImportData<StringType> ImportDataType;
	typedef ImportDataType::AnyVectorType AnyVectorType;
	typedef StatDataset<StringType> DatasetType;
	typedef DatasetType::VariableType VariableType;
	typedef DatasetType::IndivType IndivType;
	typedef DatasetType::VariablesMapType VariablesMapType;
	typedef DatasetType::IndivsMapType IndivsMapType;
	//
	try {
		ImportDataType oData;
		std::ifstream in(srcfilename.c_str());
		std::string na = "na";
		char delim = '\t';
		oData.open(in, delim, na);
		size_t ncols = oData.cols();
		if (ncols < 1) {
			return (false);
		}
		//
		StatDataManager oMan(databaseFilename);
		bool bRet = oMan.is_valid();
		if (!bRet) {
			return (false);
		}
		DatasetType oDataset;
		bRet = oMan.get_dataset_by_sigle(datasetSigle, oDataset);
		if ((!bRet) || (oDataset.id() == 0)) {
			oDataset.sigle(datasetSigle);
			oDataset.name(datasetSigle);
			oDataset.description(datasetSigle);
			bRet = oMan.insert_dataset(oDataset);
			if (!bRet) {
				return false;
			}
			bRet = oMan.get_dataset_by_sigle(datasetSigle, oDataset);
			if (!bRet) {
				return (false);
			}
		}
		int nId = oDataset.id();
		if (nId == 0) {
			return (false);
		}
		std::vector<VariableType> vars;
		if (!oMan.get_dataset_variables(nId, vars)) {
			return (false);
		}
		if (!vars.empty()) {
			if (!oMan.remove_variables(vars)) {
				return (false);
			}
			vars.clear();
		}
		//
		int nIndex = -1;
		for (size_t icol = 0; icol < ncols; ++icol) {
			StringType siglevar = oData.colname(icol);
			if (siglevar == "Dossier") {
				nIndex = icol;
				continue;
			}
			VariableType vk;
			if (oMan.get_variable_by_dataset_and_sigle(nId, siglevar, vk)) {
				continue;
			}
			VariableType oVar;
			oVar.dataset_id(nId);
			oVar.sigle(siglevar);
			oVar.name(siglevar);
			oVar.description(siglevar);
			if (oData.is_num_var(icol)) {
				oVar.var_type("float");
				oVar.is_categvar(false);
			} else {
				oVar.var_type("string");
				oVar.is_categvar(true);
			}
			vars.push_back(oVar);
		} // icol
		if (!vars.empty()) {
			if (!oMan.maintains_variables(vars)) {
				return (false);
			}
		}
		if (nIndex < 0) {
			return (true);
		}
		if (!oMan.get_dataset_variables(nId, vars)) {
			return (false);
		}
		VariablesMapType &mvars = oDataset.variables();
		for (auto it = vars.begin(); it != vars.end(); ++it) {
			VariableType v = *it;
			int key = v.id();
			std::string s = v.sigle();
			mvars[key] = v;
		} // it
		size_t nrows = oData.rows();
		if (nrows < 1) {
			return (true);
		}
		std::vector<IndivType> oInds;
		for (size_t i = 0; i < nrows; ++i) {
			double d = -1.0;
			if (oData.get_num_value(i, nIndex, d)) {
				int nx = (int) d;
				if (nx > 0) {
					std::stringstream os;
					os << "I" << nx;
					std::string sigle = os.str();
					IndivType ind0;
					if (oMan.get_indiv_by_dataset_sigle(nId, sigle, ind0)) {
						continue;
					}
					IndivType ind;
					ind.dataset_id(nId);
					ind.sigle(sigle);
					ind.name(sigle);
					ind.description(sigle);
					oInds.push_back(ind);
				} // nx
			}
		} // i
		if (!oInds.empty()) {
			if (!oMan.maintains_indivs(oInds)) {
				return (false);
			}
		}
		int nIndivs = 0;
		if (!oMan.get_dataset_indivs_count(nId, nIndivs)) {
			return (false);
		}
		if (nIndivs < 1) {
			return (true);
		}
		std::vector<IndivType> oVecx;
		if (!oMan.get_dataset_indivs(nId, oVecx)) {
			return (false);
		}
		IndivsMapType &mz = oDataset.indivs();
		for (auto it = oVecx.begin(); it != oVecx.end(); ++it){
			IndivType ind = *it;
			int key = ind.id();
			mz[key] = ind;
		}// it
		  //
		for (size_t icol = 0; icol < ncols; ++icol) {
			std::string siglecol = oData.colname(icol);
			if (siglecol == "Dossier") {
				continue;
			}
			const VariableType *pVar = oDataset.find_variable_by_sigle(
					siglecol);
			if (pVar == nullptr) {
				continue;
			}
			int nVarId = pVar->id();
			std::vector<StatValue> oVec;
			std::vector<boost::any> vals;
			oData.get_col_values(icol, vals);
			for (size_t irow = 0; irow < nrows; ++irow) {
				double d = -1.0;
				if (oData.get_num_value(irow, nIndex, d)) {
					int nx = (int) d;
					if (nx > 0) {
						std::stringstream os;
						os << "I" << nx;
						std::string sigleindiv = os.str();
						const IndivType *pInd = oDataset.find_indiv_by_sigle(sigleindiv);
						if (pInd == nullptr) {
							continue;
						}
						int nIndivId = pInd->id();
						boost::any v0;
						if (oData.get_value(irow, icol, v0)) {
							StatValue cur(v0);
							cur.indiv_id(nIndivId);
							cur.variable_id(nVarId);
							oVec.push_back(cur);
						}
					} // nx
				}
			}		// irow
			if (oVec.empty()) {
				continue;
			}
			if (!oMan.maintains_values(oVec)) {
				return (false);
			}
		}		// icol
	} catch (std::exception &ex) {
		std::cout << std::endl << "ERROR...!: " << ex.what() << std::endl;
		return (false);
	} catch (...) {
		return (false);
	}
	return (true);
} // get_import_data

