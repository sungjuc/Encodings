add_library(utils
  memory_manager.cc)

install(TARGETS utils
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)