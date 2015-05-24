stty -echo -icanon time 0 min 0
  key=$( dd bs=1 count=1 2> /dev/null; printf "." )
  key=${key%.}
  case $key in
	?) printf "%d\n" "'$key";; 
  esac
stty sane
