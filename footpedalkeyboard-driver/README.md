# Python installation requirements

1. Install miniconda with Python 3.6+ 
https://conda.io/miniconda.html
> Version 3.6.6 as of Aug 2018


2. Create conda environment:
```
conda create --name py36_fpk python=3.6
```

3. Activate conda environment:
Linux: 
```
source activate py36_fpk
```

Windows: 
```
activate py36_fpk
```

4. Upgrade pip:
```
pip python -m pip install --upgrade pip
```

5. Install the module pyserial:
```
pip install pyserial
```

6. Install the module pyautogui:
```
pip install pyautogui
```

7. Install win32gui [windows only]
    1. Download whl: https://www.lfd.uci.edu/~gohlke/pythonlibs/#pywin32

  > For Windows 10 64 bit, Python 3.6 use => pywin32-223.1-cp36-cp36m-win_amd64.whl

  2. Install whl: 
```
pip install pywin32-223.1-cp36-cp36m-win_amd64.whl
```