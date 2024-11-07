set(MY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/Includes")

add_executable(VectorSum)
target_include_directories(VectorSum PUBLIC "${MY_INCLUDES}")
target_sources(VectorSum PRIVATE VectorSum.cpp)

add_executable(ProduceConsume)
target_include_directories(ProduceConsume PUBLIC "${MY_INCLUDES}")
target_sources(ProduceConsume PRIVATE ProduceConsume.cpp)
