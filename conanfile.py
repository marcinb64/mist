from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class HelloConan(ConanFile):
    name = "mist"
    version = "0.1.7"

    # Optional metadata
    license = "MIT"
    author = "marcinb64@gmail.com"
    description = "Gamedev utilities"
    topics = ("utility")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "CompilerWarnings.cmake", "StaticAnalysis.cmake", "mist/*", "app/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={ "BUILD_TESTING" : "Off", "ENABLE_SANITIZERS" : "Off" })
        cmake.build(target='mist')

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["mist"]
