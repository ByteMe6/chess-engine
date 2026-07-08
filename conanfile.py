import os

from conan import ConanFile
from conan.tools.files import copy


class ChessEngineConan(ConanFile):
    name = "chess-engine"
    version = "1.0.0"
    license = "GPL-3.0-only"
    url = "https://github.com/ByteMe6/chess-engine"
    homepage = "https://github.com/ByteMe6/chess-engine"
    description = "Minimal chess engine in C++17 - move generation, validation and ASCII board rendering. STL only."
    topics = ("chess", "chess-engine", "board-game", "header-only")
    package_type = "header-library"
    settings = "os", "arch", "compiler", "build_type"
    no_copy_source = True
    exports_sources = "src/*", "LICENSE"

    def package(self):
        copy(self, "chessEngine.hpp", os.path.join(self.source_folder, "src"), os.path.join(self.package_folder, "include"))
        copy(self, "LICENSE", self.source_folder, os.path.join(self.package_folder, "licenses"))

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

    def package_id(self):
        self.info.clear()
