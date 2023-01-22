# Tutorial

## Importing the Toumou Python bindings

There is currently no package for the Toumou Python bindings, therefore to use it you need to add the installation folder to your Python path: 
```python
import sys
sys.path.append('/Toumou/installation/folder')
```

Once this is done, simply import the `toumou` module: 
```python
import toumou as tm
print(tm.__doc__)
```

If you get an `ImportError: DLL load failed while importing toumou`, you might need to provide the path to the IMath, OpenEXR and zlib DLLs to Python:
```python
import os
os.add_dll_directory('/path/to/Imath/DLLs')
os.add_dll_directory('/path/to/OpenEXR/DLLs')
os.add_dll_directory('/path/to/zlib/DLLs')
```
