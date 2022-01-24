load("//lib:compile_time.bzl", "aoc_compile_time_binary")

aoc_compile_time_binary(
    name = "main",
    srcs = ["main.cc"],
    input_file = "input.txt",
    deps = [
        ":lib",
    ],
    copts = ["-ftemplate-depth=10000"],
)

cc_library(
    name = "lib",
    hdrs = ["lib.h"],
    deps = [
        "//lib/cexpr:array",
        "//lib/cexpr:list",
        "//lib/cexpr:math",
        "//lib/cexpr:valuelist",
    ]
)

cc_binary(
    name = "test",
    srcs = ["lib_test.cc"],
    deps = [
        ":lib",
    ],
    copts = ["-ftemplate-depth=10000"],
)
