set(SAMPLE_DIR "${PROJECT_SOURCE_DIR}")

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

file(GLOB_RECURSE CPP_SOURCES_BOUNDARY "${SAMPLE_DIR}/thirdparty/json/*.cpp")

set(PROJECT_SAMPLE_EXEC "sample-solar-system")
add_executable(${PROJECT_SAMPLE_EXEC}
        ${CPP_SOURCES_BOUNDARY}
        "${SAMPLE_DIR}/main.cpp"
        "${SAMPLE_DIR}/Corp.cpp")

target_include_directories(${PROJECT_SAMPLE_EXEC}
        PRIVATE "${SAMPLE_DIR}/include"
)

target_link_libraries(${PROJECT_SAMPLE_EXEC} PRIVATE ${PROJECT_CORE_LIB})
target_link_libraries(${PROJECT_SAMPLE_EXEC} PUBLIC nlohmann_json::nlohmann_json)

file(COPY "assets" DESTINATION "${PROJECT_BINARY_DIR}")

