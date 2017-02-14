#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import random

from dataItem import DataItem


class DataProcess():
    def __init__(self):
        self.items = list()
        self.nItem = 0
        self.nData = 0

    def scale_all(self):
        for i in range(self.nItem):
            self.items[i].scale()

    def shuffle(self):
        for i in range(int(self.nData / 2)):
            swap = int(random.randint(0, 65556) % self.nData)
            if i != swap:
                for j in range(self.nItem):
                    self.items[j].swap(i, swap)

    def write_scale_parameter(self):
        with open("-scalable.txt", "w", encoding='utf8') as file:
            file.write("Scaling Parameters Text File generated by FannTool \n")
            file.write("Column No  ( Minimum , Maximum ) --> ( Minimum Out , Maximum Out ) \n")
            for j in range(self.nItem):
                if (self.items[j].scalable):
                    file.write(str(j + 1) + " ( " + str(self.items[j].min) + " , " + str(
                        self.items[j].max) + " ) --> ( " + str(self.items[j].minv) + " , " + str(
                        self.items[j].maxv) + " ) \n")
                else:
                    file.write("j+1 ( items[j]->GetMin() , items[j]->GetMax() ) --> Not Scaled \n")

    def load_data(self, data: list):
        self.nItem = 2
        self.nData = len(data)
        for i in range(self.nItem):
            self.items.append(DataItem())
            for j in range(self.nData):
                self.items[i].add(float(data[j][i]))

    def write_data(self, filename: str, nOut: int, pTrain: float):
        Train = min(pTrain, 1)
        Train = max(0.5, Train)
        nTrainData = int(self.nData * Train)
        nTestData = int(self.nData - nTrainData)
        nInput = self.nItem - nOut

        with open(filename + "-train.dat", 'w', encoding='utf-8') as file:
            file.write(str(nTrainData) + " " + str(nInput) + " " + str(nOut) + " \n")
            for n in range(nTrainData):
                for i in range(nInput):
                    file.write("{0:.15f}".format(self.items[i].data[n])+'\n')
                for i in range(self.nItem):
                    file.write("{0:.15f}".format(self.items[i].data[n])+'\n')

        if nTestData:
            with open(filename + "-test.dat", 'w', encoding='utf-8') as file:
                file.write(str(nTestData) + " " + str(nInput) + " " + str(nOut) + " \n")
                for n in range(nTrainData, self.nData):
                    for i in range(nInput):
                        file.write("{0:.15f}".format(self.items[i].data[n])+'\n')
                    for i in range(nInput, self.nItem):
                        file.write("{0:.15f}".format(self.items[i].data[n])+'\n')
