package(default_visibility = ["//visibility:public"])

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
)
