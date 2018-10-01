add_executable (test_crc1 "test_crc1.c")
target_link_libraries (test_crc1 ${LIBGPS_LIBRARIES})
add_test (NAME test_crc1 COMMAND test_crc1)

add_executable (test_udp1 "test_udp1.c")
target_link_libraries (test_udp1 ${LIBGPS_LIBRARIES})
add_test (NAME test_udp1 COMMAND test_udp1)

add_executable (test_udp_tll "test_udp_tll.c")
target_link_libraries (test_udp_tll ${LIBGPS_LIBRARIES})
add_test (NAME test_udp_tll COMMAND test_udp_tll)

add_executable (test_str_tll1 "test_str_tll1.c")
target_link_libraries (test_udp_tll ${LIBGPS_LIBRARIES})
add_test (NAME test_str_tll1 COMMAND test_str_tll1)

add_executable (test_str_ais1 "test_str_ais1.c")
target_link_libraries (test_str_ais1 ${LIBGPS_LIBRARIES})
add_test (NAME test_str_ais1 COMMAND test_str_ais1)


# Valgrind test:
find_program(VALGRIND "valgrind")
if(VALGRIND)
  add_custom_target(valgrind_test_crc1 COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:test_crc1>)
  add_custom_target(valgrind_test_udp1 COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:test_udp1>)
  add_custom_target(valgrind_test_udp_tll COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:test_udp_tll>)
  add_custom_target(valgrind_test_str_tll1 COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:test_str_tll1>)
endif(VALGRIND)
