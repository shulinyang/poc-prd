#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Pre-processing and processing for poc_ann
"""

import csv
import sys

from dataProcess import DataProcess
from table_vkcode import x11_code_to_vkcode


def reading(filename: str) -> list:
    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file, delimiter=';')
        data = list()
        for row in csv_reader:
            data.append(row)
        return data


def convert_to_vkcode(data: list) -> list:
    new_data = list()
    for i in range(len(data)):
        new_data.append([data[i][0], str(x11_code_to_vkcode()[data[i][2]]), data[i][3]])  # <!> hard coded
    return new_data


def preprocess(data: list) -> list:
    new_data = list()
    stack = list()
    for i in range(len(data)):
        if int(data[i][0]) == 2:    # <!> hard coded
            stack.append(data[i])
        if int(data[i][0]) == 3:
            for j in range(len(stack)):
                try:
                    if data[i][1] == stack[j][1]:
                        new_data.append([data[i][1], stack.pop(j)[2], data[i][2]])
                except IndexError as ie:
                    pass
    if len(stack) != 0:
        print("Wasted elements.")
        print(stack)
    return new_data


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

def prepare_x11(basename):
     local_data = preprocess(convert_to_vkcode(reading(basename+"x11.data")))
     with open(basename+".data", 'w', encoding='utf-8', newline='') as file:
         csv_writer = csv.writer(file, delimiter=';')
         for i in range(len(local_data)):
             csv_writer.writerow(local_data[i])


def meta(list_basename: list) -> process:
    data = list()
    dproc = list()
    for i in range(len(list_basename)):
        local_data = reading(list_basename[i] + ".data")
        data.append(process(local_data))
        data[i] += process_interkey(local_data, 2000)
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
        dproc[i].write_data(list_basename[i] + "_proper.data", 1, 0.75)  # <!> hard coded
        # writing(, data)

if __name__ == '__main__':
    prepare_x11("remi")
    meta(["alexis","nicolas", "remi"])  # <!> hard coded
    sys.exit(0)
