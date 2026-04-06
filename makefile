.PHONY: venv clean
lr1: n1/lab1.c
	gcc n1/lab1.c -o n1/lab1.exe
	./n1/lab1.exe

lr2: n2/lab2.c
	gcc n2/lab2.c -o n2/lab2.exe
	./n2/lab2.exe

venv:
	python

clean:
	rm -rfv *.exe
