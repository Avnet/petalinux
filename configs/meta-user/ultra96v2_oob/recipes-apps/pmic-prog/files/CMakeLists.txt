cmake_minimum_required (VERSION 2.6)

project (pmic_prog)

# Source files to be used in the library
set(executable_SOURCES
    main.c
    i2c-comm.c
    irps5401.c
    utils.c
    ir3806x.c
)

add_executable(${PROJECT_NAME} ${executable_SOURCES})

# Add libi2c dependency
target_link_libraries(pmic_prog LINK_PUBLIC i2c )

