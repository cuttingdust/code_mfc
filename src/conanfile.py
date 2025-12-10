from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy, patch

import os

class MFCConan(ConanFile):
    # name = "code_mfc"  
    # version = "1.0.0"
    # license = "MIT"
    # author = "Your Name <email@example.com>"
    # url = "https://github.com/your/project"
    # description = "A project using the latest OpenCV."
    # topics = ("opencv", "computer-vision")

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    # 1. 声明要导出补丁文件
    # exports_sources = "fix_libpng_find.patch"
    
    # # 1. 定义你自己的项目选项
    # options = {
        # "shared": [True, False],
    # }
    
    # default_options = {
        # "shared": False,
    # }
    
    def layout(self):
        # 声明项目布局
        cmake_layout(self)

    def requirements(self):
        # 声明依赖最新稳定版 OpenCV
        # 注意：opencv/4.12.0 是截至2024年6月的最新版本
        # 你可以通过 `conan search opencv --remote=conancenter` 确认最新版本号
        self.requires("opencv/4.12.0")