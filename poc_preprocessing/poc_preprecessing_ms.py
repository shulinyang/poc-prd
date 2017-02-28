#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Pre-processing for poc_ann

"""

import sys

from common_functions import writing, reading

"""
On MS Windows
512 = move
513 = left click down
514 = left click up
516 = right click down
517 = right click up
519 = central click down
520 = central click up
522 = roll down/up

512;0; 3368484;1309;661;0
512;0; 3368500;1309;662;0
"""


def filter(data: list) -> tuple:
    """Separate clicks and scroll
    <!> hard code
    :param data: list like [[event type, ...][event type, ...]]
    :return: tuple of lists like ([[event type, ...][event type, ...][[event type, ...]]
    """
    scroll = list()
    click = list()
    for i in range(len(data)):
        event_code = int(data[i][0])
        if event_code == 512:
            pass
        elif event_code == 522:
            scroll.append(data[i])
        else:
            click.append(data[i])
    return scroll, click


def processing_click(click: list) -> list:
    """Compute duration of clicks
    <!> hard code
    :param click: list like [[event type, , , timestamp]]
    :return: list like [[event type, duration]]
    """
    new_click = list()
    stacked = None
    click_down = False
    set_click_down = {513, 516, 519}
    for i in range(len(click)):
        if int(click[i][0]) in set_click_down and not click_down:
            click_down = True
            stacked = click[i]
        elif int(click[i][0]) not in set_click_down and click_down:
            duration = int(click[i][2]) - int(stacked[2])
            if duration > 0:
                new_click.append([stacked[0], duration])
                stacked = None
                click_down = False
    return new_click


def meta(basename: str):
    """
    <!> hard code
    :param basename: Basename (str)
    :return: None
    """
    try:
        _, data = filter(reading(basename + "_clicks.data"))
        writing(basename + "_click_proper.data", processing_click(data))
    except Exception as e:
        print("Something wrong happened.\n")
        print(e.args)


if __name__ == '__main__':
    meta("alexis")
    meta("nicolas")
    sys.exit(0)
