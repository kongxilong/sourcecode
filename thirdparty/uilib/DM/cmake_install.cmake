# Install script for directory: D:/Install/编程/开源库/DM

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/DM")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Install/编程/开源库/DM/DmMain/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/Samples/DMDemo/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/Samples/QQDemo/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/Samples/DMSpy/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/Samples/DMUnitTest/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/Samples/DMDesigner/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/Samples/TGPDemo/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/PlugIns/Plugin_Expand/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/3rdParty/zlib/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/3rdParty/gtest/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/3rdParty/lua/cmake_install.cmake")
  include("D:/Install/编程/开源库/DM/3rdParty/skia/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "D:/Install/编程/开源库/DM/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
