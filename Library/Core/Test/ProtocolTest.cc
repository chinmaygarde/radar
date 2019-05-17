/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <thread>

#include <Core/Channel.h>
#include <Core/Latch.h>
#include <Core/Macros.h>
#include <Core/Protocol.h>
#include <Core/Thread.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

class EchoProtocol : public Protocol {
 public:
  EchoProtocol(bool isServer) : Protocol(isServer) {}

 protected:
  std::string advertisementName() const override {
    return "rl.test.protocoltest.echo";
  }

  void onRequest(Message requestMessage,
                 std::unique_ptr<Channel> replyChannel,
                 ProtocolPayloadIdentifier identifier) override {
    /*
     *  The sender sends nothing
     */
    RL_ASSERT(requestMessage.readCompleted());

    fulfillRequest(identifier, std::move(replyChannel),
                   [](Message& responseMessage) {
                     std::string echo = "echo";
                     return responseMessage.encode(echo);
                   });
  }

  bool populateRequestPayload(Message& message) override { return true; }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(EchoProtocol);
};

TEST(Protocol, SimpleEcho) {
  auto serverTermination = EventLoopSource::Trivial();
  serverTermination->setWakeFunction([&](IOResult res) {
    ASSERT_EQ(res, IOResult::Success);
    EventLoop::Current()->terminate();
  });

  Latch serverLatch(1);

  auto server = std::thread([&]() {
    thread::SetName("server");
    auto loop = EventLoop::Current();
    EchoProtocol echoServer(true);
    loop->addSource(echoServer.source());
    loop->addSource(serverTermination);
    loop->loop([&]() { serverLatch.countDown(); });
  });

  auto client = std::thread([&]() {
    thread::SetName("client");
    auto loop = EventLoop::Current();
    EchoProtocol echoClient(false);

    /*
     *  Wait for the server to start up.
     */
    serverLatch.wait();

    echoClient.sendRequest([&](IOResult result, Message message) {
      ASSERT_EQ(result, IOResult::Success);
      std::string response;
      ASSERT_TRUE(message.decode(response));
      ASSERT_EQ(response, "echo");
      serverTermination->writer()(serverTermination->handles().writeHandle);
      EventLoop::Current()->terminate();
    });

    loop->addSource(echoClient.source());
    loop->loop();
  });

  server.join();
  client.join();
}

}  // namespace testing
}  // namespace core
}  // namespace rl
