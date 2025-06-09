cmake_minimum_required(VERSION 3.10)
project(optimizations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPT_LEVELS "O0" "O1" "O2" "O3")

add_executable(optimization_test main.cpp)

foreach(OPT_LEVEL ${OPT_LEVELS})
    add_test(
        NAME test_${OPT_LEVEL}
        COMMAND ${CMAKE_COMMAND}
                -DCMAKE_CXX_FLAGS=-${OPT_LEVEL}
                -DCMAKE_BUILD_TYPE=Release
                -S ${CMAKE_SOURCE_DIR}
                -B ${CMAKE_BINARY_DIR}/test_${OPT_LEVEL}
                --build ${CMAKE_BINARY_DIR}/test_${OPT_LEVEL}
                --target optimization_test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    add_test(
        NAME run_${OPT_LEVEL}
        COMMAND ${CMAKE_BINARY_DIR}/test_${OPT_LEVEL}/optimization_test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    set_tests_properties(run_${OPT_LEVEL} PROPERTIES DEPENDS test_${OPT_LEVEL})
endforeach()

enable_testing()

if(UNIX AND NOT APPLE)
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