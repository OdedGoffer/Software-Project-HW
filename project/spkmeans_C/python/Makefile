.PHONY: python clean

target: python

python: %.so

%.so: setup.py
	python3 $< build_ext -i

clean: setup.py
	-@rm -rf build
	python3 $< clean
	-@rm *.so