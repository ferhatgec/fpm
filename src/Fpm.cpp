/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>
#include <filesystem>

#include <Core.hpp>
#include <Parser.hpp>
#include <Get.hpp>

// Libraries
#include <FileSystemPlusPlus.h>
#include <ExecutePlusPlus.hpp>
#include <StringTools.h>
#include <Colorized.hpp>
#include <IntelligenTUI.hpp>

int main(int argc, char** argv) {
	Fpm main;
	
	if(argc < 3) {
		main.HelpFunction();
		return 0;
	}
	
	std::string argument(argv[1]);
	std::string package(argv[2]);
	 
	if (!getuid()) {
		if(argument == "--i" || argument == "--install") main.InstallFunction(package); 
		else if(argument == "--uni" || argument == "--uninstall") main.UnInstallFunction(package);
		else if(argument == "--update" || argument == "--upd") main.UpdatePackageList(package);
		else if(argument == "--inf" || argument == "--info") main.InfoFunction(package);
		else if(argument == "--k" || argument == "--keep") main.KeepFunction(package);
		else
			main.HelpFunction();	
	} else {
		if(argument == "--inf" || argument == "--info") main.InfoFunction(package);
		else if(argument == "--k" || argument == "--keep") main.KeepFunction(package);
		else {
			std::cout << "Use with super-user permissions\n";
			return 0;
		}
	}
	
}
