
all: clean
	@flex --noline RIDL.l
	@bison RIDL.y
	@echo All done..

clean:
	@rm -f location.hh
	@rm -f stack.hh
	@rm -f position.hh
	@rm -f Parser.h
	@rm -f Parser.cc
	@rm -f Lexer.h
	@rm -f Lexer.cc
