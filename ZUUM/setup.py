import sys
from cx_Freeze import setup, Executable



if sys.platform == 'win32':
    includefiles = ['addstartup.ps1', 'AddException.bat', 'static/', 'templates/']
    base = 'Win32GUI'
    location = 'build\\win\\Zuum\\'
else:
    includefiles = ['static/', 'templates/'] # hi colin add extra shell scripts or smth here
    base = None
    location = 'build/mac/Zuum'


build_exe_options = {
    "packages": ["os", "sys"], 
    "excludes": ["tkinter", "PyQt4.QtSql",
                "scipy.lib.lapack.flapack",
                "PyQt4.QtNetwork",
                "PyQt4.QtScript",
                "numpy.core._dotblas",
                "PyQt5"],
    "includes": ["sqlalchemy", "html.parser", "sqlalchemy.dialects.sqlite", "sqlalchemy.sql.default_comparator", "APSched", "setuptools", "jinja2.ext"],
    'build_exe': location,
    'include_files': includefiles
}


setup(  name = "ZUUM",
        version = "0.1",
        description = "Joins meetings on scheduled times",
        options = {"build_exe": build_exe_options},
        executables = [
            Executable(
                script="taskrunner.py",
                targetName="ZuumScheduler",
                icon="static\\img\\zmlogo.ico",
                base = base
            ),
            Executable(
                script="main.py",
                targetName="ZuumManager",
                icon="static\\img\\zmlogo.ico",
                base = base
            )
        ])