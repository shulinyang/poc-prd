#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import csv
from typing import List

import matplotlib.pyplot as plt
import numpy as np
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier


def reading(filename: str) -> List[List]:
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


alexis_data = reading("alexis-sorted.csv")
for i in range(len(alexis_data)):
    for j in range(len(alexis_data[i])):
        try:
            alexis_data[i][j] = int(alexis_data[i][j])
        except ValueError:
            alexis_data[i][j] = float(alexis_data[i][j])

X = list()
y = list()
for i in range(len(alexis_data)):
    X.append(alexis_data[i][0:len(alexis_data[i]) - 1])
    y.append(alexis_data[i][len(alexis_data[i]) - 1])

bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=5),
                         algorithm="SAMME",
                         n_estimators=2000)
X = np.asarray(X)
y = np.asarray(y)
bdt.fit(X, y)
plot_colors = "br"
plot_step = 0.01
class_names = "AB"

plt.figure(figsize=(10, 5))

# Plot the decision boundaries
plt.subplot(121)
x_min, x_max = X[:, :].min() - 1, X[:, :].max() + 1
y_min, y_max = y[:].min() - 1, y[:].max() + 1

xx, yy = np.meshgrid(np.arange(x_min, x_max, plot_step),
                     np.arange(y_min, y_max, plot_step))

my_vector = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0]
my_vector = [my_vector for i in range(np.c_[xx.ravel()].shape[0])]
my_vector = np.concatenate((my_vector, np.c_[xx.ravel()]), axis=1)

# Z = bdt.predict(np.c_[xx.ravel(), yy.ravel()])
Z = bdt.predict(my_vector)

Z = Z.reshape(xx.shape)
cs = plt.contourf(xx, yy, Z, cmap=plt.cm.Paired)
plt.axis("tight")

# Plot the training points
for i, n, c in zip(range(2), class_names, plot_colors):
    idx = np.where(y == i)
    plt.scatter(X[idx, 0], X[idx, 1],
                c=c, cmap=plt.cm.Paired,
                s=20, edgecolor='k',
                label="Class %s" % n)
plt.xlim(x_min, x_max)
plt.ylim(y_min, y_max)
plt.legend(loc='upper right')
plt.xlabel('x')
plt.ylabel('y')
plt.title('Decision Boundary')

# Plot the two-class decision scores
twoclass_output = bdt.decision_function(X)
plot_range = (twoclass_output.min(), twoclass_output.max())
plt.subplot(122)
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