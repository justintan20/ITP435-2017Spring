#!/bin/bash
declare -a emails=("madhav@usc.edu" \
"nicholgw@usc.edu" \
"drousso@usc.edu" \
"klecan@usc.edu" \
"peshave@usc.edu" )

#Get the git remote URL
gitremote=$(git config --get remote.origin.url)
#echo "$gitremote"

# Extract username
username=${gitremote%/*}
username=${username##*/}
#echo "$username"

# Extract repo slug
repo=${gitremote%.*}
repo=${repo##*/}
#echo "$repo"

# Ask for password
read -s -p "Bitbucket password: " password
echo ""
for i in "${emails[@]}"
do
    #echo "$i"
    #Concatenate the string (bash is fun!)
    url="https://api.bitbucket.org/1.0/privileges/"
    url="$url$username"
    url="$url/"
    url="$url$repo"
    url="$url/"
    url="$url$i"
    #echo "$url"
    status=$(curl -s -o /dev/null --request PUT --user "$username":"$password" --write-out %{http_code} $url --data read)
    echo "$status"
    if [ $status -eq 401 ]
    then
        echo "Invalid password. Please try again."
        break
    fi
    #
done

if [ -f "_nul" ]
then
    rm "_nul"
fi