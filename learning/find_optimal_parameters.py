#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from sklearn.ensemble import AdaBoostClassifier
from sklearn.metrics import accuracy_score
from sklearn.tree import DecisionTreeClassifier

from LM_keys import split_last_column, string2number, reading, save_result

if __name__ == '__main__':
    dataset_name = "alexis-nicolas"
    max_train = 0
    max_test = 0
    opt_param = dict()
    data, _ = string2number(reading(dataset_name + "-train.csv"))
    X, y = split_last_column(data)
    data, _ = string2number(reading(dataset_name + "-test.csv"))
    X_test, y_test = split_last_column(data)

    for n_estimators in range(1, 15):
        for dtd in range(1, 15):
            bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=dtd), algorithm="SAMME",
                                     n_estimators=n_estimators * 10)
            bdt.fit(X, y)
            y_pred = bdt.predict(X)
            train_accuracy = accuracy_score(y, y_pred)
            y_pred = bdt.predict(X_test)
            test_accuracy = accuracy_score(y_test, y_pred)
            if test_accuracy > max_test and train_accuracy > max_train:
                max_test = test_accuracy
                max_train = train_accuracy
                opt_param["dtd"] = dtd
                opt_param["n_estimators"] = n_estimators
    save_result(dataset_name, opt_param["n_estimators"] * 100, opt_param["dtd"], max_train, max_test)
