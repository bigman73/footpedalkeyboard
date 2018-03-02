@ECHO OFF
TITLE Foot Pedal Keyboard Driver
COLOR 17

ECHO Activating Conda environment: py36_fpk
call activate py36_fpk

ECHO Executing the FPK Python Driver
python foodpedalkeyboard-driver.py