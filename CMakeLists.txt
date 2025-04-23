CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
project(whole_body_state_rviz_plugin)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rviz_common REQUIRED)
find_package(rviz_default_plugins REQUIRED)
find_package(rviz_rendering REQUIRED)
find_package(rviz_ogre_vendor REQUIRED)
find_package(pluginlib REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Boost REQUIRED COMPONENTS system)
find_package(whole_body_state_msgs REQUIRED)
find_package(pinocchio REQUIRED)

include_directories(src ${OGRE_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS}/Paging)

set(CMAKE_AUTOMOC ON)

# Qt5 MOC
set(MOC_HEADERS
  include/whole_body_state_rviz_plugin/PointVisual.hpp
  include/whole_body_state_rviz_plugin/LineVisual.hpp
  include/whole_body_state_rviz_plugin/ArrowVisual.hpp
  include/whole_body_state_rviz_plugin/PolygonVisual.hpp
  include/whole_body_state_rviz_plugin/ConeVisual.hpp
  include/whole_body_state_rviz_plugin/PinocchioLinkUpdater.hpp
  # include/whole_body_state_rviz_plugin/WholeBodyStateDisplay.h
  include/whole_body_state_rviz_plugin/WholeBodyTrajectoryDisplay.hpp
)

qt5_wrap_cpp(MOC_FILES ${MOC_HEADERS})

add_definitions(-DQT_NO_KEYWORDS)

# インクルードディレクトリ
include_directories(
  include
  ${Boost_INCLUDE_DIRS}
)

# ソースファイル
set(SOURCE_FILES
  src/PointVisual.cpp
  src/LineVisual.cpp
  src/ArrowVisual.cpp
  src/PolygonVisual.cpp
  src/ConeVisual.cpp
  src/PinocchioLinkUpdater.cpp
  # src/WholeBodyStateDisplay.cpp
  src/WholeBodyTrajectoryDisplay.cpp
  ${MOC_FILES}
)

# ライブラリ作成
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  whole_body_state_msgs
  rviz_common
  rviz_default_plugins
  rviz_rendering
  pluginlib
  Boost
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Widgets
  pinocchio::pinocchio
)

pluginlib_export_plugin_description_file(rviz_common plugin_description.xml)

# プラグイン記述ファイル
# install(FILES plugin_description.xml
#   DESTINATION share/${PROJECT_NAME}
# )

# ライブラリのインストール
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# パッケージのエクスポート
ament_export_dependencies(rclcpp rviz_common rviz_rendering pluginlib Boost pinocchio)
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

ament_package()