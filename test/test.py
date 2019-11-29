import _rayMarcher as rm

import unittest

class GradingTest(unittest.TestCase):
    def test_basic(self):
        rm.render(0.0)
        rm.output("tmp.png")