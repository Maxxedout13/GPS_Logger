import pylab
import unittest









class TestLibs(unittest.TestCase):


    @unittest.skip('')
    def test_plot(self):
        pylab.ion()
        pylab.figure()
        input()


if __name__ == '__main__':
    unittest.main()











