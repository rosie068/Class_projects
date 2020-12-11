#Written by Rosemary He
#this sciprt focuses on regex expressions to turn an English dict into a Hawaiian dict

#!/bin/bash                                                                                                                   

#remove ?                                                                                                                     
sed 's/\?//g' |
#remove <u>                                                                                                                   
sed 's/<u>//g' |
#remove </u>                                                                                                                  
sed 's/<\/u>//g' |
#translate ` to ' and - to ' '                                                                                                
tr '`' "'" | tr '-' ' ' |
#grab hawaiian letters only                                                                                                   
grep -i " *<td[^>]*>[pk'mnwlhaeiou ]\+<\/td> *" |
sed 's/ *<td[^>]*>//g' | sed 's/<\/td> *//g' |
#squeeze ' ' to \n                                                                                                            
tr -s '[:space:]' '[\n*]' |
#change all uppercase to lowercase                                                                                            
tr '[:upper:]' '[:lower:]' |
sort -u
