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
from data_manager import DataManager
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


if __name__ == '__main__':
    # prepare_x11("remi")
    dm = DataManager()
    dm.mapper_scan(reading("nicolas.data"))
    dm.batch(reading("alexis.data"), reading("remi.data"), "alexis-remi")
    dm.batch(reading("alexis.data"), reading("nicolas.data"), "alexis-nicolas")
    dm.batch(reading("remi.data"), reading("nicolas.data"), "remi-nicolas")
    sys.exit(0)
