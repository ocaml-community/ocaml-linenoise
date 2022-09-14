
all: build test

build:
	@dune build @install

test:
	@dune runtest --no-buffer --force

example:
	@dune exec examples/show_off.exe

clean:
	@dune clean

doc:
	@dune build @doc

.PHONY: all build test example clean doc
