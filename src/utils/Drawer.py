from pathlib import Path
import matplotlib.pyplot as plt
import numpy as np


class Drawer:
    def __init__(self, files_dir: str) -> None:
        results = read_data_from_dir(Path(files_dir))
        self.data_x: dict[str, list[float]] = results[0]
        self.data_y: dict[str, list[float]] = results[1]

    def create_plot(self, x_label: str, y_label: str, output_file: str):
        # try:
        #     analyze_differences(
        #         self.data_x["x"],
        #         bf_times=np.array(self.data_y["basic_search.txt"]),
        #         bm_times=np.array(self.data_y["boyer_moore.txt"]),
        #         kmp_times=np.array(self.data_y["kmp.txt"]),
        #     )
        # except Exception:
        #     print("⚠️ Skip analyze...")
        plt.figure(figsize=(10, 6))
        for y_key in self.data_y:
            plt.plot(self.data_x["x"], self.data_y[y_key], label=y_key)
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.legend()
        plt.grid(True)
        plt.savefig(output_file, dpi=300)
        print(f"📁 График сохранён в {output_file}")

    def create_plot_v2(self, x_label: str, y_label: str, output_file: str):
        plt.figure(figsize=(10, 6))
        plt.plot(
            self.data_x["x"], self.data_y["boyer_moore.txt"], label="boyer_moore.txt"
        )
        plt.plot(self.data_x["x"], self.data_y["kmp.txt"], label="kmp.txt")
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.legend()
        plt.grid(True)
        plt.savefig(output_file, dpi=300)
        print(f"📁 График сохранён в {output_file}")


def read_data_from_dir(files_dir: Path) -> list[dict[str, list[float]]]:
    data_parsed = []
    y_parsed = {}
    for file in files_dir.glob("*.txt"):
        if "sizes.txt" in file.name:
            data_parsed.append({"x": _get_data_from_one_file(file)})
        else:
            y_parsed[file.name] = _get_data_from_one_file(file)

    data_parsed.append(y_parsed)
    return data_parsed


def _get_data_from_one_file(filename: Path) -> list[float]:
    result = []
    with open(filename, "r") as file:
        result = [float(line) for line in file.readlines()]

    return result


def analyze_differences(sizes, bf_times, kmp_times, bm_times):
    """Анализ статистических различий между алгоритмами"""

    # Вычисляем соотношения
    bf_vs_kmp = bf_times / kmp_times
    bf_vs_bm = bf_times / bm_times
    kmp_vs_bm = kmp_times / bm_times

    print("\n" + "=" * 70)
    print("STATISTICAL ANALYSIS OF ALGORITHM PERFORMANCE")
    print("=" * 70)

    # Средние значения
    print(f"\n📊 AVERAGE TIME (ms):")
    print(f"  • Basic Search:   {np.mean(bf_times):.4f} ms")
    print(f"  • KMP:           {np.mean(kmp_times):.4f} ms")
    print(f"  • Boyer-Moore:   {np.mean(bm_times):.4f} ms")

    # Соотношения
    print(f"\n📈 SPEEDUP FACTORS:")
    print(f"  • KMP is {np.mean(bf_vs_kmp):.2f}x faster than Basic Search")
    print(f"  • Boyer-Moore is {np.mean(bf_vs_bm):.2f}x faster than Basic Search")
    print(f"  • Boyer-Moore is {np.mean(kmp_vs_bm):.2f}x faster than KMP\n")
