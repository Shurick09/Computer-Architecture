#!/usr/bin/python
import autograde
import os, os.path

assignment_name = 'PA5'
release='1'


# trace.<trace>.txt
# test.<policy>.<trace>.<cache>.<assoc>.<block>.txt

class CSTests(autograde.AbstractTestGroup):
    def get_tests(self, prog, data_dir):
        prefix = 'test.' + self.id + '.'
        ref_suffix = '.txt'

        # gather the names of the reference files
        fnames = [fname for fname in os.listdir(data_dir)
                    if fname.startswith(prefix)
                    and fname.endswith(ref_suffix)]
        fnames.sort()

        # for each reference name, determine the corresponding trace file
        for ref_name in fnames:
            id = ref_name[len(prefix):-len(ref_suffix)]
            parts = id.split('.')

            trace = os.path.join(data_dir,
                'trace' + parts[0] + '.txt')

            if not os.path.exists(trace):
                autograde.logger.warning('Unmatched reference file: %r', ref_name)
                continue

            ref = os.path.join(data_dir, ref_name)

            yield autograde.FileRefTest(
                cmd = [prog, parts[1], parts[2], self.name, parts[3], trace],
                ref_file = ref,
            )

assignment = autograde.Project('cachesim',
    CSTests(id='fifo'),
    CSTests(id='lru', category=autograde.EXTRA),
)



if __name__ == '__main__':
    autograde.set_asan_symbolizer()
    autograde.main(assignment_name, assignment, release)
