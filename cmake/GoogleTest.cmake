if (NOT DEFINED GOOGLE_TEST_INCLUDED)
    set(GOOGLE_TEST_INCLUDED TRUE)
    include(ExternalProject)
    
    if (NOT TARGET GoogleTest)
        if (NOT DEFINED ExternalPrefixDir)
            set(ExternalPrefixDir "${PROJECT_BINARY_DIR}/extern")
        endif ()
        set(GMOCK_CXX_FLAGS "${CMAKE_CXX_COMPILER_ARG1} ${CMAKE_CXX_FLAGS} -D_REENTRANT")
        ExternalProject_Add(GoogleTest
            PREFIX ${ExternalPrefixDir}
            GIT_REPOSITORY "https://github.com/google/googletest.git"
            GIT_TAG "release-1.8.0"
            CONFIGURE_COMMAND ${CMAKE_COMMAND}
                -DCMAKE_CXX_FLAGS=${GMOCK_CXX_FLAGS} -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                "<SOURCE_DIR>/googlemock"
            BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -j
            INSTALL_COMMAND sh -c "install -d <INSTALL_DIR>/lib"
                    COMMAND sh -c "install --compare --mode=644 $(find <BINARY_DIR> -name 'lib*.so') <INSTALL_DIR>/lib/"
            LOG_DOWNLOAD 1
            LOG_UPDATE 0
            LOG_CONFIGURE 1
            LOG_BUILD 1
            LOG_TEST 0
            LOG_INSTALL 1
        )
    endif()
    
    ExternalProject_Get_Property(GoogleTest SOURCE_DIR INSTALL_DIR)
    set(GoogleTestIncludes "${SOURCE_DIR}/googletest/include" "${SOURCE_DIR}/googlemock/include")
    set(GoogleTestLibraries "${INSTALL_DIR}/lib/libgmock_main.so")
    include_directories(${GoogleTestIncludes})
    
    message(STATUS "GTest include: " ${GoogleTestIncludes})
    message(STATUS "GTest library: " ${GoogleTestLibraries})
endif()
