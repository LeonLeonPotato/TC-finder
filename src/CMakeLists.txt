file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_executable(tc-finder "${SOURCES}")
target_compile_features(tc-finder PRIVATE cxx_std_23)

set(Boost_NO_WARN_NEW_VERSIONS ON) # no more errors wow!
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.84.0 REQUIRED COMPONENTS program_options)

if(Boost_FOUND)
    target_include_directories(tc-finder PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(tc-finder PRIVATE ${Boost_LIBRARIES})
endif()