import sys
from cx_Freeze import setup, Executable
from os import path



if sys.platform == 'win32':
    includefiles = ['addstartup.ps1', 'AddException.bat', 'static/', 'templates/']
    base = 'Win32GUI'
    location = 'build\\win\\Zuum\\'
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
else:
    includefiles = ['static/', 'templates/'] # hi colin add extra shell scripts or smth here
    base = None
    location = 'build/mac/Zuum'
    build_exe_options = {
        "packages": ["os", "sys", "_sysconfigdata__darwin_darwin"], 
        "excludes": ["tkinter", "PyQt4.QtSql",
                    "scipy.lib.lapack.flapack",
                    "PyQt4.QtNetwork",
                    "PyQt4.QtScript",
                    "numpy.core._dotblas",
                    "PyQt5"],
        "includes": ["sqlalchemy", "sqlalchemy.dialects.sqlite", "jinja2.ext", "sqlalchemy.sql.default_comparator", "html.parser"],
        # 'build_exe': location,
        'include_files': includefiles
    }
    bdist_mac_options = {
        "iconfile": path.join('static','img','zmlogo.ico')
    }

setup(  name = "ZUUM",
        version = "0.1",
        description = "Joins meetings on scheduled times",
        options = {
            "build_exe": build_exe_options,
            "bdist_mac": bdist_mac_options
        },
        executables = [
            Executable(
                script="main.py",
                targetName="ZuumManager",
                icon=path.join('static','img','zmlogo.ico'),
                base = base
            ),
            Executable(
                script="taskrunner.py",
                targetName="ZuumScheduler",
                icon=path.join('static','img','zmlogo.ico'),
                base = base
            )
        ])