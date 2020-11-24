/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#ifndef CORE_HPP
#define CORE_HPP

#include <iostream>
#include <cstring>

#include "Parser.hpp"

inline std::string STR(const char* _ch) {
	return (std::string)_ch;
}

class Fpm {
public:
	void Install(FParser &package, int type); /* int : Uninstall */

	void InstallFunction(std::string); /* Arg, Type */
	void UnInstallFunction(std::string); /* Arg, Type */
	void InfoFunction(std::string arg);
	void KeepFunction(std::string arg);
	
	void Check(std::string arg);
	void Keep(FParser &arg);
	void Info(FParser &package);

	void UpdatePackageList(std::string package);
	void HelpFunction();
};

#endif // CORE_HPP
