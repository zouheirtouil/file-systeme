# file-systeme

The aim of the Advanced Data Structure project is to implement a simple hierarchical file system with implement a simple hierarchical file system with storage 
only in main memory. 
A hierarchical file system  hierarchical file system organises resources in a tree structure and uniquely identifies each resource by the path that connects it to the root. 
The resources contained in a hierarchical file system can be files or hierarchical file system can be files or directories. Both have a 
name, represented by a string. For the former, it is only possible to insert as leaves of the tree, while the latter can appear as both 
the latter can appear both as leaves and as intermediate nodes. The root of the tree is usually a directory, called root directory. Only file nodes can contain data, 
represented as a sequence of bytes, while directories have no associated data. directories have no associated data. All nodes in the tree structure can 
contain metadata, but for the purposes of this project, only directories contain directories contain it. The directory metadata are the names of 
its direct descendants.
The program implementing the file system will receive a log of the actions to be performed from the standard input and will print the 
result of the same on the standard output. The program must be implemented in standard C. The operation of the program requires 
that it reads a line from the action log, and executes the corresponding on the internal representation of the file system it is managing, and write the result to the 
and write the result to the standard output before moving on to the next action (the execution of the actions is completely sequential).


<img src="https://github.com/zouheirtouil/file-systeme">
