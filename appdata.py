APPNAME = "ZUUM"
import sys
from os import path, environ, makedirs
if sys.platform == 'darwin':
    from AppKit import NSSearchPathForDirectoriesInDomains
    # http://developer.apple.com/DOCUMENTATION/Cocoa/Reference/Foundation/Miscellaneous/Foundation_Functions/Reference/reference.html#//apple_ref/c/func/NSSearchPathForDirectoriesInDomains
    # NSApplicationSupportDirectory = 14
    # NSUserDomainMask = 1
    # True for expanding the tilde into a fully qualified path
    ad = path.join(NSSearchPathForDirectoriesInDomains(14, 1, True)[0], APPNAME)
elif sys.platform == 'win32':
    ad = path.join(environ['APPDATA'], APPNAME)
else:
    ad = path.expanduser(path.join("~", "." + APPNAME))

appdata = ad

if not path.exists(appdata):
    makedirs(appdata)

