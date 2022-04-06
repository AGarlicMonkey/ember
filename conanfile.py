import os

from conans import ConanFile

class CloveConan(ConanFile):
    settings = (
        "os",
        "os_build",
        "compiler",
        "build_type",
        "arch"
    )

    requires = (
        "spdlog/1.10.0",
    )

    generators = (
        "cmake_find_package_multi"
    )

    default_options = {
        "spdlog:shared": False,
    }
