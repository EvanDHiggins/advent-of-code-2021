
def inline_as_array(name, input_file):
  native.genrule(
    name = name,
    srcs = [input_file],
    outs = [name + ".h"],
    cmd = "\n".join([
        "echo \"#pragma once\n\n\" > $@",
        "echo \"constexpr const char PROGRAM_INPUT[] = \" >> $@",
        "echo -n \"R\"'\"'\"(\" >> $@",
        "cat $(location %s) >> $@" % input_file,
        "echo \")\"'\"'\";\" >> $@"])
  )

# Creates a C++ binary with access to a header file containing the contents
# of "input_file".
#
# C++ files in the binary can include "path/to/__generated_input.h" which
# contains a single global constexpr value called "PROGRAM_INPUT".
def aoc_compile_time_binary(name, srcs, input_file, **params):
    inline_as_array(
        name = "__generated_input",
        input_file = input_file,
    )
    native.cc_binary(
        name = name,
        srcs = srcs + [":__generated_input.h"],
        **params
    )
