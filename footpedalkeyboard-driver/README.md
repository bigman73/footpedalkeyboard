# Installation steps

1. Install miniconda with Python 3.7+ 
https://conda.io/miniconda.html
> Version 3.7 as of November 2019


2. Create conda environment:
```
conda create --name py37_fpk python=3.7
```

3. Activate conda environment:
Linux: 
```
source activate py37_fpk
```

Windows: 
```
activate py37_fpk
```

4. Upgrade pip:
```
python -m pip install --upgrade pip
```

5. Install the modules *pyserial* and *pyautogui*:
```
pip install pyserial pyautogui
```

6. Install win32gui [windows only]
    1. Download whl: https://www.lfd.uci.edu/~gohlke/pythonlibs/#pywin32

  > For Windows 10 64 bit, Python 3.7 use => pywin32-227-cp37-cp37m-win_amd64.whl

  2. Install the pywin32 whl: 
```
pip install pywin32-227-cp37-cp37m-win_amd64.whl
```
***

# How to run the Foot Pedal Keyboard driver on Windows

1. Change the folder to $(root)\footpedalkeyboard-driver
2. Open command shell (cmd.exe)
```
run.bat
```

> This can be automated with a Windows task that runs on desktop login

# Configure keyboard mapping
Pedal to Keyboard mapping is done using the **key-mappings.json** configuration file.

*pyautogui* module provides support for keyboard automation (programmatic keyboard actions).
The different values are documented in the [reference page](https://pyautogui.readthedocs.io/en/latest/keyboard.html#keyboard-keys)