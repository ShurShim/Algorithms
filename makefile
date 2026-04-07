.PHONY: venv clean

lr1: src/labs/lr1/lab1.c
	gcc src/labs/lr1/lab1.c -o src/labs/lr1/lab.exe
	./src/labs/lr1/lab.exe

lr2: src/labs/lr2/lab2.cpp
	g++ src/labs/lr2/lab2.cpp -o src/labs/lr2/lab.exe
	./src/labs/lr2/lab.exe

graph:
	python3 ./src/utils/create.py $$ARGS

clean: clean_exe clean_png clean_txt
	@echo "cleaned"

clean_exe:
	find . -name "*.exe" -type f -delete

clean_txt:
	rm -rfv ./src/data/*.txt

clean_png:
	rm -rfv ./src/data/graphs/*.png
