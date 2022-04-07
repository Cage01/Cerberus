from cmath import inf
import sys
import os
from glob import iglob
import ntpath

def replaceAll(infilepath):
    k = infilepath.rfind("Lyra")
    outfilepath = infilepath[:k] + "Cerberus" + infilepath[k+4:]
    if 'Lyra' not in ntpath.basename(infilepath):
        outfilepath="tmp"
        #k = infilepath.rfind("Lyra")
        #outfilepath = infilepath[:k] + "Cerberus" + infilepath[k+1:]
        #outfilepath=infilepath[::-1].replace("Lyra","Cerberus",1)[::-1]
        #outfilepath = "Cerberus".join(infilepath.rsplit('Lyra', 2))
        #outfilepath=infilepath.replace('Lyra', 'Cerberus')
        

    try:
        with open(infilepath, "r") as infile, open(outfilepath, 'w') as outfile:
            data=infile.read().replace('Lyra', 'Cerberus')
            outfile.write(data)
            # for line in infile:
            #     if "Lyra" in line:
            #         infile.write(line.replace('Lyra', 'Cerberus'))
    
    except:
        print("Could not update " + infilepath)

    if 'tmp' in outfilepath:
        os.replace(outfilepath, infilepath)
    elif 'Cerberus' in outfilepath:
        os.remove(infilepath)

if __name__ == '__main__':
    canRun=False
    
    if canRun:
        rootdir_glob = sys.argv[1] + '/**/*'
        # This will return absolute paths
        file_list = [f for f in iglob(rootdir_glob, recursive=True) if os.path.isfile(f)]
        for file in file_list:
            replaceAll(file)
    else:
        print("Unable to run script")

    sys.exit()