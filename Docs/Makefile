# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

help:
	@echo "Available targets:"
	@echo "\tall: Performs a full build after a clean of all the documentation"
	@echo "\tclean: Cleans previous build artifacts"
	@echo "\tbuild: Builds the HTML documentation and docset"

all: clean build
	@echo "Full Build Done"

build:
	@doxygen
	@(cd html && $(MAKE))

clean:
	@echo Cleaning...
	@rm -rf html/
