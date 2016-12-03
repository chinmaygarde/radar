
all: clean
	@flex --noline SVGPathLexer.l
	@bison SVGPathParser.y
	@echo All done..
	@clang-format -i location.hh
	@clang-format -i stack.hh
	@clang-format -i position.hh
	@clang-format -i SVGPathParser.h
	@clang-format -i SVGPathParser.cc
	@clang-format -i SVGPathLexer.h
	@clang-format -i SVGPathLexer.cc

clean:
	@rm -f location.hh
	@rm -f stack.hh
	@rm -f position.hh
	@rm -f SVGPathParser.h
	@rm -f SVGPathParser.cc
	@rm -f SVGPathLexer.h
	@rm -f SVGPathLexer.cc
