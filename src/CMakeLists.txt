cmake_minimum_required(VERSION 3.12)

file(GLOB_RECURSE SOURCE_FILES *.hpp *.cpp)

find_package(cpp-jwt REQUIRED)


add_executable(${PROJECT_NAME} 
        "handlers/signin/login_handler.hpp"
        "handlers/signup/registration_handler.hpp"
        "repository/repository.hpp"
        "main.cpp"
)
target_link_libraries(${PROJECT_NAME} PUBLIC userver-core userver-postgresql cpp-jwt::cpp-jwt bcrypt)

target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)