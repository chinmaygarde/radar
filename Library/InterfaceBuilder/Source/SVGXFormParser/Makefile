
all: clean
	@flex --noline SVGXFormLexer.l
	@bison SVGXFormParser.y
	@echo All done..
	@clang-format -i location.hh
	@clang-format -i stack.hh
	@clang-format -i position.hh
	@clang-format -i SVGXFormParser.h
	@clang-format -i SVGXFormParser.cc
	@clang-format -i SVGXFormLexer.h
	@clang-format -i SVGXFormLexer.cc

clean:
	@rm -f location.hh
	@rm -f stack.hh
	@rm -f position.hh
	@rm -f SVGXFormParser.h
	@rm -f SVGXFormParser.cc
	@rm -f SVGXFormLexer.h
	@rm -f SVGXFormLexer.cc
