from os import listdir
from setuptools import setup, Extension


src = listdir('../src')
sources = ['../src/' + f for f in src]
sources.append('module.c')

module = Extension('spkmeans',
                   sources=sources)

setup(name='spkmeans',
      version='1.0.0',
      description='Implementation Normalized Spectral Clustering K-means algorithm and associated methods.',
      ext_modules=[module])
