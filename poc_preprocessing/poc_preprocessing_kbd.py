#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Pre-processing and processing for poc_ann
Replace poc_data_processing
"""

import csv
import sys
from typing import List

from common_functions import reading
from dataSorting import DataSorting
from mapper import Mapper
from table_vkcode import x11_code_to_vkcode


def convert_to_vkcode(data: List[List]) -> List[List]:
    """Convert hardware code to vkCode, only for X11 agent
    <!> hard code
    :param data: list like [[event type, code1, code2, timestamp], [et, c1, c2, t1]]
    :return: list like [[type, vkcode, timestamp], [type, vkc, t]]
    """
    new_data = list()
    for i in range(len(data)):
        new_data.append([data[i][0], str(x11_code_to_vkcode()[data[i][2]]), data[i][3]])  # <!> hard coded
    return new_data


def preprocess(data: List[List]) -> List[List]:
    """Compress data, only for X11 agent
    <!> hard code
    :param data: list like [[type, vkcode, timestamp][type, vkcode, timestamp]]
    :return: list like [[vkcode, timestamp1, timestamp2][vkc, t1, t2]]
    """
    new_data = list()
    stack = list()
    for i in range(len(data)):
        if int(data[i][0]) == 2:  # <!> hard coded
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


def process(data: List[List], user_id: int, threshold: int = 3000) -> List[List]:
    """Compute duration between pressed timestamp and release timestamp
    <!> hard code
    :param data: list like [[vkcode, timestamp1, timestamp2], [vkc, t1, t2]]
    :param threshold: timeout
    :return: list like [[vkcode, duration][vkcode, duration]]
    """
    new_data = list()
    for i in range(len(data)):
        if threshold > (int(data[i][2]) - int(data[i][1])) > 16:
            new_data.append(
                categorical_effect(data[i][0]) + [(float(data[i][2]) - float(data[i][1])) / threshold] + [user_id])
    return new_data


def categorical_effect(number: str) -> List[int]:
    return Mapper.vkcode[number]


def process_interkey(data: List[List], threshold: int = 8000) -> List[List]:
    """Compute duration between two keys
    <!> hard code
    :param data: list like [[vkcode, timestamp1, timestamp2], [vkc, t1, t2]]
    :param threshold: timeout between two keys in ms (default: 2000)
    :return: list like [[vkcode0vkcode, duration],[vkcode0vkcode, duration]]
    """

    def format_interkey_name(string: str) -> str:
        if int(string) < 100:
            return "0" + string
        return string

    new_data = list()
    composite_key = [160, 161, 162, 163]
    for i in range(1, len(data)):
        duration = int(data[i][1]) - int(data[i - 1][2])
        last_vkcode = int(data[i][0])
        if 1 < duration < threshold and last_vkcode not in composite_key:
            new_data.append([format_interkey_name(data[i - 1][0]) + format_interkey_name(data[i][0]), duration])
    return new_data


def prepare_x11(basename) -> None:
    """Function to process data from X11 agent
    <!> hard code
    :param basename: basename file (filename must be like "basename"x11.data
    :return: None
    """
    local_data = preprocess(convert_to_vkcode(reading(basename + "x11.data")))
    with open(basename + ".data", 'w', encoding='utf-8', newline='') as file:
        csv_writer = csv.writer(file, delimiter=';')
        for i in range(len(local_data)):
            csv_writer.writerow(local_data[i])


def meta(list_basename: list, user_id: List[int]) -> None:
    """Function to process data (all agents)
    <!> hard code
    :param list_basename: list like [basename1, basename2]
    :return: None
    """
    meta_prepare_mapper(list_basename)
    Mapper.generate_vector()
    for i in range(len(list_basename)):
        local_data = reading(list_basename[i] + ".data")
        DataSorting(process(local_data, user_id[i])).export_csv(list_basename[i])


def meta_prepare_mapper(list_basename: list):
    for i in range(len(list_basename)):
        local_data = reading(list_basename[i] + ".data")
        for j in range(len(local_data)):
            Mapper.add_vkcode(local_data[j][0])

if __name__ == '__main__':
    prepare_x11("remi")
    meta(["alexis", "nicolas", "remi"], [0, 1, 2])  # <!> hard coded
    sys.exit(0)
