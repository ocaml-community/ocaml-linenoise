
all: build test

build:
	jbuilder build @install

test:
	jbuilder runtest --no-buffer --force

clean:
	jbuilder clean

doc:
	jbuilder build @doc

