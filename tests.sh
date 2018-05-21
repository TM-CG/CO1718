# === Script auto tests ===
#ver 17-05-2018 11:03
##############################################################################
# The script must be put in the parent folder of project
# It should exist in the parent project folder a folder with tests named tests
# to run cd to project folder and execute ../tests.sh
# Execute:
#  ../tests.sh debuginfo fail_only
# to show aditional debug when fail and stop only when a test fail.
##############################################################################
#var stats
failed=0
passed=0
yydebug=$YYDEBUG

args=("$@")

clear
echo "Compiling ..."
make

export YYDEBUG=0

if [ $? -ne 0 ]; then
   echo "$(tput setaf 1)$(tput setab 7)COMPILE FAILURE $(tput sgr 0)"
   exit 1
fi
for f in ../tests/*.gr8; do
  clear 						
  echo    "╔══════════════════════════════════════════════╗"
  echo    "║Running test $f" 
  echo    "║Already Passed: $passed	Already Failed:$failed"
  echo    "╚══════════════════════════════════════════════╝"
  echo "=== Test content: ==="
  cat $f
  echo "====================="
  #./gr8 $f -o teste.xml
  ./gr8 $f
  if [ $? -ne 0 ]; then
    echo "$(tput setaf 1)$(tput setab 7)FAILURE: Program closes unexpected $(tput sgr 0)"
    if [ ${args[0]} -eq "debuginfo" ]; then
    	echo    "╔══════════════════════════════════════════════╗"
    	echo    "║DEBUG INFORMATION ABOUT ERROR:                ║"
    	echo    "╚══════════════════════════════════════════════╝"
    	#uses valgrind to show segfault or sigabort errors
    	valgrind ./gr8 $f
    fi
    ((failed = failed + 1))
    read -rsp $'Press any key to continue...\n' -n1 key
  else
    # === TESTING OUTPUT DIFFERENCES ==
    filename=$(echo "$f" | sed -r "s/.+\/(.+)\..+/\1/")
    yasm -felf32 ../tests/${filename}.asm -o ../tests/${filename}.o

    ld ../tests/${filename}.o -o ../tests/${filename} -m elf_i386 -L ${HOME}/compiladores/root/usr/lib -lrts

    timeout 10s ./../tests/${filename} > ../tests/${filename}.myout
    if [ $? -ne 0 ]; then
      #possible infinite loop so clean up huge myout file
      rm ../tests/${filename}.myout
      echo "$(tput setaf 1)$(tput setab 7)ERROR: Time Limit Exceeded  $(tput sgr 0)"
      echo "Expected output:"
      cat ../tests/expected/${filename}.out
      read -rsp $'Press any key to continue...\n' -n1 key
    else
      diff --ignore-all-space ../tests/expected/${filename}.out ../tests/${filename}.myout 
    if [ $? -ne 0 ]; then
     ((failed = failed + 1))
     echo "$(tput setaf 1)$(tput setab 7)ERROR: Output assert error  $(tput sgr 0)"
     read -rsp $'Press any key to continue...\n' -n1 key
    else
      echo "$(tput setaf 2)$(tput setab 7)SUCCESS: OK! $(tput sgr 0)" 
     ((passed = passed + 1))
     if [ -z "$1" ]; then
      read -rsp $'Press any key to continue...\n' -n1 key
     fi
     fi
    fi
  fi
done

#restore debug option
export YYDEBUG=$yydebug
  echo    "╔══════════════════════════════════════════════╗"
  echo    "║ STATS" 
  echo    "║Passed: $passed	Failed:$failed"
  echo    "╚══════════════════════════════════════════════╝"
              
