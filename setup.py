import sys
import subprocess
from pathlib import Path
from setuptools.dist import Distribution
from setuptools import setup, find_packages
from setuptools.extension import Library
from setuptools.command.build_ext import build_ext as _build_ext


class CMakeExtension(Library):
    def __init__(self, name, cmake_file: str, build_dir="build", **kwargs):
        self._cmake_build_path = Path(cmake_file).parent / build_dir
        self._cmake_file = cmake_file
        super().__init__(name, [], **kwargs)


class cmake_build_ext(_build_ext):
    def __init__(self, dist: Distribution):
        cmake_ext_modules = [
            module for module in dist.ext_modules if isinstance(module, CMakeExtension)
        ]
        other_ext_modules = [
            module
            for module in dist.ext_modules
            if not isinstance(module, CMakeExtension)
        ]
        dist.ext_modules = other_ext_modules
        super().__init__(dist)
        self.cmake_ext_modules = cmake_ext_modules

    def run(self):
        super().run()
        for ext in self.cmake_ext_modules:
            self._run_cmake(ext)

    def _run_cmake(self, cmake_ext: CMakeExtension):
        if not cmake_ext._cmake_build_path.exists():
            cmake_ext._cmake_build_path.mkdir(parents=True)
        ret_code = subprocess.call(
            ["cmake", f"-DPYTHON_EXECUTABLE={sys.executable}", ".."],
            cwd=str(cmake_ext._cmake_build_path),
        )
        if ret_code > 0:
            print(f"fail to configure cmake extension: {cmake_ext.name}")
            return
        ret_code = subprocess.call(["make"], cwd=str(cmake_ext._cmake_build_path))
        if ret_code > 0:
            print(f"fail to build cmake extension: {cmake_ext.name}")


setup(
    name="pybind11_example",
    version="0.0.1",
    packages=find_packages(),
    ext_modules=[CMakeExtension("pybind11_numpy", "CMakeLists.txt")],
    package_data={"pybind11_example": ["*.so"]},
    cmdclass={
        "build_ext": cmake_build_ext,
    },
)
