for f in nbas/*.hoa; do
    nf=$(basename $f);
    echo $nf;
    timeout 1m ../../../code/nbautils/build/bin/nbadet -u1 -k -j -t -i -r -m -o -d $f > "dpas/${nf}";
done

