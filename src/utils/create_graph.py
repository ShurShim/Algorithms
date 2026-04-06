import numpy as np
import matplotlib.pyplot as plt


def f():
    # Загрузка данных
    x = np.loadtxt("src/data/raw/sizes.txt")
    y1 = np.loadtxt("src/data/raw/bubble_sort.txt")
    y2 = np.loadtxt("src/data/raw/merge_sort.txt")
    y3 = np.loadtxt("src/data/raw/quick_sort.txt")

    # Создаём один график
    plt.figure(figsize=(10, 6))

    # Рисуем три кривые
    plt.plot(x, y1, "b-", label="bubble_sort")
    plt.plot(x, y2, "r-", label="merge_sort")
    plt.plot(x, y3, "g-", label="quick_sort")

    # Подписи и оформление
    plt.xlabel("Размер массива (X)")
    plt.ylabel("Время / Кол-во операций (Y)")
    plt.title("Сравнение алгоритмов сортировки")
    plt.legend()
    plt.grid(True)

    # Сохраняем график
    plt.savefig("src/data/result.png", dpi=300)
    print("График сохранён в src/data/result.png")


def g():
    # Загрузка данных
    x = np.loadtxt("src/data/raw/sizes.txt")
    y2 = np.loadtxt("src/data/raw/merge_sort.txt")
    y3 = np.loadtxt("src/data/raw/quick_sort.txt")

    # Создаём один график
    plt.figure(figsize=(10, 6))

    # Рисуем три кривые
    plt.plot(x, y2, "r-", label="merge_sort")
    plt.plot(x, y3, "g-", label="quick_sort")

    # Подписи и оформление
    plt.xlabel("Размер массива (X)")
    plt.ylabel("Время / Кол-во операций (Y)")
    plt.title("Сравнение алгоритмов сортировки")
    plt.legend()
    plt.grid(True)

    # Сохраняем график
    plt.savefig("src/data/result2.png", dpi=300)
    print("График сохранён в src/data/result2.png")


f()
g()
