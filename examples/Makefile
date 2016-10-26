.PHONY:all

all:
	@ocamlfind ocamlopt show_off.ml -package linenoise -linkpkg -o T
	@./T

clean:
	@rm -rf *.o *.c* T history.txt
