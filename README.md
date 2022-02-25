# Requirements
- OpenCV
- `cmake`

# Installation

## Using `virtualenv`

```bash
$ virtualenv --python=$(which python) .venv
$ source .venv/bin/activate
$ python3 -m pip install -e .
```

The importable python library should be build and the package should be importable via:

```python
import pybind11_example

# do stuff
```

see [`scripts/example.py`](scripts/example.py) as an example
