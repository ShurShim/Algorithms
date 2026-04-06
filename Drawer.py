import matplotlib.pyplot as plt


class Drawer:
    def __init__(self, file_data_x: str, files_data_y: list[str]) -> None:
        self.data_x: list[int] = read_data_from_file(file_data_x)
        self.data_y: dict[str, list[int]] = read_data_from_file(files_data_y)

    def create_plot(self, x_label: str, y_label: str, output_file: str):
        plt.figure(figsize=(10, 6))
        for y_key, y_value in self.data_y:
            plt.plot(self.data_x, y_value, label=y_key)
        plt.xlabel(x_label)
        plt.ylabel(y_label)
        plt.legend()
        plt.grid(True)
        plt.savefig(output_file, dpi=300)
        print(f"График сохранён в {output_file}")

    def create_plot_manual(self, *args, **kwargs):
        pass


def read_data_from_file(files: str | list[str]) -> list[int] | dict[str, list[int]]:
    if isinstance(files, str):
        data_parsed = _get_data_from_one_file(files)

    elif isinstance(files, list):
        files_names = [_get_file_name(file) for file in files]
        files_data = [_get_data_from_one_file(file) for file in files]
        data_parsed = dict(zip(files_names, files_data))

    else:
        raise TypeError("Error in type of files")

    return data_parsed


def _get_file_name(filename: str) -> str:
    return filename.strip().split("/")[-1]  # output/filename.txt -> filename.txt


def _get_data_from_one_file(filename: str) -> list[int]:
    result = []
    with open(filename, "r") as file:
        result = [int(line) for line in file.readlines()]

    return result
