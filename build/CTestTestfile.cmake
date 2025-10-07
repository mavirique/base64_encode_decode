# CMake generated Testfile for 
# Source directory: /home/user/cpp/base64_encode_decode
# Build directory: /home/user/cpp/base64_encode_decode/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[Base64Tests]=] "/home/user/cpp/base64_encode_decode/build/base64_tests")
set_tests_properties([=[Base64Tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/user/cpp/base64_encode_decode/CMakeLists.txt;50;add_test;/home/user/cpp/base64_encode_decode/CMakeLists.txt;0;")
subdirs("_deps/catch2-build")
