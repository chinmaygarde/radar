/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Mutex.h>
#include <Core/ThreadLocal.h>
#include <Typography/TypographyContext.h>
#include <unicode/brkiter.h>
#include <unicode/udata.h>

namespace rl {
namespace type {

/*
 *  TODO: Figure out packaging situation and get rid of this hack.
 */
static constexpr const char* kICUDataFileName =
    "/Users/chinmaygarde/Dropbox/Projects/radar/ThirdParty/icu/icu/source/data/"
    "in/icudt59l.dat";

TypographyContext::TypographyContext()
    : _icuDataMapping(core::URI{kICUDataFileName}) {
  if (_icuDataMapping.size() == 0) {
    RL_LOG("Could not map ICU data file '%s' into memory.", kICUDataFileName);
    return;
  }

  UErrorCode status = U_ZERO_ERROR;
  udata_setCommonData(_icuDataMapping.mapping(), &status);
  if (U_FAILURE(status)) {
    RL_LOG("ICU error '%s' while trying to set common data.",
           u_errorName(status));
    return;
  }

  _valid = true;
}

TypographyContext& TypographyContext::SharedContext() {
  static auto leakyDeleter = [](TypographyContext*) {};
  static std::unique_ptr<TypographyContext, decltype(leakyDeleter)> gContext = {
      nullptr, leakyDeleter};
  if (gContext != nullptr) {
    return *gContext;
  }
  static core::Mutex gTypographyMutex;
  core::MutexLocker lock(gTypographyMutex);
  if (gContext != nullptr) {
    return *gContext;
  }
  gContext = {new TypographyContext(), leakyDeleter};
  return *gContext;
}

bool TypographyContext::isValid() const {
  return _valid;
}

icu::BreakIterator* TypographyContext::GetBreakIteratorForThread() {
  if (!_valid) {
    RL_LOG(
        "Could not get the ICU break iterator for the thread because the "
        "typography context for the process was not valid.");
    return nullptr;
  }
  RL_THREAD_LOCAL core::ThreadLocal tCurrentBreakIterator([](uintptr_t value) {
    delete reinterpret_cast<icu::BreakIterator*>(value);
  });
  icu::BreakIterator* iterator =
      reinterpret_cast<icu::BreakIterator*>(tCurrentBreakIterator.get());
  if (iterator != nullptr) {
    return iterator;
  }
  UErrorCode error = U_ZERO_ERROR;
  iterator =
      icu::BreakIterator::createLineInstance(icu::Locale::getDefault(), error);
  if (U_FAILURE(error)) {
    RL_LOG("Could not initialize the ICU break iterator for thread: %s",
           u_errorName(error));
    return nullptr;
  }
  tCurrentBreakIterator.set(reinterpret_cast<uintptr_t>(iterator));
  return iterator;
}

}  // namespace type
}  // namespace rl
