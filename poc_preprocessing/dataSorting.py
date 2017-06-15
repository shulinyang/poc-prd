#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Description here.
"""
import csv
from typing import List, Callable


class DataSorting():
    """
    Partial implementation.
    """
    def __init__(self, data: list = None):
        self.data = data  # type: list
        self.sorted_data = dict()  # type: dict

    def sort(self) -> Callable:
        if self.data is not None:
            for j in self.data:
                if j[0] not in self.sorted_data.keys():
                    self.sorted_data[j[0]] = list()
                self.sorted_data[j[0]].append(j[1])
        return self

    def load_data(self, data: List[List[int]]) -> Callable:
        self.data = data
        return self

    def export_csv(self, filename: str) -> None:
        with open(filename + "-sorted.csv", "w", encoding='utf8', newline='') as csv_file:
            csv_writer = csv.writer(csv_file, delimiter=';', quotechar='|', quoting=csv.QUOTE_MINIMAL,
                                    dialect='excel')
            for key in self.sorted_data.keys():
                csv_writer.writerow([key] + self.sorted_data[key])
        print(filename + " as been exported.")
