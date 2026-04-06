.PHONY: venv clean

build_lr1: src/labs/n1/lab1.c
	gcc src/labs/n1/lab1.c -o src/labs/n1/lab1.exe

lr2: n2/lab2.c
	gcc n2/lab2.c -o n2/lab2.exe -lm
	./n2/lab2.exe

venv:
	python

clean:
	rm -rfv *.exe
