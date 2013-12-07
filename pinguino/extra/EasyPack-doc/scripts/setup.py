# -*- coding: utf-8 -*-
#
# setup.py

from distutils.core import setup
import py2exe
manifest = """
 <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
 <assembly xmlns="urn:schemas-microsoft-com:asm.v1"
 manifestVersion="1.0">
 <assemblyIdentity
     version="0.64.1.0"
     processorArchitecture="x86"
     name="Controls"
     type="win32"
 />
 <description>Your Application</description>
 <dependency>
     <dependentAssembly>
         <assemblyIdentity
             type="win32"
             name="Microsoft.Windows.Common-Controls"
             version="6.0.0.0"
             processorArchitecture="X86"
             publicKeyToken="6595b64144ccf1df"
             language="*"
         />
     </dependentAssembly>
 </dependency>
 </assembly>
"""
py2exe_options = {
  'compressed': 1,
  'optimize': 1,
  'includes':  [],
  'excludes': [],
  'dll_excludes': [],
  'packages': [],
  'bundle_files': 3
  }
setup(
        options={'py2exe': py2exe_options},
        #windows=[
        console=[
            {
                "script" : "pinguino.py",
                "icon_resources": [(1, "pinguino.ico")]
                #"other_resources": [(24,1,manifest)],
             },
        ],
      data_files=[("",["pinguino.ico"])]
#       zipfile = None
)


"""
Might be needed:
vcredist_x86.exe
Microsoft Visual C++ 2008 SP1 Redistributable Package (x86)
http://www.microsoft.com/en-us/download/details.aspx?id=5582&WT.mc_id=MSCOM_EN_US_DLC_DETAILS_131Z4ENUS22004
"""

# by avrin, 2013/02

