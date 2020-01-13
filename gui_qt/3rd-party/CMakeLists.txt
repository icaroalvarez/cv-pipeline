if (NOT TARGET easylogging)
    message("Using easylogging")
    add_library(easylogging
            easyloggingpp/src/easylogging++.cc easyloggingpp/src/easylogging++.h)
    target_include_directories(easylogging PUBLIC easyloggingpp/src)
    target_compile_options(easylogging PUBLIC "-DELPP_THREAD_SAFE")
    set_target_properties(easylogging PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)
endif ()