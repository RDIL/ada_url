# frozen_string_literal: true

require "mkmf"

# Makes all symbols private by default to avoid unintended conflict
# with other gems. To explicitly export symbols you can use RUBY_FUNC_EXPORTED
# selectively, or entirely remove this flag.
append_cflags("-fvisibility=hidden")

if find_executable("g++")
  $CXX = "g++"
else
  raise "g++ not found. Please install a C++ compiler."
end

$CXXFLAGS += " -std=c++20"
$CXXFLAGS += " -O2"

have_header("ada_url/ada_c.h")
have_header("ada_url/ada.h")

create_makefile("ada_url/ada_url")
