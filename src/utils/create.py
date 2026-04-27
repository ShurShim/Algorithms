import sys
import Drawer


args = sys.argv[1:]
print(args)
drawer = Drawer.Drawer(files_dir="src/data/")
drawer.create_plot(x_label="n", y_label="t", output_file="src/data/graphs/graph1.png")
drawer.create_plot_v2(
    x_label="Размер строки",
    y_label="Время(мс)",
    output_file="src/data/graphs/graph1.png",
)
