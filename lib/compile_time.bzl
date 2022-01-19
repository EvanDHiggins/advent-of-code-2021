def inline_as_header(name, input_file):
  native.genrule(
    name = name,
    srcs = [input_file],
    outs = [name + ".h"],
    cmd = "\n".join([
        "echo \"#pragma once\n\n\" > $@",
        "echo \"constexpr const char* PROGRAM_INPUT = R\"'\"'\"(\" >> $@",
        "cat $(location %s) >> $@" % input_file,
        "echo \")\"'\"'\";\" >> $@"])
  )

def aoc_compile_time_binary(name, srcs):
    inline_as_header(
        name = "__generated_input",
        input_file = "input.txt",
    )
    native.cc_binary(
        name = name,
        srcs = srcs + [":__generated_input.h"]
    )
