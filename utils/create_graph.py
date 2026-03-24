import numpy as np
import matplotlib.pyplot as plt


def f():
    # Загрузка данных
    x = np.loadtxt("output/sizes.txt")
    y1 = np.loadtxt("output/bubble_sort.txt")
    y2 = np.loadtxt("output/merge_sort.txt")
    y3 = np.loadtxt("output/quick_sort.txt")

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
    plt.savefig("output/result.png", dpi=300)
    print("График сохранён в output/result.png")


def g():
    # Загрузка данных
    x = np.loadtxt("output/sizes.txt")
    y2 = np.loadtxt("output/merge_sort.txt")
    y3 = np.loadtxt("output/quick_sort.txt")

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
    plt.savefig("output/result2.png", dpi=300)
    print("График сохранён в output/result2.png")


f()
g()
