# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


SET(CPACK_BINARY_7Z "")
SET(CPACK_BINARY_BUNDLE "")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "OFF")
SET(CPACK_BINARY_DRAGNDROP "")
SET(CPACK_BINARY_IFW "OFF")
SET(CPACK_BINARY_NSIS "OFF")
SET(CPACK_BINARY_OSXX11 "")
SET(CPACK_BINARY_PACKAGEMAKER "")
SET(CPACK_BINARY_RPM "OFF")
SET(CPACK_BINARY_STGZ "ON")
SET(CPACK_BINARY_TBZ2 "OFF")
SET(CPACK_BINARY_TGZ "ON")
SET(CPACK_BINARY_TXZ "OFF")
SET(CPACK_BINARY_TZ "ON")
SET(CPACK_BINARY_WIX "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_GENERATOR "TBZ2;TGZ;TXZ;TZ")
SET(CPACK_IGNORE_FILES "/home0/ruth/software_engineering/dune/dune-PFASST/build-cmake;\\.svn;\\.git;.*/*\\.gitignore")
SET(CPACK_INSTALLED_DIRECTORIES "/home0/ruth/software_engineering/dune/dune-PFASST;/")
SET(CPACK_INSTALL_CMAKE_PROJECTS "")
SET(CPACK_INSTALL_PREFIX "/home/zam/ruth/software_engineering/dune_installation")
SET(CPACK_MODULE_PATH "/home0/ruth/software_engineering/dune/dune-PFASST/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-fufem/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-functions/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-matrix-vector/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-localfunctions/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-grid/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-typetree/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-istl/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-geometry/cmake/modules;/home/zam/ruth/software_engineering/dune/dune-common/cmake/modules")
SET(CPACK_NSIS_DISPLAY_NAME "dune-PFASST 1.0.0")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "dune-PFASST 1.0.0")
SET(CPACK_OUTPUT_CONFIG_FILE "/home0/ruth/software_engineering/dune/dune-PFASST/build-cmake/CPackConfig.cmake")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "dune-PFASST built using CMake")
SET(CPACK_PACKAGE_FILE_NAME "dune-PFASST-1.0.0")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "dune-PFASST 1.0.0")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "dune-PFASST 1.0.0")
SET(CPACK_PACKAGE_NAME "dune-PFASST")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "Humanity")
SET(CPACK_PACKAGE_VERSION "1.0.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_RESOURCE_FILE_LICENSE "/usr/share/cmake/Templates/CPack.GenericLicense.txt")
SET(CPACK_RESOURCE_FILE_README "/usr/share/cmake/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/usr/share/cmake/Templates/CPack.GenericWelcome.txt")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_7Z "")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TBZ2;TGZ;TXZ;TZ")
SET(CPACK_SOURCE_IGNORE_FILES "/home0/ruth/software_engineering/dune/dune-PFASST/build-cmake;\\.svn;\\.git;.*/*\\.gitignore")
SET(CPACK_SOURCE_INSTALLED_DIRECTORIES "/home0/ruth/software_engineering/dune/dune-PFASST;/")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/home0/ruth/software_engineering/dune/dune-PFASST/build-cmake/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "dune-PFASST-1.0.0")
SET(CPACK_SOURCE_TBZ2 "ON")
SET(CPACK_SOURCE_TGZ "ON")
SET(CPACK_SOURCE_TOPLEVEL_TAG "Linux-Source")
SET(CPACK_SOURCE_TXZ "ON")
SET(CPACK_SOURCE_TZ "ON")
SET(CPACK_SOURCE_ZIP "OFF")
SET(CPACK_STRIP_FILES "")
SET(CPACK_SYSTEM_NAME "Linux")
SET(CPACK_TOPLEVEL_TAG "Linux-Source")
SET(CPACK_WIX_SIZEOF_VOID_P "8")

if(NOT CPACK_PROPERTIES_FILE)
  set(CPACK_PROPERTIES_FILE "/home0/ruth/software_engineering/dune/dune-PFASST/build-cmake/CPackProperties.cmake")
endif()

if(EXISTS ${CPACK_PROPERTIES_FILE})
  include(${CPACK_PROPERTIES_FILE})
endif()
