encoder="$1"
cracker="$2"
decoder="$3"

RED='\033[0;31m'
GREEN='\033[0;32m'
NO_COLOR='\033[0m'

for keyFile in ./Key*
do
  keyFile="${keyFile:2}"
  echo "Checking key $keyFile..."

  echo "    Encoding Input.txt with $keyFile ..."
  encoded="Temp${keyFile}.EncodedOutput.txt"
  $encoder Input.txt "$keyFile" "$encoded" > /dev/null

  echo "    Cracking $encoded key..."
  cracked="Temp${keyFile}.CrackedKeyOutput.txt"
  $cracker "$encoded" "$cracked" > /dev/null

  echo "    Decoding $encoded..."
  decoded="Temp${keyFile}.DecodedCrackedOutput.txt"
  $decoder "$encoded" "$cracked" "$decoded"

  diff -i Input.txt "$decoded" > /dev/null
  diffResult=$?

  if [ $diffResult -eq 0 ]; then
    printf "    ${GREEN}Success:${NO_COLOR} cracked keyFile is equal to initial!\n"
  else
    printf "    ${RED}Failed:${NO_COLOR} cracked keyFile does not match initial!\n"
  fi
done
