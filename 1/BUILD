load("//lib:compile_time.bzl", "aoc_compile_time_binary")

aoc_compile_time_binary(
    name = "main",
    srcs = ["main.cc"],
    input_file = "input.txt",
    deps = [
        "//lib/cexpr:list",
        "//lib/cexpr:array",
        "//lib/cexpr:primitives",
    ],

    # We need to parse the input file which is about 9600 characters.
    # TODO: optimize this so that we can lower the depth.
    copts = ["-ftemplate-depth=10000"],
)
