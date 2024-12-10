set(MY_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/Includes")

add_executable(VectorSum)
target_include_directories(VectorSum PUBLIC "${MY_INCLUDES}")
target_sources(VectorSum PRIVATE VectorSum.cpp)

add_executable(ProduceConsume)
target_include_directories(ProduceConsume PUBLIC "${MY_INCLUDES}")
target_sources(ProduceConsume PRIVATE ProduceConsume.cpp)

add_executable(MatrixVectorMult)
target_include_directories(MatrixVectorMult PUBLIC "${MY_INCLUDES}")
target_sources(MatrixVectorMult PRIVATE MatrixVectorMult.cpp)

add_executable(ThreadPool)
target_include_directories(ThreadPool PUBLIC "${MY_INCLUDES}")
target_sources(ThreadPool PRIVATE ThreadPool.cpp)
