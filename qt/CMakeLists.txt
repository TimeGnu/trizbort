# Copyright (c) 2026  Jason Self <j@jxself.org>
#
# This file is free software: you may copy, redistribute and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or (at your option)
# any later version.  There is NO WARRANTY, to the extent permitted by law.
# See the GNU General Public License for more details:
# https://www.gnu.org/licenses/.

cmake_minimum_required(VERSION 3.16)
project(trizbort-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(trizbort-qt
    src/main.cpp
    src/MapDocument.cpp
    src/TrizbortReader.cpp
    src/MapScene.cpp
    src/MapView.cpp
    src/MainWindow.cpp
    src/export/ExportModel.cpp
    src/export/CodeExporter.cpp
    src/export/ZilExporter.cpp
    src/export/AdventuronExporter.cpp
    src/export/HugoExporter.cpp
    src/export/AlanExporter.cpp
    src/export/TadsExporter.cpp
    src/export/Inform6Exporter.cpp
    src/export/Inform7Exporter.cpp
)

target_link_libraries(trizbort-qt PRIVATE Qt6::Widgets)
