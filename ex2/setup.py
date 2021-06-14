from setuptools import setup, Extension

module1 = Extension('kmeanssp',
                    sources = ['kmeans.c'])

setup (name = 'kmeanssp',
       version = '1.0',
       description = 'Implementation of kmeans++ algorithim in Python using C',
       ext_modules = [module1])
