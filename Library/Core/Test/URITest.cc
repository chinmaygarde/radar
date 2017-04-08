/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Bundle.h>
#include <Core/URI.h>
#include <Core/Utilities.h>
#include <TestRunner/TestRunner.h>

TEST(URITest, Simple) {
  rl::core::URI uri("file://hello/world.txt");
  ASSERT_EQ(uri.toString(), "file://hello/world.txt");
}

TEST(URITest, FileSystemRepresentation) {
  rl::core::URI uri("file://hello/world.txt");
  ASSERT_EQ(uri.filesystemRepresentation(), "hello/world.txt");
}

TEST(URITest, ExecutablePath) {
  auto exePath = rl::core::GetExecutablePath();
  ASSERT_TRUE(exePath.isValid());
}

TEST(URITest, AppendToURI) {
  rl::core::URI uri("file:///one/two/three");

  rl::core::URI directory = uri.append(rl::core::URI{"../TWO"});

  ASSERT_EQ(directory.isValid(), true);
  ASSERT_EQ(directory.toString(), "file:///one/TWO");
}

TEST(URITest, NormalizeURI) {
  rl::core::URI uri("file:///one/../three");

  ASSERT_TRUE(uri.isValid());

  ASSERT_TRUE(uri.normalize());

  ASSERT_EQ(uri.toString(), "file:///three");
}

TEST(URITest, SimpleBundle) {
  rl::core::Bundle bundle;

  auto uri = bundle.uriForResource(rl::core::URI{"file://hello.txt"});
  ASSERT_TRUE(uri.isValid());
}
