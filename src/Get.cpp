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

#include <Core.hpp>
#include <Get.hpp>

// Libraries
#include <FileSystemPlusPlus.h>
#include <ExecutePlusPlus.hpp>
#include <StringTools.h>
#include <Colorized.hpp>

void FGet::FetchRepositoryData(std::string repository) {
	std::string path = "/etc/";
	
	if (fsplusplus::IsExistFile("/etc/fpm") != true) {
		/* home/scrift_repository/repository/scrift/app.repo */
		ExecutePlusPlus exec;
		exec.RunFunction(STR("git clone ") + repository + STR(" /etc/fpm/"));
	}
}
