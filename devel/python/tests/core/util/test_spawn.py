import os
import stat

import sys

from ert.cwrap import clib, CWrapper
from ert.test.extended_testcase import ExtendedTestCase
from ert.test.test_area import TestAreaContext

test_lib = clib.ert_load("libert_util") # create a local namespace (so we don't overwrite StringList)
cwrapper = CWrapper(test_lib)

spawn = cwrapper.prototype("int util_spawn(char*, int, void*, char*, char*, void*)")

class SpawnTest(ExtendedTestCase):
    def createScript(self, name):
        with open(name, "w") as f:
            f.write("#!/usr/bin/env python\n")
            f.write("import sys\n")
            f.write("sys.stdout.write('stdout\\n')\n")
            f.write("sys.stdout.flush()\n")
            f.write("sys.stderr.write('stderr\\n')\n")
            f.write("sys.stderr.flush()\n")

            mode = os.fstat(f.fileno()).st_mode
            mode |= stat.S_IXUSR | stat.S_IXGRP
            os.fchmod(f.fileno(), stat.S_IMODE(mode))



    def test_spawn(self):

        with TestAreaContext("spawn_test", store_area=True) as test_area:
            self.createScript("print.py")
            print(sys.stdin.fileno(), sys.stdout.fileno(), sys.stderr.fileno())
            status = spawn("print.py", 0, None, "print.out", "print.err", None)
            print(sys.stdin, sys.stdout, sys.stderr)
            print(sys.stdin.fileno(), sys.stdout, sys.stderr)
            print(sys.stdin.closed, sys.stdout, sys.stderr)
            # status = spawn("print.py", 0, None, None, None, None)
            # print(os.strerror(status)) # print error strings from Python error codes
            self.assertEqual(status, 0)
            print("Hello")
            sys.stderr.write("World\n")

            with open("print.out", "r") as f:
                print("From .out: %s" %f.read())

            with open("print.err", "r") as f:
                print("From .err: %s" %f.read())
