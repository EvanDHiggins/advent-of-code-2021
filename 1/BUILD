load("//lib:compile_time.bzl", "aoc_compile_time_binary")

aoc_compile_time_binary(
    name = "main",
    srcs = ["main.cc"],
    input_file = "input.txt",
    deps = [
        ":lib",
    ],

    # We need to parse the input file which is about 9600 characters.
    # TODO: optimize this so that we can lower the depth.
    copts = ["-ftemplate-depth=10000"],
)

cc_binary(
    name = "test",
    srcs = ["lib_test.cc"],
    deps = [
        ":lib",
    ]
)

cc_library(
    name = "lib",
    hdrs = ["lib.h"],
    deps = [
        "//lib/cexpr:list",
        "//lib/cexpr:array",
        "//lib/cexpr:primitives",
        "//lib/cexpr:math",
        "//lib/cexpr:valuelist",
    ]
)
