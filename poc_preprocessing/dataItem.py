#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
    class DataItem
    Data storage for FANN neural network
    Recoding of poc_processing
"""


class DataItem:
    def __init__(self):
        self.scalable = True
        self.min = 0
        self.max = 0
        self.minv = 0
        self.maxv = 1
        self.sum = 0
        self.avg = 0
        self.data = list()

    def scale(self, minv: float = None, maxv: float = None):
        def inner_scale(current_object: DataItem):
            if not current_object.scalable:
                return
            for i in range(len(current_object.data)):
                current_object.data[i] = (((current_object.maxv - current_object.minv) * (
                current_object.data[i] - current_object.min)) / (
                                              current_object.max - current_object.min)) + current_object.minv

        if minv is not None:
            self.minv = minv
            self.maxv = maxv
        inner_scale(self)

    def set_data(self, item: int, value: float):
        self.min = min(value, self.min)
        self.max = max(value, self.max)
        self.sum += value
        self.sum -= self.data[item]
        self.data[item] = value
        self.avg = self.sum / len(self.data)

    def add(self, value: float):
        if len(self.data) == 0:
            self.min = value
            self.max = value
        self.min = min(self.min, value)
        self.max = max(self.max, value)
        self.sum += value
        self.data.append(value)
        self.avg = self.sum / len(self.data)

    def swap(self, item1: int, item2: int):
        tmp = self.data[item1]
        self.data[item1] = self.data[item2]
        self.data[item2] = tmp
