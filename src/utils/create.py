import sys
import Drawer


args = sys.argv[1:]
print(args)
drawer = Drawer.Drawer(files_dir="src/data/")
drawer.create_plot(x_label="n", y_label="t", output_file="src/data/graph1.png")
try:
    drawer.create_plot_v2(x_label="n", y_label="t", output_file="src/data/graph2.png")
except Exception:
    print("ну ошибка чо")
