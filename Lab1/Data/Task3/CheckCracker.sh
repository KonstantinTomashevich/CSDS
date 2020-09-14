encoder="$1"
cracker="$2"
decoder="$3"

RED='\033[0;31m'
GREEN='\033[0;32m'
NO_COLOR='\033[0m'

for file in ./Input*
do
  file="${file:2}"
  echo "Checking $file..."

  echo "    Encoding $file with Key.txt..."
  encoded="Temp${file}.EncodedOutput.txt"
  $encoder "$file" Key.txt "$encoded" > /dev/null

  echo "    Cracking $encoded key..."
  cracked="Temp${file}.CrackedKeyOutput.txt"
  $cracker "$encoded" "$cracked" > /dev/null

  echo "    Decoding $encoded..."
  decoded="Temp${file}.DecodedCrackedOutput.txt"
  $decoder "$encoded" "$cracked" "$decoded"

  diff -i "$file" "$decoded" > /dev/null
  diffResult=$?

  if [ $diffResult -eq 0 ]; then
    printf "    ${GREEN}Success:${NO_COLOR} cracked file is equal to initial!\n"
  else
    printf "    ${RED}Failed:${NO_COLOR} cracked file does not match initial!\n"
  fi
done
