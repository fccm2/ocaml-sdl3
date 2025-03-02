all: sdl3.cmxa sdl3.cma

sdl3.cmi: sdl3.mli
	ocamlc -c $<

sdl3.cmx: sdl3.ml sdl3.cmi
	ocamlopt -c $<

sdl3_stubs.o: sdl3_stubs.c
	ocamlopt -c $<

sdl3.cmxa: sdl3.cmx sdl3_stubs.o
	ocamlmklib -o sdl3 -oc sdl3_stub sdl3.cmx sdl3_stubs.o -L/usr/local/lib -cclib -lSDL3

sdl3.cma: sdl3.cmo sdl3_stubs.o
	ocamlmklib -o sdl3 -oc sdl3_stub sdl3.cmo sdl3_stubs.o -L/usr/local/lib -linkall -lSDL3

sdl3.cmo: sdl3.ml sdl3.cmi
	ocamlc -c $<

clean:
	$(RM) *.[oa] *.so *.cm[ioxat] *.cmx[as] *.exe *.opt

doc:
	mkdir -p doc
	ocamldoc -d doc -html sdl3.mli

PREFIX = $(shell ocamlc -where)/sdl3
CP = cp
install:
	mkdir -p $(PREFIX)
	$(CP)             \
	  dllsdl3_stub.so \
	  libsdl3_stub.a  \
	  sdl3.a          \
	  sdl3.cmi        \
	  sdl3.cmx        \
	  sdl3.cmxa       \
	  sdl3.cma        \
	  sdl3.mli        \
	  $(PREFIX)/
