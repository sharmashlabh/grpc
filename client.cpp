#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "greeterservice.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using greeter::GreeterService;
using greeter::HelloRequest;
using greeter::HelloReply;

class GreeterClient {
public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(GreeterService::NewStub(channel)) {}

  std::string SayHello(const std::string& user) {
    HelloRequest request;
    request.set_name(user);

    HelloReply reply;
    ClientContext context;

    Status status = stub_->SayHello(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cerr << "RPC failed: " << status.error_message() << std::endl;
      return "RPC failed";
    }
  }

private:
  std::unique_ptr<GreeterService::Stub> stub_;
};

int main() {
  GreeterClient greeter(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  std::string reply = greeter.SayHello("Shlabh");
  std::cout << "Greeter received: " << reply << std::endl;
  return 0;
}
