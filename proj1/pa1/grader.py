#!/usr/bin/python
__author__  = 'David Menendez'
__version__ = '1'

import os, os.path, sys
import logging, threading, subprocess, itertools, collections

DEFAULT_TIMEOUT = 30

TestCase = collections.namedtuple('TestCase', 'group subpart cmd score')

Score = collections.namedtuple('Score', 'group name score')

NORMAL, EXTRA = range(2)

class Error(Exception):
    pass

class CommandError(Error):
    def __init__(self, cmd, code, out=None):
        self.cmd = cmd
        self.code = code
        self.out = out

def run_command(cmd):
    logging.debug('Running %s', cmd)
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    (out,err)= p.communicate()

    if p.returncode != 0:
        raise CommandError(cmd, p.returncode, out)

def compare_output(code, out, exp_out, exp_code=0):
    if code == -9:
        return 'timeout', []
    if code != exp_code:
        return 'unexpected code ' + str(code), []

    out = out.rstrip()
    outlines = out.split('\n')
    explines = exp_out.split('\n')
    errors = []

    for i,(expl,outl) in enumerate(itertools.izip_longest(explines, outlines),1):
        if outl is None:
            errors += [
                'line ' + str(i),
                '  expected: ' + repr(expl),
                '  received nothing']
        elif expl is None:
            errors += ['extra lines in output']
            break
        elif outl != expl:
            errors += [
                'line ' + str(i),
                '  expected: ' + repr(expl),
                '  received: ' + repr(outl)]

    if errors:
        return 'incorrect output', errors

    return None, []


class Part(object):
    def __init__(self, name, weight=1, timeout=DEFAULT_TIMEOUT):
        self.name = name
        self.weight = weight
        self.timeout = timeout

    def get_tests(self, data_dir, verbosity=0):
        try:
            data_file = os.path.join(data_dir, 'test.txt')

            logging.debug('Opening test file: %r', data_file)

            lines = open(data_file)

            while True:
                arg = next(lines).rstrip()
                exp = next(lines).rstrip()

                yield TestCase(
                    group=NORMAL,
                    subpart=None,
                    cmd=['./' + self.name, arg],
                    score=lambda code,out: compare_output(code, out, exp))

        except StopIteration:
            pass

    def grade(self, src_dir, data_dir, verbosity=0):
        logging.info('Grading %r', self.name)

        # move into subdir
        src_dir = os.path.join(src_dir, self.name)
        if not os.path.isdir(src_dir):
            logging.info('Source dir not found: %r', src_dir)
            return []

        data_dir = os.path.join(data_dir, self.name)
        if not os.path.isdir(data_dir):
            msg = 'Data directory not found: ' + repr(data_dir)
            logging.error(msg)
            raise Error(msg)

        tested = 0
        correct = 0

        try:
            os.chdir(src_dir)
            run_command(['make', 'clean'])
            run_command(['make'])

            prog = os.path.join(src_dir, self.name)

            if not os.path.exists(prog):
                print "{0}: program not found".format(self.name)
                return None

            correct = collections.defaultdict(collections.Counter)

            for test in self.get_tests(data_dir, verbosity):
                tested += 1
                logging.debug('Running test %s (%s %s): %s',
                    tested, test.group, test.subpart, test.cmd)

                p = subprocess.Popen(test.cmd,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT)

                timer = threading.Timer(self.timeout, p.kill)
                try:
                    timer.start()
                    (out,err) = p.communicate()
                finally:
                    timer.cancel()

                logging.debug('Complete. Return code %s. Output:\n%s\n',
                    p.returncode, out)

                (mark, more) = test.score(p.returncode, out)

                if mark or verbosity > 1:
                    print
                    print "{0}: {1}".format(self.name, mark or "correct")
                    print "  called as:", test.cmd
                    for line in more:
                        print " ", line

                if (mark and verbosity) or verbosity > 1:
                    print
                    print "output"
                    print "------"
                    print out
                    print "------"

                correct[test.subpart][test.group] += 0 if mark else 1

            print
            print "{0}: {1} tests complete".format(self.name, tested)

            #logging.info('%s / %s correct', correct, tested)

            return [
                Score(
                    group=group,
                    name=self.name + ':' + subpart if subpart else self.name,
                    score=crrct * self.weight)
                for subpart in sorted(correct.iterkeys())
                for group,crrct in correct[subpart].iteritems()]

        except CommandError as e:
            print
            print "{0}: error running {1.cmd[0]!r} (return code {1.code})".format(self.name, e)
            if len(e.cmd) > 1:
                print "  arguments:", e.cmd[1:]
            if e.out is not None:
                print e.out

            return []

class FilePart(Part):
    def get_tests(self, data_dir, verbosity=0):
        logging.debug('Reading .in files from %r', data_dir)

        ins = []
        outs = set()
        for file in os.listdir(data_dir):
            if not file.startswith('test.'):
                continue

            if file.endswith('.out'):
                outs.add(file[:-4])
            else:
                ins.append(file)

        ins.sort()
        for name in ins:
            if name not in outs: continue
            logging.debug('Test file %r', name)

            name_parts = name.split('.')
            if len(name_parts) > 2:
                subpart = '.'.join(name_parts[1:-1])
            else:
                subpart = None

            group = EXTRA if name_parts[-1].startswith('X') else NORMAL



            arg = os.path.join(data_dir, name)
            exp_file = os.path.join(data_dir, name + '.out')

            def score(code, out):
                try:
                    logging.debug('Opening reference %r', exp_file)
                    exp = open(exp_file).read().rstrip()
                    mark,more = compare_output(code, out, exp)

                    if (mark and verbosity) or verbosity > 1:
                        more += ['', 'input', '-----']
                        more += open(arg).read().split('\n')
                        more.append('-----')

                    return mark,more
                except IOError as e:
                    raise Error('Error reading reference {!r}: {}'.format(
                        exp_file, e.strerror))

            yield TestCase(
                group=group,
                subpart=subpart,
                cmd=['./' + self.name, arg],
                score=score)

def grade(parts, src_dir, data_dir, verbosity):
    print assignment_name, "Auto-Grader, release", __version__

    scores = collections.defaultdict(list)

    try:
        for part in parts:
            for score in part.grade(src_dir, data_dir, verbosity):
                logging.debug('Got score %s', score)
                scores[score.group].append(score)
    except Error as e:
        print "grader:", e
        exit(1)
    except Exception as e:
        logging.exception('Uncaught exception: %s', e)
        print "grader: internal error"
        exit(1)

    total_score = 0
    print
    for score in scores[NORMAL]:
        print "{0.score:5.1f} {0.name}".format(score)
        total_score += score.score

    print '----- -----'
    print "{0:5.1f} Total".format(total_score)

    if EXTRA in scores:
        total_score = 0
        print
        print "Extra credit"
        print
        for score in scores[EXTRA]:
            print "{0.score:5.1f} {0.name}".format(score)
            total_score += score.score

        print '----- -----'
        print '{0:5.1f} Total'.format(total_score)


def grade_archive(parts, arch, data_dir, verbosity):
    """Unpack a Tar archive to a temporary directory.
    """
    import tempfile, shutil

    arch = os.path.realpath(arch)
    logging.debug('Archive path: %r', arch)
    if not os.path.exists(arch):
        print "grader: archive not found:", arch
        exit(1)

    temp_dir = tempfile.mkdtemp()
    try:
        logging.debug('Created temporary directory: %r', temp_dir)

        os.chdir(temp_dir)
        run_command(['tar', 'xf', arch])

        if not os.path.isdir('src'):
            print "grader: Archive does not contain src directory"
            exit(1)

        src_dir = os.path.realpath('src')

        grade(parts, src_dir, data_dir, verbosity)

    finally:
        logging.debug('Deleting temporary directory')
        shutil.rmtree(temp_dir)


assignment_name = 'PA1'
assignment = [
    Part('yell', weight=0.375),
    Part('rle', weight=0.6),
    Part('anagram', weight=0.6),
    FilePart('list', weight=1),
    FilePart('table', weight=1),
    FilePart('mexp', weight=0.6),
    FilePart('sudoku', weight=1),
    FilePart('bst', weight=1.5),
]


def main():
    import argparse, logging.config, tempfile

    argp = argparse.ArgumentParser()
    argp.add_argument('-v', '--verbose', action='count',
        help='Print more output')
    argp.add_argument('-s', '--src', metavar='dir', default='src',
        help='Directory containing program files (default: ./src)')
    argp.add_argument('-a', '--archive', metavar='tar',
        help='Archive containing program files (overrides -s)')
    argp.add_argument('-d', '--debug', action='store_true',
        help='Increase logging')
    argp.add_argument('program', nargs='*',
        help='Name of program to grade')

    args = argp.parse_args()

    h = logging.FileHandler('grader.log', mode='w', delay=True)
    h.setFormatter(logging.Formatter('%(asctime)s %(levelname)-8s %(message)s'))
    logging.getLogger().addHandler(h)

    if args.debug:
        logging.getLogger().setLevel(logging.DEBUG)

    data_dir = os.path.join(sys.path[0], 'data')

    logging.debug('Data directory: %r', data_dir)

    if args.program:
        progs = set(args.program)
        parts = [p for p in assignment if p.name in progs]
    else:
        parts = assignment

    if args.archive:
        grade_archive(parts, args.archive, data_dir, args.verbose)
    else:
        src_dir = os.path.realpath(args.src)

        logging.debug('Source directory: %r', src_dir)

        if not os.path.isdir(src_dir):
            print "grader: invalid src directory:", src_dir
            exit(1)

        grade(parts, src_dir, data_dir, args.verbose)

if __name__ == '__main__':
	main()
