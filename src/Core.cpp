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

/* Definitions */
#define IS_EXIST(x) std::cout << x << " already installed\nWould you like to run it? (y/n) : ";
#define IS_NOT_EXIST(x) std::cout << x << " is not installed.\nDo you want to install " << x << " from source? (y/n) : ";

#define IS_NOT_SUPER_USER(x) std::cout << "Use '" << x << "' as super user.\n"
#define IS_NOT_FOUND(x) std::cout << x << " not found. Aborted.\n";

#define CANNOT_BE_REMOVED(x) std::cout << "Really? " << x << " is not installed. Cannot be removed.\n";
#define UNINSTALL(x) std::cout << "Do you want to uninstall " << x << "? (y/n) : ";

static std::string installed = "✅ ";
static std::string uninstalled = "❎ ";

void HelpFunction();

/*
	int
		uninstall = 0 : Install
		uninstall = 1 : Uninstall
*/
void
Fpm::Install(std::string name, std::string repository, std::string object, std::string folder, int uninstall) {
	ExecutePlusPlus exec;
	/* Check is exist? */
	if(fsplusplus::IsExistFile("/bin/" + object) == false) {
		if(uninstall == 1)
			CANNOT_BE_REMOVED(name)
		else {
			IS_NOT_EXIST(name)
			char input = getchar();
			if(input == 'y' || input == 'Y') {
				if(fsplusplus::IsExistFile("/bin/git") == true || fsplusplus::IsExistFile("/usr/bin/git") == true) {
					chdir(getenv("HOME"));
					
					system((STR("git clone ") + repository + STR(" &>/dev/null")).c_str());
					
					IntelligenTUI::ProgressBar(std::clog, 20, "", "=");
					
					if(fsplusplus::IsExistFile("/bin/g++") == true) {
						if(fsplusplus::IsExistFile("/bin/gcc") == true) {
							std::string path(getenv("HOME"));
							path.append("/" + folder);
							
							chdir(path.c_str());
							
							IntelligenTUI::ProgressBar(std::clog, 20, "", "=");
							
							#ifdef __FreeBSD__
								if (getuid())
									IS_NOT_SUPER_USER(name)
								else
								 	system("sh install.sh");
							#else
								system("sudo sh install.sh &>/dev/null");
							#endif

							if(fsplusplus::IsExistFile("/bin/" + object) == true)
								std::cout << "Installed!\n";
							else
								std::cout << "Could not install.\n";
						
							chdir(getenv("HOME"));
								
							std::cout << "Cleaning..\n";
							
							std::filesystem::remove_all(STR(getenv("HOME")) + "/" + folder);
						
							IntelligenTUI::ProgressBar(std::clog, 10, "", "=");
						} else
							IS_NOT_FOUND("gcc")
					} else
						IS_NOT_FOUND("g++")
				} else
					IS_NOT_FOUND("git")
			} else
				std::cout << "Aborted.\n";
		}
	} else {
		if(uninstall == 1) {
			UNINSTALL(name)
			char input = getchar();
			if(input == 'y' || input == 'Y') {
				exec.RunFunction("sudo rm -f /bin/" + object);
				if(fsplusplus::IsExistFile("/bin/" + object) == false)
					std::cout << "Removed!\n";
				else
					std::cout << "Could not remove.\n";
			} else
				std::cout << "Aborted.\n";
		} else {
			IS_EXIST(name)
			char input = getchar();
			if(input == 'y' || input == 'Y')
				exec.RunFunction(object);
			else
				std::cout << "Aborted.\n";
		}
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
	RESETW printfc({8, 199, 107}, "Fegeya Package Manager");
	
	RESETW printfc({245, 178, 7}, "(fpm)\n");
	 
	RESETW printfc({6, 140, 75}, "Usage: fpm [--i --install || --uni --uninstall | --info | --update] app\n");
}

void
Fpm::InstallFunction(std::string arg) {
	FParser parser;
	
	std::cout << "Checking..\n";
	
	if(fsplusplus::IsExistFile(STR(DEFAULT_DIRECTORY) + "/packages/") != true) {
		FGet get;
		
		get.FetchRepositoryData(STR(DEFAULT_FPI_REPOSITORY));
	}
	
	parser.ParseRepositoryFile(arg);
	
	Install(parser.app_name, parser.app_repo, parser.app_exec, parser.app_folder, 0); 
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
	
	Install(parser.app_name, parser.app_repo, parser.app_exec, parser.app_folder, 1); 
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
	std::cout << "App: " << package.app_name + "\n";
	
	/* Description of package */
	std::cout << "Desc: " << package.app_desc + "\n";
	
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
