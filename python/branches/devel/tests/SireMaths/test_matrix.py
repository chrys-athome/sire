
from Sire.Maths import *

from nose.tools import assert_equal

def test_matrix_multiplication(verbose = False):

    m = Matrix( 1, 2, 3, \
                4, 5, 6, \
                7, 8, 9 )

    v = Vector( 1, \
                2, \
                3 )

    expected_result = Vector( 14, \
                              32, \
                              50 )

    result = m * v

    if verbose:
        print("\nTesting matrix multiplication")
        print("%s\ntimes\n%s\nequals\n%s" % (m,v,result))
        print("Expect result equals\n%s" % expected_result)

    assert_equal(expected_result, result)


def test_matrix_inverse(verbose = False):

    m = Matrix( 1, 2, 3, \
                4, 5, 6, \
                7, 8, 9 )

    expected_result = Matrix( 1, 4, 7, 
                              2, 5, 8,
                              3, 6, 9 )

    result = m.transpose()

    if verbose:
        print("\nTesting matrix.transpose()")
        print("%s.transpose()\nequals\n%s" % (m,result))
        print("Expect result equals\n%s" % expected_result)

    assert_equal(expected_result, result)


if __name__ == "__main__":
    test_matrix_multiplication(True)
    test_matrix_inverse(True)

