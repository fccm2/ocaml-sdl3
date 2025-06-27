#INCLUDE_DIR = +sdl3
INCLUDE_DIR = ../src

all: test.opt test7.opt

%.opt: %.ml
	ocamlopt -I $(INCLUDE_DIR) sdl3.cmxa $< -o $@

clean:
	$(RM) *.o *.cm[ioxt] *.exe *.opt *.byte
