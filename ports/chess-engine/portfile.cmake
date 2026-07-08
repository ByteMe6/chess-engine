vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO ByteMe6/chess-engine
    REF "v${VERSION}"
    SHA512 1aae58167cf85b62d484ebea4ffda2c5cf19b126d0f43abc7c1366edad95331876a0c1e830dbd433c5ab74c210e784460f8024e189a9ece54f74ec12a5b679e5
    HEAD_REF master
)

file(INSTALL "${SOURCE_PATH}/src/chessEngine.hpp" DESTINATION "${CURRENT_PACKAGES_DIR}/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
