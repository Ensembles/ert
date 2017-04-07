from cwrap import Stream
from ert.test import ExtendedTestCase
from ert.test.test_area import TestAreaContext

import numpy as np


class StreamTest(ExtendedTestCase):

    def test_init(self):
        with TestAreaContext("stream_tests") as test_area:
            with open('outintest', 'w') as f:
                f.write('dag og tid\nnatt og dag\n')
            s = Stream('outintest', 'r')
            self.assertEqual('dag og tid\n', s.readline())

    def test_write(self):
        with TestAreaContext("stream_tests") as test_area:
            out = Stream('writetest', 'w')
            out.write('one two three')
            out.close()
            inp = Stream('writetest', 'r')
            self.assertEqual('one tw', inp.read(6))

            with self.assertRaises(IOError):
                inp.write('stream is not writable')

            with self.assertRaises(IOError):
                out.write('stream is closed!')

    def test_closed(self):
        with TestAreaContext("stream_tests_closed1") as test_area:
            out = Stream('closetest', 'w')
            self.assertFalse(out.closed)
            out.close()
            self.assertTrue(out.closed)
            out.close()
            self.assertTrue(out.closed)
        with TestAreaContext("stream_tests_closed2") as test_area:
            with Stream('closetest2', 'w') as s:
                self.assertFalse(s.closed)
            self.assertTrue(s.closed)

    def test_nosuchfile(self):
        with TestAreaContext("stream_tests_nsf") as test_area:
            with self.assertRaises(IOError):
                Stream('nosuchfile.abc', 'r')

    def test_readlines(self):
        with TestAreaContext("stream_tests_rls") as test_area:
            out = Stream('writelinetest', 'w')
            out.write('myline1\nandline2\nfinalline3\n')
            out.close()
            inp = Stream('writelinetest', 'r')
            self.assertEqual('myline1\n', inp.readline())
            self.assertEqual('andline2\n', inp.readline())
            self.assertEqual('finalline3\n', inp.readline())
            self.assertEqual('', inp.readline())
            self.assertEqual('', inp.readline())
            self.assertEqual('', inp.readline())


    def test_yield(self):
        cnt = ['line 1\n', 'LINE 2\n', 'finalline\n']
        with TestAreaContext("stream_tests_yield") as test_area:
            with open('writeyield', 'w') as f:
                for x in cnt:
                    f.write('%s' % x)

            inp = Stream('writeyield', 'r')
            c = 0
            for line in inp:
                self.assertEqual(cnt[c], line)
                c += 1
            self.assertFalse(inp.closed)
            inp.close()
            self.assertTrue(inp.closed)

    def test_with_file_stream(self):
        with TestAreaContext("stream_tests_with_fs") as test_area:
            with open('writewith', 'w') as s:
                s.write('testing ```with``` syntax')
            with Stream('writewith', 'r') as s:
                l = s.readline()
                self.assertEqual('testing ```with``` syntax', l)

    def test_with_stream_file(self):
        with TestAreaContext("stream_tests_with_sf") as test_area:
            with Stream('writewith', 'w') as s:
                s.write('testing ```with``` syntax')
            with open('writewith', 'r') as s:
                l = s.readline()
                self.assertEqual('testing ```with``` syntax', l)
                l = s.readline()
                self.assertEqual('', l)

    def test_bytes(self):
        def generate_data(size = 100):
            san = lambda x: max(0, min(127, abs(x)))
            data = np.zeros(size, dtype=np.byte)
            for i in range(size):
                data[i] = san(127-i)
            return data

        with TestAreaContext("stream_tests") as test_area:
            data = generate_data()
            read = None

            with Stream('writebytes', 'wb') as s_out:
                w_ret = s_out.write(data)
                self.assertEqual(w_ret, 100)
                self.assertFalse(s_out.closed)
            self.assertTrue(s_out.closed)

            with Stream('writebytes', 'rb') as s_in:
                read = s_in.read()

            for i in range(100):
                self.assertEqual(data[i], read[i])
            self.assertEqual(100, len(read))
            self.assertEqual(len(data), len(read))

    def test_contextmanager_exception(self):
        # testing that exceptions in with still closes the file
        with TestAreaContext("test_contextmanager_exception") as test_area:
            fname = 'expectexcept'
            with self.assertRaises(IOError):
                with Stream(fname, 'w') as s:
                    s.write('yes!\n')
                    raise IOError('bloody hell!')
            self.assertTrue(s.closed)
            content = ''
            with Stream(fname, 'r') as inp:
                content = inp.readline()
            self.assertEquals('yes!\n', content)
