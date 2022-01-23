package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "tests",
    srcs = ["tests.cc"],
    deps = [
        ":array_test"
    ]
)

cc_library(
    name = "valuelist",
    hdrs = ["valuelist.h"],
    deps = []
)

cc_library(
    name = "array_test",
    hdrs = ["array_test.h"],
    deps = [
        ":array",
        ":string",
    ]
)

cc_library(
    name = "list",
    hdrs = ["list.h"],
)

cc_library(
    name = "array",
    hdrs = ["array.h"],
    deps = [
        "//lib/cexpr:list",
        "//lib/cexpr:primitives",
        "//lib/cexpr:string",
    ]
)

cc_library(
    name = "primitives",
    hdrs = ["primitives.h"],
)

cc_library(
    name = "string",
    hdrs = ["string.h"],
    deps = [
        "//lib/cexpr:math",
    ]
)

cc_library(
    name = "math",
    hdrs = ["math.h"],
)
