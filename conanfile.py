from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout


class HelloConan(ConanFile):
    name = "mist"
    version = "0.1.8"

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
    exports_sources = "CMakeLists.txt", "*.cmake", "mist/*", "app/*"
    generators = "CMakeDeps"

    def requirements(self):
        self.requires("catch2/3.1.0")

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
        cmake.configure(variables={ "ENABLE_SANITIZERS" : "Off" })
        cmake.build(target='mist')

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["mist"]
