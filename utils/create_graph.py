import matplotlib.pyplot as plt


def f():
    # Загрузка данных
    x = read_data("output/sizes.txt")

    # Создаём один график
    plt.figure(figsize=(10, 6))

    # Рисуем три кривые
    plt.plot(x, y1, "b-", label="bubble_sort")
    plt.plot(x, y2, "r-", label="merge_sort")
    plt.plot(x, y3, "g-", label="quick_sort")

    # Подписи и оформление
    plt.xlabel("Размер массива (X)")
    plt.ylabel("Время (Y)")
    plt.title("Сравнение алгоритмов сортировки")
    plt.legend()
    plt.grid(True)

    # Сохраняем график
    plt.savefig("output/result.png", dpi=300)
    print("График сохранён в output/result.png")


def g():
    # Загрузка данных
    x = read_data("output/sizes.txt")
    y2 = read_data("output/merge_sort.txt")
    y3 = read_data("output/quick_sort.txt")

    # Создаём один график
    plt.figure(figsize=(10, 6))

    # Рисуем три кривые
    plt.plot(x, y2, "r-", label="merge_sort")
    plt.plot(x, y3, "g-", label="quick_sort")

    # Подписи и оформление
    plt.xlabel("Размер массива (X)")
    plt.ylabel("Время(Y)")
    plt.title("Сравнение алгоритмов сортировки")
    plt.legend()
    plt.grid(True)

    # Сохраняем график
    plt.savefig("output/result2.png", dpi=300)
    print("График сохранён в output/result2.png")


f()
g()
