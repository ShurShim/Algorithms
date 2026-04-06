from pathlib import Path
import matplotlib.pyplot as plt


class Drawer:
    def __init__(self, files_dir: str) -> None:
        results = read_data_from_dir(Path(files_dir))
        self.data_x: dict[str, list[int]] = results[0]
        self.data_y: dict[str, list[int]] = results[1]

    def create_plot(self, x_label: str, y_label: str, output_file: str):
        plt.figure(figsize=(10, 6))
        for y_key, y_value in self.data_y:
            plt.plot(self.data_x["x"], y_value, label=y_key)
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.legend()
        plt.grid(True)
        plt.savefig(output_file, dpi=300)
        print(f"График сохранён в {output_file}")

    def create_plot_manual(self, *args, **kwargs):
        pass


def read_data_from_dir(files_dir: Path) -> list[dict[str, list[int]]]:
    data_parsed = []
    y_parsed = {}
    for file in files_dir.glob("*.txt"):
        if "n.txt" in file.name:
            data_parsed.append({"x": _get_data_from_one_file(file)})
        else:
            y_parsed[file.name] = _get_data_from_one_file(file)

    data_parsed.append(y_parsed)
    return data_parsed


def _get_data_from_one_file(filename: Path) -> list[int]:
    result = []
    with open(filename, "r") as file:
        result = [int(line) for line in file.readlines()]

    return result
