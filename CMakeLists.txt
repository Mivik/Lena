cmake_minimum_required(VERSION 3.16)
project(Lena)

set(CMAKE_CXX_STANDARD 17)

set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/res)

add_executable(Lena
		ui_main.cpp
		main_window.cpp
		tray_icon.cpp
		lena_client.cpp
		common.cpp
		request_parser.cpp
		)

find_package(PkgConfig)

pkg_search_module(GTKMM REQUIRED gtkmm-3.0)
include_directories(Lena ${GTKMM_INCLUDE_DIRS})
link_directories(Lena ${GTKMM_LIBRARY_DIRS})
target_link_libraries(Lena ${GTKMM_LIBRARIES})

pkg_search_module(APP_INDICATOR REQUIRED appindicator3-0.1)
include_directories(Lena ${APP_INDICATOR_INCLUDE_DIRS})
link_directories(Lena ${APP_INDICATOR_LIBRARY_DIRS})
target_link_libraries(Lena ${APP_INDICATOR_LIBRARIES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Lena Threads::Threads)
