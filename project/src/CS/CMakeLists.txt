cmake_minimum_required(VERSION 3.5)

project(CSshow)

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH /root/loongarch/project/bin)

# 添加源文件
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/CSshow.cpp)

# 查找 Qt5 库
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

# 添加链接库
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} Qt5::Sql)

# 为目标添加编译器选项
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 安装目标
install(TARGETS ${PROJECT_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
