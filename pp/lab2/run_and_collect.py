import subprocess
import matplotlib.pyplot as plt
import plotly.express as px
from tqdm import tqdm
import pandas as pd

cmd = "./runner.sh {xi}"

x = range(1000, 100000, 10000)
y_simple = []
y_threads = []
y_binary_search = []


for xi in tqdm(x):
    p = subprocess.Popen(cmd.format(xi=xi), shell=True, stdout=subprocess.PIPE).communicate()
    results = p[0].decode().split("\n")
    result1 = float(results[0].split(",")[-1])
    result2 = float(results[1].split(",")[-1])
    result3 = float(results[2].split(",")[-1])
    y_simple.append(result1), y_threads.append(result2), y_binary_search.append(result3)

df = pd.DataFrame.from_dict({"Размер массива": x,
                             "Простой Merge Sort": y_simple,
                             "Многопоточный Merge Sort": y_threads, 
                             "Многопоточный Merge Sort с binary search": y_binary_search})

fig = px.line(df, x="Размер массива", y=["Простой Merge Sort", "Многопоточный Merge Sort", "Многопоточный Merge Sort с binary search"])
fig.write_html("graphs/file.html")
fig.show()