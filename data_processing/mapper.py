#!/usr/bin/env python3
# -*- coding: utf-8 -*-

class Mapper():
    vkcode = dict()

    def __init__(self):
        pass

    @staticmethod
    def add_vkcode(vk: int):
        if vk not in Mapper.vkcode.keys():
            Mapper.vkcode[vk] = None

    @staticmethod
    def generate_vector():
        counter = 0  # type: int
        for key in Mapper.vkcode.keys():
            Mapper.vkcode[key] = [0] * counter + [1] + [0] * (len(Mapper.vkcode.keys()) - counter)
            counter += 1
