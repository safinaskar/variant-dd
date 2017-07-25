cmake_minimum_required (VERSION 3.1) # 3.1 adds C_STANDARD
project (variant-dd LANGUAGES C)
add_executable (variant-dd variant-dd.c)
set_property (TARGET variant-dd PROPERTY C_STANDARD 99)
target_link_libraries (variant-dd -lsh)
install (TARGETS variant-dd DESTINATION bin)
