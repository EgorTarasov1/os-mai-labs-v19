cmake_minimum_required(VERSION 3.10) # Установка минимальной версии CMake. Может потребоваться изменить в зависимости от вашей версии.

project(MultiProcessApplication) # Название проекта

# Установка стандарта C++.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавление исполняемого файла для родительского процесса.
add_executable(parent parent.cpp)

# Создание исполняемых файлов для дочерних процессов.
add_executable(ChildProcess1 child.cpp)
add_executable(ChildProcess2 child.cpp)

# Установка директив компилятора для Windows (если требуется).
if(MSVC)
    target_compile_definitions(parent PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(child PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()