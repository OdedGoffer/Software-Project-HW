from os import listdir
from setuptools import setup, Extension

sources = ['spkmeans.c', 'spkmeansmodule.c']

module = Extension('spkmeans',
                   sources=sources)

setup(name='spkmeans',
      version='1.0.0',
      description='Implementation Normalized Spectral Clustering K-means algorithm and associated methods.',
      ext_modules=[module])
