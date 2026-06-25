cmake_minimum_required(VERSION 4.1)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include_directories(domain)

add_executable(TicTacToe
        domain/Joc.h
        app_coordinator.cpp
        tests/Tests.cpp
        domain/validator/ValidatorException.h
        domain/validator/Validator.h
        domain/validator/Validator.cpp
        repository/RepositoryException.h
        repository/Repository.h
        repository/Repository.cpp
        service/Service.h
        service/ServiceException.h
        service/Service.cpp
        UI/GameWindow.h
        UI/MainGUI.h
        service/Observable.h
        UI/JocTableModel.h)

target_link_libraries(TicTacToe PRIVATE Qt6::Widgets)
