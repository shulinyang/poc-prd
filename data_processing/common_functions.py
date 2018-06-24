#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import csv


def reading(filename: str) -> list:
    """Read CSV format file

    :param filename: Complete filename (str)
    :return: data in [[code, timestamp1, timestamp2][c, t1, t2]]
    """
    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file, delimiter=';')
        data = list()
        for row in csv_reader:
            data.append(row)
        return data


def writing(filename: str, data: list):
    """Write CSV file

    :param filename: filename (str)
    :param data: list of lists
    :return: None
    """
    with open(filename, 'w', encoding='utf-8', newline='') as file:
        csv_writer = csv.writer(file, delimiter=' ')
        for i in range(len(data)):
            csv_writer.writerow(data[i])
