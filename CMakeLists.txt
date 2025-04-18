cmake_minimum_required(VERSION 3.8)

project(GreeterService C CXX)
include("${CMAKE_CURRENT_SOURCE_DIR}/third_party/grpc/examples/cpp/cmake/common.cmake")

# Proto file
get_filename_component(hw_proto "greeterservice.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/greeterservice.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/greeterservice.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/greeterservice.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/greeterservice.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# greeterservice_grpc_proto
add_library(greeterservice_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(greeterservice_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Targets greeter_[async_](client|server)
foreach(_target client server)
  add_executable(${_target} "${_target}.cpp")
  target_link_libraries(${_target}
    greeterservice_grpc_proto
    absl::flags
    absl::flags_parse
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()
