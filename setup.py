import sys
from cx_Freeze import setup, Executable

includefiles = ['addstartup.ps1', 'AddException.bat', 'static/', 'templates/']

build_exe_options = {
    #"packages": ["os", "sys"], 
    "excludes": ["tkinter", "PyQt4.QtSql",
                "scipy.lib.lapack.flapack",
                "PyQt4.QtNetwork",
                "PyQt4.QtScript",
                "numpy.core._dotblas",
                "PyQt5"],
    "includes": ["sqlalchemy", "html.parser", "sqlalchemy.dialects.sqlite", "sqlalchemy.sql.default_comparator", "APSched", "setuptools", "jinja2.ext"],
    'build_exe': 'build\\Zuum\\',
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
                base = "Win32GUI"
            ),
            Executable(
                script="main.py",
                targetName="ZuumManager",
                icon="static\\img\\zmlogo.ico",
                base = "Win32GUI"
            )
        ])