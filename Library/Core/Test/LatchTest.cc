/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Latch.h>
#include <TestRunner/TestRunner.h>
#include <thread>

TEST(LatchTest, SimpleLatch) {
  rl::core::Latch l(3);
  bool done[3]{false, false, false};

  std::thread one([&]() {
    std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
    done[0] = true;
    l.countDown();
  });

  std::thread two([&]() {
    std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
    done[1] = true;
    l.countDown();
  });

  std::thread three([&]() {
    std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
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
    rl::core::AutoLatch l(3);

    one = std::thread([&]() {
      std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
      done[0] = true;
      l.countDown();
    });

    two = std::thread([&]() {
      std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
      done[1] = true;
      l.countDown();
    });

    three = std::thread([&]() {
      std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
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
