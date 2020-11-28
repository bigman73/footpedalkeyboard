@ECHO OFF
TITLE Foot Pedal Keyboard Driver
COLOR 17

ECHO Activating Conda environment: py37_fpk
call activate py37_fpk

ECHO Executing the FPK Python Driver
python driver.py