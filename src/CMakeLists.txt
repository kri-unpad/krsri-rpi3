add_executable(${PROJECT_NAME} main.cpp serial.cpp serial.h hmc5883l.cpp hmc5883l.h vector3.cpp vector3.h navigator.cpp navigator.h)

target_link_libraries(
  ${PROJECT_NAME}
  Threads::Threads
  ${wiringPi_LIBS}
)
