cmake_minimum_required(VERSION 3.10)
project(optimizations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define optimization levels to test
set(OPT_LEVELS "O0" "O1" "O2" "O3")

# Create a single executable
add_executable(optimization_test main.cpp)

# Simple test that runs the main executable
enable_testing()
add_test(NAME basic_test COMMAND optimization_test)

# Platform-specific disassembly analysis
if(UNIX AND NOT APPLE)
    # Linux with objdump
    add_custom_target(test_all_optimizations
        COMMENT "Testing all optimization levels with objdump (Linux)"
    )
    
    foreach(OPT_LEVEL ${OPT_LEVELS})
        add_custom_command(
            TARGET test_all_optimizations
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "=== Testing -${OPT_LEVEL} ==="
            COMMAND ${CMAKE_CXX_COMPILER} -${OPT_LEVEL} -std=c++17 ${CMAKE_SOURCE_DIR}/main.cpp -o ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}
            COMMAND ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}
            COMMAND objdump -d ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL} | head -30
            COMMAND stat -c "Size: %s bytes" ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}
        )
    endforeach()
    
elseif(WIN32)
    # Windows with dumpbin (MSVC) or objdump (MinGW)
    add_custom_target(test_all_optimizations
        COMMENT "Testing all optimization levels with disassembly (Windows)"
    )
    
    foreach(OPT_LEVEL ${OPT_LEVELS})
        add_custom_command(
            TARGET test_all_optimizations
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "=== Testing /${OPT_LEVEL} ==="
            COMMAND ${CMAKE_CXX_COMPILER} /${OPT_LEVEL} /std:c++17 ${CMAKE_SOURCE_DIR}/main.cpp /Fe:${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}.exe
            COMMAND ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}.exe
            COMMAND ${CMAKE_COMMAND} -E echo "Disassembly:"
            COMMAND cmd /c "dumpbin /disasm ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}.exe 2>nul | head -30 || objdump -d ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}.exe | head -30"
            COMMAND powershell -Command "(Get-Item '${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}.exe').Length" | findstr /R "^[0-9]" > temp_size.txt && set /p SIZE=<temp_size.txt && echo Size: %SIZE% bytes
        )
    endforeach()
    
else()
    # Other platforms (macOS, etc.)
    add_custom_target(test_all_optimizations
        COMMENT "Testing all optimization levels (no disassembly)"
    )
    
    foreach(OPT_LEVEL ${OPT_LEVELS})
        add_custom_command(
            TARGET test_all_optimizations
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "=== Testing -${OPT_LEVEL} ==="
            COMMAND ${CMAKE_CXX_COMPILER} -${OPT_LEVEL} -std=c++17 ${CMAKE_SOURCE_DIR}/main.cpp -o ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}
            COMMAND ${CMAKE_BINARY_DIR}/temp_${OPT_LEVEL}
            COMMAND ${CMAKE_COMMAND} -E echo "Disassembly tools not available on this platform"
        )
    endforeach()
endif()