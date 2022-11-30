add_executable(
  krsri_2022
  main.cpp
  serial.cpp
  serial.h
  # hmc5883l.cpp
  # hmc5883l.h
  vector3.cpp
  vector3.h
  navigator.cpp
  navigator.h
)
target_link_libraries(
  krsri_2022
  Threads::Threads
  ${wiringPi_LIBS}
)
install(
  TARGETS krsri_2022
  RUNTIME DESTINATION bin
)
