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

  rl::type::TextRuns runs(attributedHello);
  ASSERT_TRUE(runs.isValid());
  ASSERT_EQ(runs.runs().size(), 4u);

  /*
   *  Setup break opportunities at each character. There are no characters in
   *  this buffer that take more than one 16 byte point.
   */

  std::vector<size_t> breaks;
  for (size_t i = 0, end = attributedHello.string().size(); i < end; i++) {
    breaks.emplace_back(i);
  }

  auto breakRuns = runs.splitAtBreaks(breaks);
  ASSERT_TRUE(breakRuns.isValid());
  /*
   *  Zero sized runs are not present in the results.
   */
  ASSERT_EQ(breakRuns.runs().size(), 61u);
  /*
   *  All run starts should be increasing by 1 from 0. All run lengths should be
   *  1.
   */
  size_t current = 0;
  for (const auto& run : breakRuns.runs()) {
    ASSERT_EQ(run.range().start, current++);
    ASSERT_EQ(run.range().length, 1u);
  }
}
