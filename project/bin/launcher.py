#! /usr/bin/python3
# launcher.py - запуск транспилятора и сборщика, очистка промежуточных хранилищ.

#------------------------------------------------------------------------------
import os
import glob
import shutil

#------------------------------------------------------------------------------
# Каталог для размещения файлов на EO, полученных в ходе транспиляции и сборки
# Указан относительно текущего каталога
resultDir = "../../result/eo/c2eo/src/"

#------------------------------------------------------------------------------
# Каталог в котором формируется файл global.eo для последующей пересылки в
# каталог проекта на EO
assemblyDir = "../assembly/"

#------------------------------------------------------------------------------
if __name__ == '__main__':
    print("Test out")
    # Получение текущего каталога
    print(f'Current Working Directory is: {os.getcwd()}')
    # Проверка наличия нужного каталога
    if os.path.exists(resultDir):
        print(f'Resul Directory is: {resultDir}')
    # Проверка наличия файла в каталоге
    if os.path.exists(os.path.join(resultDir, 'README.md')):
        print(f'File README.md is in: {resultDir}')
    else:
        print(f'File README.md is absence in: {resultDir}')
    # Получение содержимого каталога
    print(f'Directory {resultDir} contain: {os.listdir(resultDir)}')
    print(f'Directory {assemblyDir} contain: {os.listdir(assemblyDir)}')
    #globFileList = glob.globlist(glob(os.path.join('sample_data', '*.csv')))
    resultFileList = list(glob.glob(os.path.join(resultDir, '*.eo')))
    print(f'Directory {resultDir} contain: {resultFileList}')
    assemlyFileList = list(glob.glob(os.path.join(assemblyDir, '*.c')))
    print(f'Directory {assemblyDir} contain: {assemlyFileList}')

    #for file in list(glob.glob(os.path.join(assemblyDir, '*.c'))):
    #    shutil.move(file, resultDir)

    # Удаление всех файлов, находящихся в каталоге resultDir
    for file in list(glob.glob(os.path.join(resultDir, '*'))):
        os.remove(file)

    # Проверка наличия файла в каталоге
    if os.path.exists(os.path.join(assemblyDir, 'global.eo')):
        shutil.move(os.path.join(assemblyDir, 'global.eo'), resultDir)
    else:
        print(f'File global.eo is absence in: {assemblyDir}')

    # Удаление всех файлов, находящихся в каталоге assemblyDir
    #for file in list(glob.glob(os.path.join(assemblyDir, '*'))):
    #    os.remove(file)

