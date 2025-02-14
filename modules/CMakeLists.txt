# Get a list of all entries (files and directories) in this directory.
file(GLOB MODULE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

foreach(module ${MODULE_DIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${module}")
        message(STATUS "Found module directory: ${module}")
        file(GLOB_RECURSE MODULE_SOURCES
            "${CMAKE_CURRENT_SOURCE_DIR}/${module}/*.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/${module}/*.cxx"
            "${CMAKE_CURRENT_SOURCE_DIR}/${module}/*.cc")

        if(MODULE_SOURCES)
            add_library(${module} SHARED ${MODULE_SOURCES})
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${module}/include")
                target_include_directories(${module} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/${module}/include")
            endif()
            target_link_libraries(${module} PUBLIC WeevilEngine)
            set_target_properties(${module} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
            )

            message(STATUS "Added module target: ${module}")
        else()
            message(WARNING "No source files found for module: ${module}")
        endif()
    endif()
endforeach()
