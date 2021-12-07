# CS 170 Project Fall 2021 (Group: Student_solutions)

## Requirements:

Python 3.6+

```
pip install cython click
```

## Files

- `_sa.pxd/pyx`: cython binding of `sa.cpp/h`
- `dp.py`: dp solver for special cases
- `parse.py`: functions to read/write inputs and outputs
- `solver.py`: where you should be writing your code to solve inputs
- `prepare_submission`: generate submission.json
- `random_choice`: brute-force
- `SA_solver.py`: where simulated annealing is used
- `sa.cpp/h`: cpp version of simulated anealing 
- `setup.py`: cython setup
- `solver.py`: main entry
- `Task.py`: contains a class that is useful for processing inputs

When writing inputs/outputs:
- Make sure you use the functions `write_input_file` and `write_output_file` provided
- Run the functions `read_input_file` and `read_output_file` to validate your files before submitting!
- These are the functions run by the autograder to validate submissions

## Build cpp

Windows:
```
python setup.py build_ext --inplace
```

Linux:
```
make
```

## Usage

```sh
python solver.py --help
# Usage: solver.py [OPTIONS] CASE
# 
#   CASE format are [s/m/l]+number, e.g. l1; all for solve all
# 
# Options:
#   -s, --solver [sapy|dp|sa]
#   -e, --eval
#   -f, --force-replace
#   --help                     Show this message and exit.
```
