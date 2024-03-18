cd src/ &&
./adoq $1 &&
less +G -S -R -n results.ans ;
cd ..
