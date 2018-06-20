#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# import matplotlib.pyplot as plt
import numpy as np
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier

from common_functions import reading
# import matplotlib.pyplot as plt
import numpy as np
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier

from common_functions import reading

remi_data = reading("remi-sorted.csv")
nicolas_data = reading("nicolas-sorted.csv")

bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=1),
                         algorithm="SAMME",
                         n_estimators=200)

bdt.fit(np.array([remi_data, nicolas_data]), [124, 124])

plot_colors = "br"
plot_step = 0.02
class_names = "AB"

#plt.figure(figsize=(10, 5))

# Plot the two-class decision scores
twoclass_output = bdt.decision_function(array)
plot_range = (twoclass_output.min(), twoclass_output.max())
#plt.subplot(122)
print(twoclass_output)
print(twoclass_output.shape)
# for i, n, c in zip(range(2), class_names, plot_colors):
#   plt.hist(twoclass_output[i,],bins=10,range=plot_range,facecolor=c, label='Class %s' % n, alpha=.5)
# x1, x2, y1, y2 = plt.axis()
# plt.axis((x1, x2, y1, y2 * 1.2))
# plt.legend(loc='upper right')
# plt.ylabel('Samples')
# plt.xlabel('Score')
#plt.title('Decision Scores')

plt.tight_layout()
plt.subplots_adjust(wspace=0.35)
plt.show()
