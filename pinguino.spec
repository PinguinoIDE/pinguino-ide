# -*- mode: python -*-

a = Analysis(['pinguino.py'],
             pathex=['qtgui/resources', '.'],
             hiddenimports=[],
             hookspath=None,
             runtime_hooks=None)
             
pyz = PYZ(a.pure)

exe = EXE(pyz,
          a.scripts,
          a.binaries,
          a.zipfiles,
          a.datas,
          [('paths.cfg', 'paths.cfg', 'DATA')],
          name='pinguino',
          debug=False,
          strip=None,
          upx=True,
          console=True
          )
          
          
coll = COLLECT([('paths.cfg', 'paths.cfg', 'DATA')],
               Tree('qtgui/config',"qtgui/config"),
               name='pinguino_data',
               strip=None,
               upx=True,
               )
