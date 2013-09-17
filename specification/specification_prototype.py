#!/usr/bin/env python
 
import hashlib
import time
 
def create_md5(xor):
    hashs = hashlib.md5()
    hashs.update(xor)
    global ownhash
    ownhash = hashs.hexdigest()
    print 'hash: '+ownhash
   
   
def main(cl):
    acctime = time.time()
    #acctime = int(acctime)   #<-- on/off liczby calkowite
    targettime = cl
    count = 1
    while acctime > targettime:
        create_md5(str(acctime))
        acctime = acctime -0.01 #co ile ma odejmowac
        print 'nr %s' % count
        print 'for time: '+str(acctime)
        print '----------------'
        count += 1
        if ownhash == '10a117580ae889c04bc3d2b5e5fcddd9': #tutaj hash
            print 'found! ==> '+ownhash
           
            break
        else:
            continue
       
main(1379033993.02)
