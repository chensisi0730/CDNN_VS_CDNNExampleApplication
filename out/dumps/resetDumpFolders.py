import os
from os.path import join as jp
import shutil
# -------------------------------------------------------------------------------------------------
def buildDumpDirTree(basedir):
    os.mkdir(jp(basedir, 'Generation'))
    os.mkdir(jp(basedir, 'Generation', 'in'))
    os.mkdir(jp(basedir, 'Generation', 'in', 'fixed'))
    os.mkdir(jp(basedir, 'Generation', 'in', 'fixedToFloat'))
    os.mkdir(jp(basedir, 'Generation', 'in', 'float'))
    os.mkdir(jp(basedir, 'Generation', 'out'))
    os.mkdir(jp(basedir, 'Generation', 'out', 'fixed'))
    os.mkdir(jp(basedir, 'Generation', 'out', 'float'))
    os.mkdir(jp(basedir, 'Generation', 'out', 'fixedToFloat'))
    os.mkdir(jp(basedir, 'Generation', 'params'))
    os.mkdir(jp(basedir, 'Generation', 'params', 'fixed'))
    os.mkdir(jp(basedir, 'Generation', 'params', 'float'))
    os.mkdir(jp(basedir, 'NoDma'))
    os.mkdir(jp(basedir, 'NoDma', 'in'))
    os.mkdir(jp(basedir, 'NoDma', 'in', 'fixed'))
    os.mkdir(jp(basedir, 'NoDma', 'in', 'fixedToFloat'))
    os.mkdir(jp(basedir, 'NoDma', 'in', 'float'))

    os.mkdir(jp(basedir, 'NoDma', 'out'))
    os.mkdir(jp(basedir, 'NoDma', 'out', 'fixed'))
    os.mkdir(jp(basedir, 'NoDma', 'out', 'fixedToFloat'))
    os.mkdir(jp(basedir, 'NoDma', 'out', 'float'))

# --------------------------------------------------------------------
    os.mkdir(jp(basedir, 'Dma'))

    os.mkdir(jp(basedir, 'Dma', 'in'))
    os.mkdir(jp(basedir, 'Dma', 'in', 'fixed'))
    os.mkdir(jp(basedir, 'Dma', 'in', 'fixedToFloat'))
    os.mkdir(jp(basedir, 'Dma', 'in', 'float'))

    os.mkdir(jp(basedir, 'Dma', 'out'))
    os.mkdir(jp(basedir, 'Dma', 'out', 'fixed'))
    os.mkdir(jp(basedir, 'Dma', 'out', 'fixedToFloat'))
    os.mkdir(jp(basedir, 'Dma', 'out', 'float'))
# -------------------------------------------------------------------------------------------------
baseDumpDir = '.'
DmaDumpDir = './Dma'
NoDmaDumpDir = './NoDma'
GenDumpDir = './Generation'

if os.path.exists(DmaDumpDir):
    shutil.rmtree(DmaDumpDir)

if os.path.exists(NoDmaDumpDir):
    shutil.rmtree(NoDmaDumpDir)

if os.path.exists(GenDumpDir):
    shutil.rmtree(GenDumpDir)

buildDumpDirTree(baseDumpDir)
# -------------------------------------------------------------------------------------------------
