#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import csv
import random
from typing import List, NoReturn, Tuple


class DataManager():
    def __init__(self):
        self.mapper = dict()  # type: dict

    def mapper_scan(self, dataset: List[List]) -> NoReturn:
        for i in dataset:
            for j in i:
                self._add_vkcode(int(j[0]))

    def _add_vkcode(self, vk: int) -> NoReturn:
        if vk not in self.mapper.keys():
            self.mapper[vk] = None

    def _generate_vector(self) -> NoReturn:
        if len(self.mapper.keys()) == 0:
            raise Exception
        counter = 0  # type: int
        for key in self.mapper.keys():
            self.mapper[key] = [0] * counter + [1] + [0] * (len(self.mapper.keys()) - counter) + [0]
            counter += 1

    def shuffle_data(self, dataset: List) -> List:
        data = dataset.copy()
        for i in range(int(len(data) / 2)):
            swap_idx = int(random.randint(0, 65556) % len(data))
            if i != swap_idx:
                tmp = data[swap_idx]
                data[swap_idx] = data[i]
                data[i] = tmp
        return data

    def export_csv(self, filename: str, dataset: List) -> None:
        with open(filename + ".csv", "w", encoding='utf8', newline='') as csv_file:
            csv_writer = csv.writer(csv_file, delimiter=';', quotechar='|', quoting=csv.QUOTE_MINIMAL,
                                    dialect='excel')
            # for key in self.sorted_data.keys():
            #    csv_writer.writerow([key] + self.sorted_data[key])
            for key in dataset:
                csv_writer.writerow(key)
        print(filename + " as been exported.")

    def process_key(self, dataset1: List, dataset2: List, training_percent: float = 0.80) -> Tuple[
        List[List], List[List]]:
        def _inner_process(self: DataManager, dataset: List[List], user_id: int) -> List[List]:
            data = list()
            for i in range(len(dataset)):
                if 3000 > (int(dataset[i][2]) - int(dataset[i][1])) > 16:
                    data.append(
                        self.mapper[dataset[i][0]] + [(float(dataset[i][2]) - float(dataset[i][1])) / 3000] + [user_id])
            return data

        data1 = _inner_process(self, dataset1, 0)  # type: List[List]
        data2 = _inner_process(self, dataset2, 1)  # type: List[List]
        final_length = int(min(len(data1), len(data2)) * training_percent)  # type: int
        train_dataset = data1[:final_length] + data2[:final_length]
        test_dataset = data1[final_length:] + data2[final_length:]
        return train_dataset, test_dataset

    def process_inter(self, dataset1: List, dataset2, training_percent: float = 0.80) -> Tuple[List, List]:
        def _inner_process_interkey(self: DataManager, dataset: List[List], user_id: int) -> List[List]:
            def formating_interkey(vector1: List[int], vector2: List[int]):
                data = [x1 | x2 for x1, x2 in (vector1, vector2)]
                data[len(data) - 1] = 1
                return data

            new_data = list()
            composite_key = [160, 161, 162, 163]
            for i in range(1, len(dataset)):
                duration = int(dataset[i][1]) - int(dataset[i - 1][2])
                last_vkcode = int(dataset[i][0])
                if 10 < duration < 8000 and last_vkcode not in composite_key:
                    new_data.append(
                        formating_interkey(self.mapper[dataset[i - 1][0]], self.mapper[dataset[i][0]]) + [duration,
                                                                                                          user_id])
            return new_data

        data1 = _inner_process_interkey(self, dataset1, 0)  # type: List[List]
        data2 = _inner_process_interkey(self, dataset2, 1)  # type: List[List]
        final_length = int(min(len(data1), len(data2)) * training_percent)  # type: int
        train_dataset = data1[:final_length] + data2[:final_length]
        test_dataset = data1[final_length:] + data2[final_length:]
        return train_dataset, test_dataset

    def batch(self, dataset1: List, dataset2: List, output_filename):
        self.mapper_scan(dataset1)
        self.mapper_scan(dataset2)
        self._generate_vector()
        train_inter, test_inter = self.process_inter(dataset1, dataset2)
        train, test = self.process_key(self.shuffle_data(dataset1), self.shuffle_data(dataset2))
        self.export_csv(output_filename + "-train", train + train_inter)
        self.export_csv(output_filename + "test", test + test_inter)
