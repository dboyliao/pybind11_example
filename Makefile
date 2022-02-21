PYTHON?=$$(which python3)

all: configure python

configure:
	mkdir -p build; \
	cd build; \
	cmake -DPYTHON_EXECUTABLE=$(PYTHON) ..

python:
	cd build && \
	make

clean:
	cd build && \
	make clean

.PHONY: clean
