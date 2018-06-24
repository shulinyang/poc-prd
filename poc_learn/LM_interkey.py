#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import csv
from typing import List, Tuple

import matplotlib.pyplot as plt
import numpy as np
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier


def reading(filename: str) -> List[List[str]]:
    """Read CSV format file

    :param filename: Complete filename (str)
    :return: data in [[code, timestamp1, timestamp2][c, t1, t2]]
    """
    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file, delimiter=';')
        data = list()
        for row in csv_reader:
            data.append(row)
        return data


def string2number(local_data: List[List[str]]) -> List[List]:
    for i in range(len(local_data)):
        for j in range(len(local_data[i])):
            try:
                local_data[i][j] = int(local_data[i][j])
            except ValueError:
                local_data[i][j] = float(local_data[i][j])
    return local_data


def split_last_column(local_data: List[List]) -> Tuple[np.ndarray, np.ndarray]:
    X = list()
    y = list()
    for i in range(len(local_data)):
        X.append(local_data[i][0:len(local_data[i]) - 1])
        y.append(local_data[i][len(local_data[i]) - 1])
    return np.asarray(X), np.asarray(y)


X, y = split_last_column(string2number(reading("alexis-remi-inter.csv")))
bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=2), algorithm="SAMME", n_estimators=500)

bdt.fit(X, y)
plot_colors = "br"
plot_step = 0.01
class_names = "AR"

plt.figure(figsize=(10, 5))

# Plot the decision boundaries
x_min, x_max = X[:, :].min() - 0.25, X[:, :].max() + 0.25
y_min, y_max = y[:].min() - 0.25, y[:].max() + 0.25

plt.axis("tight")
# Plot the two-class decision scores
twoclass_output = bdt.decision_function(X)
plot_range = (twoclass_output.min(), twoclass_output.max())
for i, n, c in zip(range(2), class_names, plot_colors):
    plt.hist(twoclass_output[y == i],
             bins=10,
             range=plot_range,
             facecolor=c,
             label='Class %s' % n,
             alpha=.5,
             edgecolor='k')
x1, x2, y1, y2 = plt.axis()
plt.axis((x1, x2, y1, y2 * 1.2))
plt.legend(loc='upper right')
plt.ylabel('Samples')
plt.xlabel('Score')
plt.title('Decision Scores')

plt.tight_layout()
plt.subplots_adjust(wspace=0.35)
plt.show()
