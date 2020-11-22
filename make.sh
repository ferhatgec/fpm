#!/bin/sh

c++ -std=c++17 -I./include/ -I./libraries/ src/Parser.cpp src/Get.cpp src/Core.cpp src/Fpm.cpp -o fpm
