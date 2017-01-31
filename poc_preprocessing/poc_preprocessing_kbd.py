#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import csv
import sys


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
        first_vkcode = int(data[i - 1][0])
        last_vkcode = int(data[i][0])
        if 1 < duration < threshold and first_vkcode not in composite_key and last_vkcode not in composite_key:
            new_data.append([data[i - 1][0] + "-" + data[i][0], duration])
    return new_data


def writing(filename: str, data: list):
    with open(filename, 'w', encoding='utf-8', newline='') as file:
        csv_writer = csv.writer(file, delimiter=' ')
        for i in range(len(data)):
            csv_writer.writerow(data[i])


def meta(basename: str):
    data = process(reading(basename + ".data"))
    data += process_interkey(reading(basename + ".data"), 5500)
    writing(basename + "_proper.data", data)


if __name__ == '__main__':
    meta("alexis")
    meta("nicolas")
    sys.exit(0)
