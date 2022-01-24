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
        # TODO: Fix the wrapper script to avoid this hack.
        # The newline that cat adds at the end of the string
        # is annoying and makes me have to handle things more
        # generally than I would otherwise.
        ":input",
        "//lib/cexpr:list",
        "//lib/cexpr:array",
        "//lib/cexpr:valuelist",
    ]
)

cc_library(
    name = "input",
    hdrs = ["input.h"],
)
