set(MODULE_ID mist)

add_library(${MODULE_ID} STATIC src/Point.cpp src/Noise.cpp src/MapTools.cpp)

file(GLOB HEADER_FILES src/*.h)

target_include_directories(
    ${MODULE_ID} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src"
                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/mist")

target_link_libraries(${MODULE_ID} PRIVATE project_warnings project_options)

install(TARGETS ${MODULE_ID})
install(FILES ${HEADER_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mist/)

if (BUILD_TESTING)
    find_package(Catch2 REQUIRED)

    add_executable(utest_${MODULE_ID} test/utest_basic.cpp
                                      test/utest_observable.cpp)

    target_include_directories(utest_${MODULE_ID} PRIVATE ".")

    target_link_libraries(
        utest_${MODULE_ID}
        PRIVATE project_warnings
                project_options
                Catch2::Catch2WithMain
                ${MODULE_ID})

    include(CTest)
    include(Catch)
    catch_discover_tests(utest_${MODULE_ID} TEST_PREFIX "${MODULE_ID}@")
endif ()
