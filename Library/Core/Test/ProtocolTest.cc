/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Channel.h>
#include <Core/Latch.h>
#include <Core/Macros.h>
#include <Core/Protocol.h>
#include <Core/Thread.h>
#include <TestRunner/TestRunner.h>
#include <thread>

class EchoProtocol : public rl::core::Protocol {
 public:
  EchoProtocol(bool isServer) : Protocol(isServer) {}

 protected:
  std::string advertisementName() const override {
    return "rl.test.protocoltest.echo";
  }

  void onRequest(rl::core::Message requestMessage,
                 std::unique_ptr<rl::core::Channel> replyChannel,
                 rl::core::ProtocolPayloadIdentifier identifier) override {
    /*
     *  The sender sends nothing
     */
    RL_ASSERT(requestMessage.readCompleted());

    fulfillRequest(identifier, std::move(replyChannel),
                   [](rl::core::Message& responseMessage) {
                     std::string echo = "echo";
                     return responseMessage.encode(echo);
                   });
  }

  bool populateRequestPayload(rl::core::Message& message) override {
    return true;
  }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(EchoProtocol);
};

TEST(Protocol, SimpleEcho) {
  auto serverTermination = rl::core::EventLoopSource::Trivial();
  serverTermination->setWakeFunction([&](rl::core::IOResult res) {
    ASSERT_EQ(res, rl::core::IOResult::Success);
    rl::core::EventLoop::Current()->terminate();
  });

  rl::core::Latch serverLatch(1);

  auto server = std::thread([&]() {
    rl::core::thread::SetName("server");
    auto loop = rl::core::EventLoop::Current();
    EchoProtocol echoServer(true);
    loop->addSource(echoServer.source());
    loop->addSource(serverTermination);
    loop->loop([&]() { serverLatch.countDown(); });
  });

  auto client = std::thread([&]() {
    rl::core::thread::SetName("client");
    auto loop = rl::core::EventLoop::Current();
    EchoProtocol echoClient(false);

    /*
     *  Wait for the server to start up.
     */
    serverLatch.wait();

    echoClient.sendRequest(
        [&](rl::core::IOResult result, rl::core::Message message) {
          ASSERT_EQ(result, rl::core::IOResult::Success);
          std::string response;
          ASSERT_TRUE(message.decode(response));
          ASSERT_EQ(response, "echo");
          serverTermination->writer()(serverTermination->handles().writeHandle);
          rl::core::EventLoop::Current()->terminate();
        });

    loop->addSource(echoClient.source());
    loop->loop();
  });

  server.join();
  client.join();
}
