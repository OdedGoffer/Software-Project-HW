from setuptools import setup, Extension

module1 = Extension('kmeans',
                    sources = ['kmeans.c'])

setup (name = 'kmeans',
       version = '1.0',
       description = 'Implementation of kmeans++ algorithim in Python using C',
       ext_modules = [module1])
