/* MIT License
#
# Copyright (c) 2020 Ferhat Geçdoğan All Rights Reserved.
# Distributed under the terms of the MIT License.
#
# */

#include <iostream>
#include <sstream>
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

/* Definitions */
#define IS_EXIST(x) std::cout << x << " already installed\nWould you like to run it? (y/n) : ";
#define IS_NOT_EXIST(x) std::cout << x << " is not installed.\nDo you want to install " << x << " from source? (y/n) : ";

#define IS_NOT_SUPER_USER(x) std::cout << "Use '" << x << "' as super user.\n"
#define IS_NOT_FOUND(x) std::cout << x << " not found. Aborted.\n";

#define CANNOT_BE_REMOVED(x) std::cout << "Really? " << x << " is not installed. Cannot be removed.\n";
#define UNINSTALL(x) std::cout << "Do you want to uninstall " << x << "? (y/n) : ";

#define KEEP(x) std::cout << "Do you want to keep " << x << "? (y/n) : ";

static std::string installed = "✅ ";
static std::string uninstalled = "❎ ";

void HelpFunction();

/*
	int
		uninstall = 0 : Install
		uninstall = 1 : Uninstall
*/

/*
	TODO:
	- Create file for log of build
*/

void
Fpm::Install(FParser &package, int uninstall) {
	if(uninstall == 0) 
		InstallDependencies(package);
	
	ExecutePlusPlus exec;
	
	/* Check is exist? */
	if(package.app_exec != "<LIBRARY>") {
		if(fsplusplus::IsExistFile("/bin/" + package.app_exec) != false) {
			if(uninstall == 1) {
				UNINSTALL(package.app_name)
				
				char input;
				std::cin >> input;
				
				if(input == 'y' || input == 'Y') {
					exec.RunFunction("sudo rm -f /bin/" + package.app_exec);
					if(fsplusplus::IsExistFile("/bin/" + package.app_exec) == false)
						std::cout << "Removed!\n";
					else
						std::cout << "Could not remove.\n";
				} else
					std::cout << "Aborted.\n";
			
				return;
			} else {
				IS_EXIST(package.app_name)
				char input;
				
				std::cin >> input;
				
				if(input == 'y' || input == 'Y')
					exec.RunFunction(package.app_exec);
				else
					std::cout << "Aborted.\n";
			}
		}
	}
	
	if(uninstall == 1)
		CANNOT_BE_REMOVED(package.app_name)
	else {
	    char input = 'y';

	    if(uninstall != 3) {
		    IS_NOT_EXIST(package.app_name)
		    std::cin >> input;
        }

		if(input == 'y' || input == 'Y') {
			if(fsplusplus::IsExistFile("/bin/" + package.app_scm) == true || fsplusplus::IsExistFile("/usr/bin/" + package.app_scm) == true) {
				chdir(getenv("HOME"));
				
				std::cout << "\r(" + package.app_exec + ") Fetching.." << std::flush;
				
				system((package.app_scm + STR(" clone ") + package.app_repo + STR(" 2>/dev/null")).c_str());
				
				if(fsplusplus::IsExistFile("/bin/g++") == true) {
					if(fsplusplus::IsExistFile("/bin/gcc") == true) {
						std::string path(getenv("HOME"));
						path.append("/" + package.app_folder);
							
						chdir(path.c_str());
						
						std::cout << "\r(" + package.app_exec + ") Installing.." << std::flush;
						
						std::istringstream build(package.app_build_instruction);
						std::string build_string;
							 					
						#ifdef __FreeBSD__
							if (getuid())
								IS_NOT_SUPER_USER(package.app_name)
						#endif
						
						while(std::getline(build, build_string)) {
							system((build_string + STR(" 2>/dev/null")).c_str());
						}

						if(package.app_exec != "<LIBRARY>") { 
							if(fsplusplus::IsExistFile("/bin/" + package.app_exec) == true)
								std::cout << "\r(" + package.app_exec + ") Installed!" << std::flush;
							else
								std::cout << "\r(" + package.app_exec + ") Could not install.." << std::flush;
						}
							
						chdir(getenv("HOME"));
								
						if (getuid())
							std::cout << "Use 'fpm' as super user\n";
						else
							std::filesystem::remove_all(STR(getenv("HOME")) + "/" + package.app_folder);

						std::cout << "\r(" + package.app_exec + ") Cleaning..\n";
					} else
						IS_NOT_FOUND("gcc")
				} else
					IS_NOT_FOUND("g++")
			} else
				IS_NOT_FOUND(package.app_scm)
		} else
			std::cout << "Aborted.\n";
	}	
}


void
Fpm::DirectInstall(FParser &package) {
	ExecutePlusPlus exec;
		
	if(fsplusplus::IsExistFile("/bin/" + package.app_scm) == true 
		|| fsplusplus::IsExistFile("/usr/bin/" + package.app_scm) == true) {
		chdir(getenv("HOME"));
					
		system((package.app_scm + STR(" clone ") + package.app_repo + STR(" 2>/dev/null")).c_str());
				
		IntelligenTUI::ProgressBar(std::clog, 10, "", "=", "[Fetching..]");
				
		if(fsplusplus::IsExistFile("/bin/g++") == true) {
			if(fsplusplus::IsExistFile("/bin/gcc") == true) {
				std::string path(getenv("HOME"));
				path.append("/" + package.app_folder);
							
				chdir(path.c_str());
						
				IntelligenTUI::ProgressBar(std::clog, 10, "", "=", "[Installing]");
						
				std::istringstream build(package.app_build_instruction);
				std::string build_string;
							 					
				#ifdef __FreeBSD__
					if (getuid())
						IS_NOT_SUPER_USER(package.app_name)
				#endif
						
				while(std::getline(build, build_string)) {
					system((build_string + STR(" 2>/dev/null")).c_str());
				}

				chdir(getenv("HOME"));
								
				if (getuid())
					std::cout << "Use 'fpm' as super user\n";
				else
					std::filesystem::remove_all(STR(getenv("HOME")) + "/" + package.app_folder);

			} else
					IS_NOT_FOUND("gcc")
		} else
				IS_NOT_FOUND("g++")
	}
}

void Check_Installed(std::string name, std::string data, std::string object) {
	if(fsplusplus::IsExistFile("/bin/" + object) == true) {
		std::cout << installed << WBOLD_GREEN_COLOR << name + " ";
		
		RESETW printfc({160, 255, 8}, "(" + data + ")\n");
	} else {
		std::cout << uninstalled << WBOLD_GREEN_COLOR << name + " ";
		
		RESETW printfc({245, 178, 7}, "(" + data + ")\n");
	}
}

void Fpm::HelpFunction() {
	RESETW printfc({8, 199, 107}, "Fegeya Package Manager ");

	RESETW printfc({245, 178, 7}, "(fpm) - ");

	RESETW printfc({192, 215, 37}, FPM_FULL_VERSION);
	 
	RESETW printfc({6, 140, 75}, "\nUsage: fpm [options] package\n");
	RESETW printfc({245, 178, 7}, "-------\n");

	RESETW 
	printfc({235, 97, 35}, 
	STR("--i --install 	   : Install package\n") +
	STR("--uni --uninstall  : Uninstall package\n") +
	STR("--inf --info 	   : More information of package\n") +
	STR("--k --keep 	   : Clone & keep source code of package\n"));
}

void
Fpm::InstallFunction(std::string arg) {
	FParser parser;

	std::cout << "Checking..\n";
    std::istringstream packages(arg);

    std::string package, install;
    char answer = 'y';

    if(fsplusplus::IsExistFile(STR(DEFAULT_DIRECTORY) + "/packages/") != true) {
		FGet get;
		
		get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
	}
	
	std::cout << "The following packages will be installed:\n\n";

    LIGHT_GREEN_COLOR

	while(std::getline(packages, package)) {
	    parser.ParseRepositoryFile(package);

        if(fsplusplus::IsExistFile("/bin/" + parser.app_exec) == false) {
            std::cout << "   " << parser.app_name + " (" +
                WBOLD_YELLOW_COLOR + parser.app_exec + WLIGHT_GREEN_COLOR + ")\n";

            install.append(package + "\n");
        }
	}

    RESETB

    std::cout << "\nDo you want to continue? (Y/n) : ";
    std::cin >> answer;

    packages.str("");

    if(answer == 'n' || answer == 'N') {
        package.erase();
        install.erase();

        std::cout << "Aborted.\n";
        return;
    }

    package.erase();

    std::istringstream install_packages(install);

    while(std::getline(install_packages, package)) {
        parser.ParseRepositoryFile(package);

        Install(parser, 3);
    }

    install_packages.str("");
}


void
Fpm::UnInstallFunction(std::string arg) {
	FParser parser;
	
	std::cout << "Checking..\n";
	
	if(fsplusplus::IsExistFile(STR(DEFAULT_DIRECTORY) + "/packages/") != true) {
		FGet get;
		
		get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
	}

	parser.ParseRepositoryFile(arg);
	
	Install(parser, 1); 
}


void
Fpm::KeepFunction(std::string arg) {
	FParser parser;
	
	if(fsplusplus::IsExistFile(STR(DEFAULT_DIRECTORY) + "/packages/") != true) {
		FGet get;
		
		get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
	}
	
	parser.ParseRepositoryFile(arg);	

	Keep(parser);
}

void
Fpm::Keep(FParser &package) {
	KEEP(package.app_name); 
	
	char input = getchar();
	
	if(input == 'y' || input == 'Y') {
		chdir(getenv("HOME"));
					
		system((package.app_scm + STR(" clone ") + package.app_repo + STR(" 2>/dev/null")).c_str());
					
		IntelligenTUI::ProgressBar(std::clog, 3, "", "=", "[Fetching]");
		std::cout << "\n";
	} else { std::cout << "Aborted.\n"; }
}

void 
Fpm::InfoFunction(std::string arg) {
	FParser parser;
	
	if(fsplusplus::IsExistFile(STR(DEFAULT_DIRECTORY) + "/packages/") != true) {
		FGet get;
		
		get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
	}
	
	parser.ParseRepositoryFile(arg);
	
	Info(parser);
}

void
Fpm::Info(FParser &package) {
	/* Name of package */
	std::cout << "App: " << package.app_name;
	
	std::cout << "\n-------\n";
	
	/* Description of package */	
	std::cout << "Desc: " << package.app_desc + "\n";
	
	/* Language of package */
	std::cout << "Language: " << package.app_lang + "\n";
	
	/* Author of package */
	std::cout << "Author: " << package.app_author + "\n"; 

	/* License of package */
	std::cout << "License: " << package.app_license + "\n";
	
	/* Executable file of package */
	std::cout << "File: " << package.app_exec + "\n";
	
	/* Repository of package */
	std::cout << "Repository: " << package.app_repo + "\n";
}

void
Fpm::InstallDependencies(FParser &package) {
	if(package.app_recipe != "Nothing.") {	
		FParser parser;
		std::istringstream required(package.app_recipe);
		std::string required_packages;
	
	
		if(fsplusplus::IsExistFile(STR(DEFAULT_DIRECTORY) + "/packages/") != true) {
			FGet get;
		
			get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
		}
		
		while(std::getline(required, required_packages)) {
			required_packages = stringtools::EraseAllSubString(required_packages, "\n");
			required_packages = stringtools::EraseAllSubString(required_packages, " ");
			
			if(required_packages.length() != 0) {
				parser.ParseRepositoryFile(required_packages);
			
				if(parser.is_installed != true) {
					std::cout << "Required package/s: " << parser.app_folder + "\n";
					DirectInstall(parser);
				}
			}
		}
	}				
}

void
Fpm::UpdatePackageList(std::string package) {
	FGet get;
	
	if(fsplusplus::IsExistFile(STR(DEFAULT_DIRECTORY) + "/packages/") != true) {
		std::cout << "Fetching from DEFAULT_FPI_REPOSITORY...";
			
		get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
	} else {
		std::cout << "Is this ok? (y/N) : ";
		char ok = getchar();
	
		if(package == "repository" || package == "all") {
			if(ok == 'y' || ok == 'Y') {
				std::filesystem::remove_all(STR(DEFAULT_DIRECTORY) + "/");
			
				get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
			} else std::cout << "Aborted.\n"; 
		}
	}
}
