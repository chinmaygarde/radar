/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <thread>

#include <Core/Latch.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(LatchTest, SimpleLatch) {
  Latch l(3);
  bool done[3]{false, false, false};

  std::thread one([&]() {
    std::this_thread::sleep_for(ClockDurationMicro(20));
    done[0] = true;
    l.countDown();
  });

  std::thread two([&]() {
    std::this_thread::sleep_for(ClockDurationMicro(20));
    done[1] = true;
    l.countDown();
  });

  std::thread three([&]() {
    std::this_thread::sleep_for(ClockDurationMicro(20));
    done[2] = true;
    l.countDown();
  });

  l.wait();

  ASSERT_TRUE(done[0] == true);
  ASSERT_TRUE(done[1] == true);
  ASSERT_TRUE(done[2] == true);

  one.join();
  two.join();
  three.join();
}

TEST(LatchTest, AutoLatch) {
  std::thread one, two, three;
  bool done[] = {false, false, false};
  {
    AutoLatch l(3);

    one = std::thread([&]() {
      std::this_thread::sleep_for(ClockDurationMicro(20));
      done[0] = true;
      l.countDown();
    });

    two = std::thread([&]() {
      std::this_thread::sleep_for(ClockDurationMicro(20));
      done[1] = true;
      l.countDown();
    });

    three = std::thread([&]() {
      std::this_thread::sleep_for(ClockDurationMicro(20));
      done[2] = true;
      l.countDown();
    });
  }

  ASSERT_TRUE(done[0] == true);
  ASSERT_TRUE(done[1] == true);
  ASSERT_TRUE(done[2] == true);

  one.join();
  two.join();
  three.join();
}

}  // namespace testing
}  // namespace core
}  // namespace rl
