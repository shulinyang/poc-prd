#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Pre-processing and processing for poc_ann
"""

import csv
import sys

from dataProcess import DataProcess


def reading(filename: str) -> list:
    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file, delimiter=';')
        data = list()
        for row in csv_reader:
            data.append(row)
        return data


def process(data: list) -> list:
    new_data = list()
    for i in range(len(data)):
        if (int(data[i][2]) - int(data[i][1])) > 16:
            new_data.append([data[i][0], int(data[i][2]) - int(data[i][1])])
    return new_data


def process_interkey(data: list, threshold: int) -> list:
    new_data = list()
    composite_key = [160, 161, 162, 163]
    for i in range(1, len(data)):
        duration = int(data[i][1]) - int(data[i - 1][2])
        last_vkcode = int(data[i][0])
        if 1 < duration < threshold and last_vkcode not in composite_key:
            new_data.append([data[i - 1][0] + "0" + data[i][0], duration])
    return new_data


def writing(filename: str, data: list):
    with open(filename, 'w', encoding='utf-8', newline='') as file:
        csv_writer = csv.writer(file, delimiter=' ')
        for i in range(len(data)):
            csv_writer.writerow(data[i])


def meta(list_basename: str) -> process:
    data = list()
    dproc = list()
    for i in range(len(list_basename)):
        data.append(process(reading(list_basename + ".data")))
        data[i] += process_interkey(reading(list_basename + ".data"), 2000)
        dproc.append(DataProcess())
        dproc[i].load_data(data[i])
    max_value = list()
    min_value = list()
    for i in range(len(dproc)):
        for j in range(len(dproc[i].items)):
            if i == 0:
                max_value.append(dproc[i].items[j].max)
                min_value.append(dproc[i].items[j].min)
            max_value[j] = max(max_value[j], dproc[i].items[j].max)
            min_value[j] = min(min_value[j], dproc[i].items[j].min)

    for i in range(len(list_basename)):
        for j in range(len(dproc[i].items)):
            dproc[i].items[j].max = max_value[j]
            dproc[i].items[j].min = min_value[j]
        dproc[i].scale_all()
        dproc[i].shuffle()
        dproc[i].write_data(list_basename + "_proper.data", 1, 0.75)  # <!> hard coded
        # writing(, data)


if __name__ == '__main__':
    meta("alexis")  # <!> hard coded
    meta("nicolas")  # <!> hard coded
    sys.exit(0)
