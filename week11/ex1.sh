#!/bin/bash

# Obtain the device path loop
n=$(sudo losetup -a | grep lofs.img | cut -d ' ' -f 1 | tr -d:)

# Create a file lofs.img
sudo dd if=/dev/zero of=lofs.img bs=1M count=55

# Link the disk image to the loop device
sudo losetup -fP lofs.img

#Make the loop device's filesystem an ext4 filesystem
sudo mkfs.ext4 $n

# Create a directory to mount the loop device
mkdir ./lofsdisk

# Mount the loop device to the created directory
sudo mount -o loop /dev/$n ./lofsdisk

# Add file1 to the LOFS
sudo touch ./lofsdisk/file1

# Write into the file1
echo "Demyan" | sudo tee ./lofsdisk/file1

# Add file1 to the LOFS
sudo touch ./lofsdisk/file2

# Write into the file2
echo "Zverev" | sudo tee ./lofsdisk/file2

# Сreating a folder where there will be functions
sudo mkdir -p ./lofsdisk/bin

# Function to add to the chroot environment the libraries needed for a given command
add_libs() {
  # Find path by argument
  path=$(which "$1")
  # Retrieve the required libraries for a given binary executable  
  libs=$(ldd "$path" | awk '/=>/ {print $3}')
  # Copy required libraries to the chroot environment
  sudo cp --parents $libs ./lofsdisk/
  # Copy the command to the ./lofsdisk/bin
  sudo cp "$path" ./lofsdisk/bin
}

# Adding a library ls in the chroot environment
add_libs "ls"
# Adding a library echo in the chroot environment
add_libs "echo"
# Adding a library bash in the chroot environment
add_libs "bash"
# Adding a library cat in the chroot environment
add_libs "cat"

# In the chroot environment, create a directory structure that resembles /lib64
sudo mkdir -p ./lofsdisk/lib64
# In the chroot environment, create a directory structure that resembles /lib
sudo mkdir -p ./lofsdisk/lib

# Mount the real /lib64 directoriy into the chroot environment by using the Bind command.
sudo mount --bind /lib64 ./lofsdisk/lib64
# Mount the real /lib directory into the chroot environment by using the Bind command.
sudo mount --bind /lib ./lofsdisk/lib

# Compile ex1.c 
gcc ex1.c -o ex1
# Move the compiled executable to the chroot environment
sudo mv ex1 ./lofsdisk

# Run ex1 in the chroot environment and save output to the ex1.txt
sudo chroot ./lofsdisk /bin/bash -c "./ex1 > /ex1.txt"

# Compile ex1.c outside the chroot environment
gcc ex1.c -o ex1

# Run ex1 
run_ex1=$(./ex1)
# Save output to the ex1.txt
sudo echo "$run_ex1" >> ./lofsdisk/ex1.txt
