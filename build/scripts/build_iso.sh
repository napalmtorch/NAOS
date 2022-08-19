mkdir "bin/iso"
mkdir "bin/iso/boot"
mkdir "bin/iso/boot/grub"

cp 'bin/kernel.bin' 'bin/iso/boot/kernel.bin'
cp 'build/grub.cfg' 'bin/iso/boot/grub/grub.cfg'
grub-mkrescue -o 'naos.iso' 'bin/iso'


cp 'naos.iso' 'naos.img'