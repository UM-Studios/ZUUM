# -*- mode: python ; coding: utf-8 -*-

block_cipher = None


a = Analysis(['ZUUM.py'],
             pathex=['/Users/colinzhao/src/ZUUM/SchoolZoomGUIMac/files'],
             binaries=[],
             datas=[],
             hiddenimports=['tkinter'],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher,
             noarchive=False)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          [],
          name='ZUUM',
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=True,
          upx_exclude=[],
          runtime_tmpdir=None,
          console=False , icon='zmlogo.icns')
app = BUNDLE(exe,
             name='ZUUM.app',
             icon='zmlogo.icns',
             bundle_identifier=None)
