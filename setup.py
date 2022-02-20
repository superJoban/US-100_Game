from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

ext_modules = [
    Extension("ultrasonic",
                sources = ["ultrasonicwrapper.pyx", 'ultrasonic.c'],
                include_dirs = ["/opt/vc/include"],
                libraries = ["bcm_host"],
                library_dirs = ["/opt/vc/lib"]
    )
]

setup(
    name = "ultrasonic",
    ext_modules = cythonize(ext_modules)
)

