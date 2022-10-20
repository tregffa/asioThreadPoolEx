cmake_minimum_required (VERSION 3.8)

project ("asioThreadPoolEx")

find_package(asio CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_library(asioThreadPoolEx INTERFACE)
target_include_directories(asioThreadPoolEx
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
	    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(asioThreadPoolEx INTERFACE asio asio::asio)

# Include sub-projects.
add_subdirectory ("test")
set_property(TARGET tests PROPERTY CXX_STANDARD 17)