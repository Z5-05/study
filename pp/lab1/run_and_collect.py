import subprocess
import matplotlib.pyplot as plt
import plotly.express as px
from tqdm import tqdm
import pandas as pd

cmd = "./runner.sh {xi}"

x = range(1000, 1000000, 10000)
y_simple = []
y_threads = []


for xi in tqdm(x):
    p = subprocess.Popen(cmd.format(xi=xi), shell=True, stdout=subprocess.PIPE).communicate()
    results = p[0].decode().replace("thread::id of a non-executing thread", "").split("\n")
    for string in results:
        if "Количество потоков" in string:
            threads, time = int(string.split(" ")[-3]), float(string.split(" ")[-1])
            if threads == 8:
                y_threads.append(time)
            elif threads == 1:
                y_simple.append(time)
            else:
                raise ValueError("Num of threads...")

df = pd.DataFrame.from_dict({"Размер массива": x,
                             "Один поток": y_simple,
                             "Много потоков": y_threads,})

fig = px.line(df, x="Размер массива", y=["Один поток", "Много потоков"])
fig.write_html("graphs/file.html")
fig.show()