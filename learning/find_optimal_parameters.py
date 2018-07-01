#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from sklearn.ensemble import AdaBoostClassifier
from sklearn.metrics import accuracy_score
from sklearn.tree import DecisionTreeClassifier

from learning_machine_adaboost import split_last_column, string2number, reading, save_result

if __name__ == '__main__':
    dataset_name = "remi-nicolas"
    max_train = 0
    max_test = 0
    opt_param = dict()
    data, _ = string2number(reading(dataset_name + "-train.csv"))
    X, y = split_last_column(data)
    data, _ = string2number(reading(dataset_name + "-test.csv"))
    X_test, y_test = split_last_column(data)
    n_estimators_range = [10 * i for i in range(1, 10)] + [100 * i for i in range(1, 15)]
    for lr in [0.7, 0.8, 0.9, 1]:
        for n_estimators in n_estimators_range:
            for dtd in range(1, 11):
                bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=dtd), algorithm="SAMME",
                                         n_estimators=n_estimators, learning_rate=lr)
                bdt.fit(X, y)
                y_pred = bdt.predict(X)
                train_accuracy = accuracy_score(y, y_pred)
                y_pred = bdt.predict(X_test)
                test_accuracy = accuracy_score(y_test, y_pred)
                if test_accuracy > max_test:
                    max_test = test_accuracy
                    max_train = train_accuracy
                    opt_param["dtd"] = dtd
                    opt_param["n_estimators"] = n_estimators
                    opt_param["learning_rate"] = lr
                    print(
                        "%s; %s; %s; %s; %s; %s" % (dataset_name, n_estimators, dtd, train_accuracy, test_accuracy, lr))
    save_result("result.txt", dataset_name, opt_param["n_estimators"], opt_param["dtd"], max_train, max_test,
                opt_param["learning_rate"])
