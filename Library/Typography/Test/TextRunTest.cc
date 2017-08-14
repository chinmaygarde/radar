/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/AttributedStringBuilder.h>
#include <Typography/TextRun.h>

TEST(TextRunTest, SimpleRunsCanBeConstructed) {
  rl::type::AttributedStringBuilder builder;
  std::string hello(
      "World ציור עסקים מדע מה. צ'ט בקלות הבאים  Hello מאמרשיחהצפה של");
  builder.appendText(hello);
  auto attributedHello = builder.attributedString();

  rl::type::TextRuns runs(attributedHello);
  ASSERT_EQ(runs.runs().size(), 4u);
}

TEST(TextRunTest, RunsSplitOnAttributedString) {
  rl::type::AttributedStringBuilder builder;
  std::string hello(
      "World ציור עסקים מדע מה. צ'ט בקלות הבאים  Hello מאמרשיחהצפה של");
  builder.appendText(hello);
  auto attributedHello = builder.attributedString();
  ASSERT_EQ(attributedHello.string().size(), 62u);
  ASSERT_EQ(attributedHello.string().lengthOfCharacters(), 62u);

  rl::type::TextRuns textRuns(attributedHello);
  ASSERT_TRUE(textRuns.isValid());
  ASSERT_EQ(textRuns.runs().size(), 4u);

  /*
   *  Setup break opportunities at each character. There are no characters in
   *  this buffer that take more than one 16 byte point.
   */

  std::vector<size_t> breaks;
  for (size_t i = 0, end = attributedHello.string().size(); i < end; i++) {
    breaks.emplace_back(i);
  }

  auto splitRuns = textRuns.splitAtBreaks(breaks);
  ASSERT_TRUE(splitRuns.isValid());
  /*
   *  Zero sized runs are not present in the results.
   */
  ASSERT_EQ(splitRuns.runs().size(), 62u);
  /*
   *  All run starts should be increasing by 1 from 0. All run lengths should be
   *  1.
   */
  size_t current = 0;
  for (const auto& run : splitRuns.runs()) {
    ASSERT_EQ(run.range().start, current++);
    ASSERT_EQ(run.range().length, 1u);
  }
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplitNoRuns) {
  std::vector<rl::type::TextRun> runs;
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({50});
  ASSERT_EQ(splitRuns.runs().size(), 0);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplitNoBreaks) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 100}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({});
  ASSERT_EQ(splitRuns.runs().size(), 1);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplitNoRunsOrBreaks) {
  std::vector<rl::type::TextRun> runs;
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({});
  ASSERT_EQ(splitRuns.runs().size(), 0);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit1) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 100}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({50});
  ASSERT_EQ(splitRuns.runs().size(), 2);
  rl::type::TextRange expected(0, 50);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected);
  rl::type::TextRange expected2(50, 50);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected2);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit2) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 50}});
  runs.emplace_back(rl::type::TextRun{direction, {50, 50}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({75});
  ASSERT_EQ(splitRuns.runs().size(), 3);
  rl::type::TextRange expected(0, 50);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected);
  rl::type::TextRange expected2(50, 25);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected2);
  rl::type::TextRange expected3(75, 25);
  ASSERT_EQ(splitRuns.runs()[2].range(), expected3);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit2Direction) {
  std::vector<rl::type::TextRun> runs;
  runs.emplace_back(
      rl::type::TextRun{rl::type::TextRun::Direction::LeftToRight, {0, 50}});
  runs.emplace_back(
      rl::type::TextRun{rl::type::TextRun::Direction::RightToLeft, {50, 50}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({75});
  ASSERT_EQ(splitRuns.runs().size(), 3);
  rl::type::TextRange expected(0, 50);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected);
  ASSERT_EQ(splitRuns.runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  rl::type::TextRange expected2(50, 25);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected2);
  ASSERT_EQ(splitRuns.runs()[1].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  rl::type::TextRange expected3(75, 25);
  ASSERT_EQ(splitRuns.runs()[2].range(), expected3);
  ASSERT_EQ(splitRuns.runs()[2].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit3) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 50}});
  runs.emplace_back(rl::type::TextRun{direction, {50, 50}});
  runs.emplace_back(rl::type::TextRun{direction, {100, 50}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({75});
  ASSERT_EQ(splitRuns.runs().size(), 4);
  rl::type::TextRange expected(0, 50);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected);
  rl::type::TextRange expected2(50, 25);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected2);
  rl::type::TextRange expected3(75, 25);
  ASSERT_EQ(splitRuns.runs()[2].range(), expected3);
  rl::type::TextRange expected4(100, 50);
  ASSERT_EQ(splitRuns.runs()[3].range(), expected4);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit4) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {25, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {50, 50}});
  runs.emplace_back(rl::type::TextRun{direction, {100, 50}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({75});
  ASSERT_EQ(splitRuns.runs().size(), 5);
  rl::type::TextRange expected0(0, 25);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected0);
  rl::type::TextRange expected1(25, 25);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected1);
  rl::type::TextRange expected2(50, 25);
  ASSERT_EQ(splitRuns.runs()[2].range(), expected2);
  rl::type::TextRange expected3(75, 25);
  ASSERT_EQ(splitRuns.runs()[3].range(), expected3);
  rl::type::TextRange expected4(100, 50);
  ASSERT_EQ(splitRuns.runs()[4].range(), expected4);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit5) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {25, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {50, 50}});
  runs.emplace_back(rl::type::TextRun{direction, {100, 50}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({75, 125});
  ASSERT_EQ(splitRuns.runs().size(), 6);
  rl::type::TextRange expected0(0, 25);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected0);
  rl::type::TextRange expected1(25, 25);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected1);
  rl::type::TextRange expected2(50, 25);
  ASSERT_EQ(splitRuns.runs()[2].range(), expected2);
  rl::type::TextRange expected3(75, 25);
  ASSERT_EQ(splitRuns.runs()[3].range(), expected3);
  rl::type::TextRange expected4(100, 25);
  ASSERT_EQ(splitRuns.runs()[4].range(), expected4);
  rl::type::TextRange expected5(125, 25);
  ASSERT_EQ(splitRuns.runs()[5].range(), expected5);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit6) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {25, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {50, 50}});
  runs.emplace_back(rl::type::TextRun{direction, {100, 50}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({75, 80, 125});
  ASSERT_EQ(splitRuns.runs().size(), 7);
  rl::type::TextRange expected0(0, 25);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected0);
  rl::type::TextRange expected1(25, 25);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected1);
  rl::type::TextRange expected2(50, 25);
  ASSERT_EQ(splitRuns.runs()[2].range(), expected2);
  rl::type::TextRange expected3(75, 5);
  ASSERT_EQ(splitRuns.runs()[3].range(), expected3);
  rl::type::TextRange expected4(80, 20);
  ASSERT_EQ(splitRuns.runs()[4].range(), expected4);
  rl::type::TextRange expected5(100, 25);
  ASSERT_EQ(splitRuns.runs()[5].range(), expected5);
  rl::type::TextRange expected6(125, 25);
  ASSERT_EQ(splitRuns.runs()[6].range(), expected6);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}

TEST(TextRunTest, SimpleSplit6RepeatsDontCount) {
  std::vector<rl::type::TextRun> runs;
  auto direction = rl::type::TextRun::Direction::LeftToRight;
  runs.emplace_back(rl::type::TextRun{direction, {0, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {25, 25}});
  runs.emplace_back(rl::type::TextRun{direction, {50, 50}});
  runs.emplace_back(rl::type::TextRun{direction, {100, 50}});
  rl::type::TextRuns textRuns(std::move(runs));
  auto splitRuns = textRuns.splitAtBreaks({75, 80, 80, 125});
  ASSERT_EQ(splitRuns.runs().size(), 7);
  rl::type::TextRange expected0(0, 25);
  ASSERT_EQ(splitRuns.runs()[0].range(), expected0);
  rl::type::TextRange expected1(25, 25);
  ASSERT_EQ(splitRuns.runs()[1].range(), expected1);
  rl::type::TextRange expected2(50, 25);
  ASSERT_EQ(splitRuns.runs()[2].range(), expected2);
  rl::type::TextRange expected3(75, 5);
  ASSERT_EQ(splitRuns.runs()[3].range(), expected3);
  rl::type::TextRange expected4(80, 20);
  ASSERT_EQ(splitRuns.runs()[4].range(), expected4);
  rl::type::TextRange expected5(100, 25);
  ASSERT_EQ(splitRuns.runs()[5].range(), expected5);
  rl::type::TextRange expected6(125, 25);
  ASSERT_EQ(splitRuns.runs()[6].range(), expected6);
  ASSERT_EQ(textRuns.totalLength(), splitRuns.totalLength());
}
