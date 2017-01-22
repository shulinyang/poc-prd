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


def process_interkey(data:list)->list:
    new_data = list()
    for i in range(1, len(data)):
        new_data.append([data[i - 1][0] + "-" + data[i][0], int(data[i][1]) - int(data[i - 1][2])])
    return new_data

def writing(filename: str, data: list):
    with open(filename, 'w', encoding='utf-8', newline='') as file:
        csv_writer = csv.writer(file, delimiter=' ')
        for i in range(len(data)):
            csv_writer.writerow(data[i])


if __name__ == '__main__':
    writing("keys_proper.data", process(reading("keys.data")))
    sys.exit(0)
