cmake_minimum_required (VERSION 2.6)
SET(LINK_FLAGS "-lpthread")

project (lotto)
add_executable(lotto ./lotto.c) 
target_link_libraries(lotto pthread)
