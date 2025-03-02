INCLUDE_DIR = +sdl3
#INCLUDE_DIR = ../src

test.opt: test.ml
	ocamlopt -I $(INCLUDE_DIR) sdl3.cmxa $< -o $@

clean:
	$(RM) *.o *.cm[ioxt] *.exe *.opt
