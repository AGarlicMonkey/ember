import os

from conans import ConanFile

class EmberConan(ConanFile):
    settings = (
        "os",
        "os_build",
        "compiler",
        "build_type",
        "arch"
    )

    requires = (
        "gtest/cci.20210126",
        "shaderc/2021.1",
        "spdlog/1.10.0",
    )

    generators = (
        "cmake_find_package_multi"
    )

    default_options = {
        "shaderc:shared": True,
        "spdlog:shared": False,
    }
