# A mini game console operating system kernel prototype
This operating system imitates the "MINE" operating system produced by Tian Yu, the author of "The Design and Implementation of a 64-bit Operating System". I made a simple game console operating system kernel based on it, and built in a game I made myself.
This operating system currently only supports running on virtual machines, and more advanced features are still under development.
## How to compile it?
Under the compiling environment of Linux.  
My environment:Centos 7.  
Enter "make" in the console and execute

## What is the final target file for compilation?
"kernel.bin"

## How to run it?
1.We need to make a UEFI bootable USB flash drive（shell.efi,boot.efi are already prepared for you.("shell.efi" and "boot.efi" is made by Tian Yu.）

2.Insert the USB flash disk into the system and type the following command on the console(Suppose these files are on your desktop:shell.efi,boot.efi,kernel.bin):

command1:  
mount /dev/sdb1 /mnt/  
mkdir -p /mnt/EFI/BOOT  
cp Desktop/Shell.efi /mnt/EFI/BOOT/BOOTx64.EFI  
sync  
umount /mnt/  

command2:  
mount /dev/sdb1 /mnt/  
mkdir -p /mnt/EFI/BOOT  
cp Desktop/boot.efi /mnt/  
cp Desktop/kernel.bin /mnt/  
sync  
umount /mnt/  


3. Test on virtual machine
