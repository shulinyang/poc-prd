#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import csv
from typing import List, Tuple

import matplotlib.pyplot as plt
import numpy as np
from sklearn.ensemble import AdaBoostClassifier
from sklearn.metrics import zero_one_loss, accuracy_score
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


def string2number(local_data: List[List[str]]) -> Tuple[List[List], int]:
    float_position = -1
    for i in range(len(local_data)):
        for j in range(len(local_data[i])):
            try:
                local_data[i][j] = int(local_data[i][j])
            except ValueError:
                local_data[i][j] = float(local_data[i][j])
                float_position = j
    return local_data, float_position


def split_last_column(local_data: List[List]) -> Tuple[np.ndarray, np.ndarray]:
    X = list()
    y = list()
    for i in range(len(local_data)):
        X.append(local_data[i][0:len(local_data[i]) - 1])
        y.append(local_data[i][len(local_data[i]) - 1])
    return np.asarray(X), np.asarray(y)


def plot_the_train(bdt: AdaBoostClassifier, X: np.ndarray, y: np.ndarray, float_pos: float):
    plot_colors = "br"
    plot_step = 0.005
    class_names = "RN"

    plt.figure(figsize=(10, 5))

    # Plot the decision boundaries
    plt.subplot(121)
    x_min, x_max = X[:, :].min() - 0.25, X[:, :].max() + 0.25
    y_min, y_max = y[:].min() - 0.25, y[:].max() + 0.25

    xx, yy = np.meshgrid(np.arange(x_min, x_max, plot_step),
                         np.arange(y_min, y_max, plot_step))

    my_vector = [0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    my_vector = [my_vector for i in range(np.c_[xx.ravel()].shape[0])]
    my_vector = np.concatenate((my_vector, np.c_[xx.ravel()]), axis=1)

    Z = bdt.predict(my_vector)
    Z = Z.reshape(xx.shape)
    cs = plt.contourf(xx, yy, Z, cmap=plt.cm.Paired)
    plt.axis("tight")

    # Plot the training points
    for i, n, c in zip(range(2), class_names, plot_colors):
        idx = np.where(y == i)

        plt.scatter(X[idx, float_pos], y[idx],
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


def testing(bdt: AdaBoostClassifier, X_train, y_train, X_test: np.ndarray, y_test: np.ndarray, n_estimators):
    ada_discrete_err_train = np.zeros((n_estimators,))
    for i, y_pred in enumerate(bdt.staged_predict(X_train)):
        ada_discrete_err_train[i] = zero_one_loss(y_train, y_pred)

    ada_discrete_err_test = np.zeros((n_estimators,))
    for i, y_pred in enumerate(bdt.staged_predict(X_test)):
        ada_discrete_err_test[i] = zero_one_loss(y_test, y_pred)

    fig = plt.figure(2)
    ax = fig.add_subplot(111)

    ax.plot(np.arange(n_estimators) + 1, ada_discrete_err_train,
            label='AdaBoost Train 01 Loss',
            color='blue')

    ax.plot(np.arange(n_estimators) + 1, ada_discrete_err_test,
            label='AdaBoost Test 01 Loss',
            color='green')

    max_y = max(ada_discrete_err_train[:, ].max() + 0.25, ada_discrete_err_test[:, ].max() + 0.25)
    ax.set_ylim((0.0, max_y))
    ax.set_xlabel('n_estimators')
    ax.set_ylabel('error rate')

    leg = ax.legend(loc='upper right', fancybox=True)
    leg.get_frame().set_alpha(0.7)

    plt.show()


if __name__ == "__main__":
    n_estimators = 1000
    data, float_pos = string2number(reading("remi-nicolas-train.csv"))
    X, y = split_last_column(data)

    bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=5), algorithm="SAMME", n_estimators=n_estimators)
    bdt.fit(X, y)

    y_pred = bdt.predict(X)
    print(accuracy_score(y, y_pred))

    plot_the_train(bdt, X, y, float_pos)

    data, _ = string2number(reading("remi-nicolas-test.csv"))
    X_test, y_test = split_last_column(data)

    testing(bdt, X, y, X_test, y_test, n_estimators)

    y_pred = bdt.predict(X_test)
    print(accuracy_score(y_test, y_pred))
