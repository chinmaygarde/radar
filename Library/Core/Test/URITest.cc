/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Bundle.h>
#include <Core/URI.h>
#include <Core/Utilities.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(URITest, Simple) {
  URI uri("file://hello/world.txt");
  ASSERT_EQ(uri.toString(), "file://hello/world.txt");
}

TEST(URITest, FileSystemRepresentation) {
  URI uri("file://hello/world.txt");
  ASSERT_EQ(uri.filesystemRepresentation(), "hello/world.txt");
}

TEST(URITest, ExecutablePath) {
  auto exePath = GetExecutablePath();
  ASSERT_TRUE(exePath.isValid());
}

TEST(URITest, AppendToURI) {
  URI uri("file:///one/two/three");

  URI directory = uri.append(URI{"../TWO"});

  ASSERT_EQ(directory.isValid(), true);
  ASSERT_EQ(directory.toString(), "file:///one/TWO");
}

TEST(URITest, NormalizeURI) {
  URI uri("file:///one/../three");

  ASSERT_TRUE(uri.isValid());

  ASSERT_TRUE(uri.normalize());

  ASSERT_EQ(uri.toString(), "file:///three");
}

TEST(URITest, SimpleBundle) {
  Bundle bundle;

  auto uri = bundle.uriForResource(URI{"file://hello.txt"});
  ASSERT_TRUE(uri.isValid());
}

}  // namespace testing
}  // namespace core
}  // namespace rl
