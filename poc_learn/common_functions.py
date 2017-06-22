#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import csv
from typing import List, Dict


def reading(filename: str) -> List[List]:
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


def writing(filename: str, data: List[List]) -> None:
    """Write CSV file

    :param filename: filename (str)
    :param data: list of lists
    :return: None
    """
    with open(filename, 'w', encoding='utf-8', newline='') as file:
        csv_writer = csv.writer(file, delimiter=' ')
        for i in range(len(data)):
            csv_writer.writerow(data[i])


def cast_int(data: list) -> List[int]:
    return [int(x) for x in data]


def list_to_dict(data: List[List]) -> Dict[List]:
    sorted_data = dict()
    for i in data:
        sorted_data[i[0]] = cast_int(i[1:])
    return sorted_data


def find_matching_list(data1: Dict[List], data2: Dict[List]) -> list:
    matching = set(data1.keys()).intersection(set(data2))
    return list(matching)
