set(MODULE_ID sandbox)

add_executable(${MODULE_ID}
    main.cpp)

target_link_libraries(${MODULE_ID} 
    PRIVATE
        project_warnings
        project_options
        mist)
