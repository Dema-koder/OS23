size=$(stat -c "%s" ex1)
nb=$(stat -c "%b" ex1)
sb=$(stat -c "%o" ex1)
permissions=$(ls -l ex1| awk '{print $1}')
echo "Number of blocks = $nb"
echo "Size of each block = $sb"
echo "Total size = $size bytes"
echo "Permissions: $permissions"
echo "The number of links in the ex1 and ex2 files is the same—it is 1. Since they are distinct identifiers that store metadata about each file, the inodes of each file differ from one another."
stat -c "%h - %n" /etc/* | grep ^3
echo "Three links indicates that there are three hard links in these files. A hard link is an extra file link that goes to the same inode as the original file."
