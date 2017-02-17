#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
    class DataProcess
    Processing data for FANN neural network
    Recoding of poc_data_processing
"""

import random

from dataItem import DataItem

class DataProcess:
    def __init__(self):
        self.items = list()
        self.n_item = 0 # number of input / output
        self.n_data = 0 # number of samples

    def scale_all(self):
        for i in range(self.n_item):
            self.items[i].scale()

    def shuffle(self):
        for i in range(int(self.n_data / 2)):
            swap = int(random.randint(0, 65556) % self.n_data)
            if i != swap:
                for j in range(self.n_item):
                    self.items[j].swap(i, swap)

    def write_scale_parameter(self):
        with open("-scalable.txt", "w", encoding='utf8') as file:
            file.write("Scaling Parameters Text File generated by FannTool \n")
            file.write("Column No  ( Minimum , Maximum ) --> ( Minimum Out , Maximum Out ) \n")
            for j in range(self.n_item):
                if (self.items[j].scalable):
                    file.write(str(j + 1) + " ( " + str(self.items[j].min) + " , " + str(
                        self.items[j].max) + " ) --> ( " + str(self.items[j].minv) + " , " + str(
                        self.items[j].maxv) + " ) \n")
                else:
                    file.write("j+1 ( items[j]->GetMin() , items[j]->GetMax() ) --> Not Scaled \n")

    def load_data(self, data: list):
        self.n_item = 2  # <!> hard coding
        self.n_data = len(data)
        for i in range(self.n_item):
            self.items.append(DataItem())
            for j in range(self.n_data):
                self.items[i].add(float(data[j][i]))

    def write_data(self, filename: str, n_out: int, p_train: float):
        train = min(p_train, 1)
        train = max(0.5, train)
        n_train_data = int(self.n_data * train)
        n_test_data = int(self.n_data - n_train_data)
        n_input = self.n_item - n_out

        with open(filename + "-train.dat", 'w', encoding='utf-8') as file:
            file.write(str(n_train_data) + " " + str(n_input) + " " + str(n_out) + " \n")
            for n in range(n_train_data):
                for i in range(n_input):
                    file.write("{0:.15f}".format(self.items[i].data[n])+'\n')
                for i in range(self.n_item):
                    file.write("{0:.15f}".format(self.items[i].data[n])+'\n')

        if n_test_data:
            with open(filename + "-test.dat", 'w', encoding='utf-8') as file:
                file.write(str(n_test_data) + " " + str(n_input) + " " + str(n_out) + " \n")
                for n in range(n_train_data, self.n_data):
                    for i in range(n_input):
                        file.write("{0:.15f}".format(self.items[i].data[n])+'\n')
                    for i in range(n_input, self.n_item):
                        file.write("{0:.15f}".format(self.items[i].data[n])+'\n')
