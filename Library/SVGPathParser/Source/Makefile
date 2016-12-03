
all: clean
	@flex --noline SVGPathLexer.l
	@bison SVGPathParser.y
	@echo All done..

clean:
	@rm -f location.hh
	@rm -f stack.hh
	@rm -f position.hh
	@rm -f SVGPathParser.h
	@rm -f SVGPathParser.cc
	@rm -f SVGPathLexer.h
	@rm -f SVGPathLexer.cc
