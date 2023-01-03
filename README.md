# A mini game console operating system kernel prototype
This operating system is imitated from the "mine" operating system made by Tian Yu.I made a simple game console operating system kernel based on it, and added a game I made myself.
## How to compile it?
Under the compiling environment of Linux.
My environment:Centos 7.
## What is the final result of compilation?
"kernel.bin"
## How to use it?
1.Make a USB flash disk for UEFI boot（shell.efi,boot.efi is ready for you."shell.efi" and "boot.efi" is made by Tian Yu.）

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

3.Test on virtual machine
