# vars
cmsgl="GCC   :   '"
nmsgl="NASM  :   '"
cmsgm="' -> '"
cmsge="'"
outdir="bin/objs/"
cdir="kernel/src/"
inf=""
outf=""

# Clear the console
clear

# Setup build directory
rm -r "bin"
mkdir "bin"
mkdir "bin/kernel/"
mkdir "bin/kernel/objs/"

# boot.asm
cdir="kernel/src/asm/" outdir="bin/" inf="boot.asm" outf="boot.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# registers.asm
cdir="kernel/src/asm/" outdir="bin/kernel/objs/" inf="registers.asm" outf="registers.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# irqs.asm
cdir="kernel/src/asm/" outdir="bin/kernel/objs/" inf="irqs.asm" outf="irqs.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# realmode.asm
cdir="kernel/src/asm/" outdir="bin/kernel/objs/" inf="realmode.asm" outf="realmode.o"
nasm -felf32 "$cdir$inf" -o "$outdir$outf"
echo "$nmsgl$cdir$inf$cmsgm$outdir$outf$cmsge"

# ----------------------------------------------- KERNEL --------------------------------------------------------------
cdir="kernel/src/"
outdir="bin/kernel/objs/"
files=$(find $cdir -name "*.cpp" -print)

for file in $files; do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -Wall -w -Ikernel/include -Ilib/include -c "$file" -o "$outdir$outf" -fno-use-cxa-atexit -fno-exceptions -fno-rtti -nostdlib -ffreestanding -O2 -Wextra -fshort-enums -fsanitize=undefined -DKERNEL
    echo "$cmsgl$inf$cmsgm$outdir$outf$cmsge"
done

# ----------------------------------------------- LIB -----------------------------------------------------------------
cdir="lib/src/"
outdir="bin/kernel/objs/"
files=$(find $cdir -name "*.cpp" -print)

for file in $files; do
    inf="$(basename $file)"
    outf="$(echo ${inf%.*}.o)"
    i686-elf-g++ -Wall -w -Ikernel/include -Ilib/include -c "$file" -o "$outdir$outf" -fno-use-cxa-atexit -fno-exceptions -fno-rtti -nostdlib -ffreestanding -O2 -Wextra -fshort-enums -fsanitize=undefined -DKERNEL
    echo "$cmsgl$inf$cmsgm$outdir$outf$cmsge"
done

# Link all files
cd 'bin/kernel/objs'
i686-elf-ld -T '../../../build/linker.ld' -o '../../kernel.bin' "../../boot.o" *.o
cd '../../../'

objdump -t -dwarf -Mintel "bin/kernel.bin" > "build/dumps/kernel.dump"
