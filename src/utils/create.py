import argparse
import Drawer


parser = argparse.ArgumentParser()
parser.add_argument("--first_filename", type=str)
parser.add_argument("--second_filename", type=str)

args = parser.parse_args()
print("Аргументы запуска: ", args)
drawer = Drawer.Drawer(files_dir="src/data/")
drawer.create_plot(
    x_label="Размер", y_label="Время(м/с)", output_file="src/data/graph1.png"
)
drawer.create_plot_v2(
    x_label="Размер",
    y_label="Время(м/с)",
    output_file="src/data/graph2.png",
    first=args.first_filename,
    second=args.second_filename,
)
