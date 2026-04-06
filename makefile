.PHONY: venv clean

build_lr1: src/labs/n1/lab1.c
	gcc src/labs/n1/lab1.c -o src/labs/n1/lab1.exe

lr2: src/labs/n2/lab2.c
	gcc src/labs/n2/lab2.c -o src/labs/n2/lab2.exe
	./src/labs/n2/lab2.exe

lr2_cpp: src/labs/n2/main.cpp
	g++ src/labs/n2/main.cpp -o src/labs/n2/main.exe
	./src/labs/n2/main.exe
	make graph ARGS="none"

graph:
	python3 ./src/utils/create.py $$ARGS

clean: clean_exe clean_png clean_txt
	@echo "cleaned"

clean_exe:
	rm -rfv *.exe

clean_txt:
	rm -rfv ./src/data/*.txt

clean_png:
	rm -rfv ./src/data/graphs/*.png
