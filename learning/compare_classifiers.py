#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from sklearn.ensemble import AdaBoostClassifier
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier

from learning_machine_adaboost import split_last_column, string2number, reading

if __name__ == '__main__':
    plt.figure(figsize=(10, 5))
    dataset_name = "alexis-nicolas"
    data, _ = string2number(reading(dataset_name + "-train.csv"))
    X, y = split_last_column(data)
    data, _ = string2number(reading(dataset_name + "-test.csv"))
    X_test, y_test = split_last_column(data)
    svm = SVC()
    svm.fit(X, y)
    bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=5), algorithm="SAMME", n_estimators=1000)
    bdt.fit(X, y)
    ada = AdaBoostClassifier(algorithm="SAMME", n_estimators=1000)
    ada.fit(X, y)
    dt = DecisionTreeClassifier(max_depth=5)
    dt.fit(X, y)

    print("AdaBoost+DT: " + str(bdt.score(X_test, y_test)))
    print("SVM: " + str(svm.score(X_test, y_test)))
    print("AdaBoost: " + str(ada.score(X_test, y_test)))
    print("DT: " + str(dt.score(X_test, y_test)))


def plotting(X, bdt, svm, ada):
    plot_colors = "br"
    plot_step = 0.005
    class_names = "AN"
    # Plot the two-class decision scores
    twoclass_output = bdt.decision_function(X)
    plot_range = (twoclass_output.min(), twoclass_output.max())
    plt.subplot(131)
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
    plt.title('Decision Scores AdaBoost+DT')

    twoclass_output = svm.decision_function(X)
    plot_range = (twoclass_output.min(), twoclass_output.max())
    plt.subplot(132)
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
    plt.title('Decision Scores SVM')

    twoclass_output = ada.decision_function(X)
    plot_range = (twoclass_output.min(), twoclass_output.max())
    plt.subplot(133)
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
    plt.title('Decision Scores Adaboost')

    plt.tight_layout()
    plt.subplots_adjust(wspace=0.35)
    plt.show()
