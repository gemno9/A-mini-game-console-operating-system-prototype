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
We need to make a UEFI bootable USB flash drive（"shell.efi","boot.efi" are already prepared for you."shell.efi" and "boot.efi" is made by Tian Yu.）

## How to make a UEFI bootable USB flash drive?
1.Use tools to clear all data in the sector of the U disk, and then convert the partition table type to GUID format.  
2.Create a new ESP partition (20MB) in the U disk and align it to an integer multiple of 4096 sectors.  
3.ESP partition file system select "FAT32 file" file system, 10MB is enough.  
4.Use the following console command to put the UEFI program into the U disk under the Linux system(Suppose these files are on your desktop:shell.efi,boot.efi,kernel.bin):

mount /dev/sdb1 /mnt/  
mkdir -p /mnt/EFI/BOOT  
cp Desktop/Shell.efi /mnt/EFI/BOOT/BOOTx64.EFI  
cp Desktop/boot.efi /mnt/  
cp Desktop/kernel.bin /mnt/  
sync  
umount /mnt/  

## Test on virtual machine
1.Enter the BIOS of the virtual machine, and adjust the boot priority of the U disk to the highest.  
2.After entering the shell, enter "boot (the name of the bootloader)" to enter the operating system.  
3.enjoy the game

##How to set up the virtual machine?
1.I use "VMware Workstation 15" as a test environment(You need to set it to "UEFI Boot Mode").  
2.I have prepared a virtual machine file for you (it stores a snapshot, you can load the snapshot to experience the system), decompress the "mini game system_vmdk.rar" and use "VM15" to open the virtual machine file inside and you can use it .

