.PHONY: venv clean

lr1: src/labs/n1/lab1.c
	gcc src/labs/n1/lab1.c -o src/labs/n1/lab1.exe
	./src/labs/n1/lab1.exe
	make graph ARGS="none"

lr2: src/labs/n2/lab2.cpp
	g++ src/labs/n2/lab2.cpp -o src/labs/n2/lab2.exe
	./src/labs/n2/lab2.exe
	make graph ARGS="none"

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
