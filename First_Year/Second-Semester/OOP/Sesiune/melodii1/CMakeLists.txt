cmake_minimum_required(VERSION 4.1)
project(melodii1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(domain)

add_executable(melodii1
        domain/Melodie.cpp
        domain/Melodie.h
        tests/tests.h
        tests/tests.cpp
        domain/validator/ValidatorException.h
        domain/validator/ValidatorMelodie.h
        domain/validator/ValidatorMelodie.cpp
        app_coordinator.cpp
        repository/MelodieRepository.h
        repository/MelodieRepository.cpp
        repository/RepositoryException.h
        Service/ServiceException.h
        Service/MelodieService.h
        Service/MelodieService.cpp
        ui/MelodieTableModel.h
        ui/GUI.h
        ui/GUI.h
        ui/RankBarWidget.h)

target_link_libraries(melodii1 PRIVATE Qt6::Widgets)
