# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "E:/Espressif_5.4.1/Espressif/frameworks/esp-idf-v5.4.1/components/bootloader/subproject")
  file(MAKE_DIRECTORY "E:/Espressif_5.4.1/Espressif/frameworks/esp-idf-v5.4.1/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader"
  "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader-prefix"
  "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader-prefix/tmp"
  "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader-prefix/src/bootloader-stamp"
  "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader-prefix/src"
  "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/fiverr/lenergy2023/LCD_TOF/lenergy5.4.1/Working_Demo/ESP32-S3-Touch-LCD1/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
