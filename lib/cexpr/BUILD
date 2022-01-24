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
    deps = [
        ":list",
        ":math"
    ]
)

cc_library(
    name = "array_test",
    hdrs = ["array_test.h"],
    deps = [
        ":array",
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
        "//lib/cexpr:valuelist"
    ]
)

cc_library(
    name = "primitives",
    hdrs = ["primitives.h"],
)

cc_library(
    name = "math",
    hdrs = ["math.h"],
)
