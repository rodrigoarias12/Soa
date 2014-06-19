project(TP4)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(TP4_SRCS TP4.cpp main.cpp)
qt4_automoc(${TP4_SRCS})
add_executable(TP4 ${TP4_SRCS})
target_link_libraries(TP4 ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS TP4 RUNTIME DESTINATION bin)
